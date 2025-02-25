#include<DxLib.h>
#include "Player.h"
#include"../Input.h"
#include"../Scene/GameScene.h"
#include<cmath>
#include<cassert>
#include<algorithm>
constexpr int anim_interval = 7;//アニメの絵一枚あたりの表示フレーム
constexpr int anim_num = 8;//一つの動作に対するアニメの枚数
constexpr int cut_width = 48;//切り抜き幅
constexpr int cut_height = 64;//切り抜き高さ
constexpr float draw_scale = 2.0f;//拡大率
constexpr int center_pos_x = 48/2;//画像中心X
constexpr int center_pos_y = 42;//画像中心Y
constexpr float move_speed = 3.0f;//移動速度
constexpr float collision_rate = 0.95f;//画像のサイズにするとでかすぎるので調整値
//中心点が画像中心からズレているので、ズレ分を計算しておく
constexpr float collsion_adjust_y = -(center_pos_y - cut_height / 2);

void Player::IdleUpdate(Input& input)
{
	++animFrame_;
	if (Move(input)) {
		update_ = &Player::WalkUpdate;
		draw_ =&Player::WalkDraw;
		animFrame_ = 0;
	}
}

void Player::IdleDraw()
{
	//現在の表示すべきインデックスに変換
	auto idx = (animFrame_ / anim_interval) % anim_num;
	//idx = std::max(idx, anim_num-1);
	idx = std::clamp(idx, 0, anim_num - 1);
	//インデックスから切り抜く画像のX座標を計算している
	DrawRectRotaGraph2F(pos_.x, pos_.y,//表示中心座標
		cut_width * idx, cut_height * static_cast<int>(currentDir_),//切り取り開始座標
		cut_width, cut_height,//切り抜く幅、高さ
		center_pos_x, center_pos_y,//画像中心座標
		draw_scale, 0.0f,//拡大率と回転角
		idleH_, true);
}

void Player::WalkUpdate(Input& input)
{
	++animFrame_;
	if (!Move(input)) {
		update_ = &Player::IdleUpdate;
		draw_ = &Player::IdleDraw;
		animFrame_ = 0;
	}
}

void Player::WalkDraw()
{
	//現在の表示すべきインデックスに変換
	auto idx = (animFrame_ / anim_interval) % anim_num;
	//インデックスから切り抜く画像のX座標を計算している
	DrawRectRotaGraph2F(pos_.x, pos_.y,//表示中心座標
		cut_width * idx, cut_height * static_cast<int>(currentDir_),//切り取り開始座標
		cut_width, cut_height,//切り抜く幅、高さ
		center_pos_x, center_pos_y,//画像中心座標
		draw_scale, 0.0f,//拡大率と回転角
		walkH_, true);
}

void Player::DeadUpdate(Input&)
{
	++animFrame_;
	if (animFrame_ > anim_interval * anim_num) {
		update_ = &Player::DeadAfterUpdate;
		animFrame_ = anim_interval * (anim_num - 1);
		frame_ = 0;
	}
}

void Player::DeadAfterUpdate(Input&)
{
	++frame_;
	if (frame_ > 60) {
		//復活
		SetPosition(Position2(center_pos_x, center_pos_y));//初期座標へ
		collision_.Enable();
		frame_ = 0;
		animFrame_ = 0;
		update_ = &Player::IdleUpdate;
		draw_ = &Player::IdleDraw;
	}
}


void Player::DeadDraw()
{
	if ((frame_ / 5) % 2 == 1)return;
	//現在の表示すべきインデックスに変換
	auto idx = (animFrame_ / anim_interval);
	//インデックスから切り抜く画像のX座標を計算している
	DrawRectRotaGraph2F(pos_.x, pos_.y,//表示中心座標
		cut_width * idx, cut_height * static_cast<int>(currentDir_),//切り取り開始座標
		cut_width, cut_height,//切り抜く幅、高さ
		center_pos_x, center_pos_y,//画像中心座標
		draw_scale, 0.0f,//拡大率と回転角
		deathH_, true);
}

Player::Player(GameScene& gameScene):Actor(gameScene)
{
	update_ = &Player::IdleUpdate;
	draw_ = &Player::IdleDraw;
	idleH_ = LoadGraph(L"img/Player/idle.png");
	assert(idleH_);
	deathH_ = LoadGraph(L"img/Player/death.png");
	assert(deathH_);
	walkH_ = LoadGraph(L"img/Player/walk.png");
	assert(walkH_);
	pos_ = { center_pos_x,center_pos_y };
	collision_.SetType(CollisionType::Player);
	collision_.SetRect(Rect({pos_.x,
							pos_.y+ collsion_adjust_y *draw_scale},//Y方向に中心点がずれてるので補正
							{static_cast<int>(cut_width* collision_rate) ,
							static_cast<int>(cut_width* collision_rate) }));
	
}

Player::~Player()
{
}

void Player::Update(Input& input)
{
	(this->*update_)(input);
}

bool Player::Move(Input& input)
{
	Vector2 vec = {};
	if (input.IsPressed("up")) {
		currentDir_ = Direction::up;
		vec.y -= 1.0f;
	}
	else if (input.IsPressed("down")) {
		currentDir_ = Direction::down;
		vec.y += 1.0f;
	}
	if (input.IsPressed("left")) {
		currentDir_ = Direction::left;
		vec.x -= 1.0f;
	}
	else if (input.IsPressed("right")) {
		currentDir_ = Direction::right;
		vec.x += 1.0f;
	}
	SetBomb(input);
	bool isMoved = (vec.Length() > 0.0f);
	Position2 pos = pos_;
	pos += vec.Normalized() * move_speed;
	SetPosition(pos);
	return isMoved;
}

void Player::Draw()
{
	(this->*draw_)();
	collision_.GetRect().Draw(0xffffffff);
}

void Player::OnHit(const Collision& coll)
{
	if (coll.GetType() == CollisionType::Block) {
		OnHitBlock(coll);
	}
	else if (coll.GetType() == CollisionType::Enemy) {
		update_ = &Player::DeadUpdate;
		draw_ = &Player::DeadDraw;
		collision_.Disable();
		animFrame_ = 0;
	}
}

void Player::SetPosition(const Position2& pos)
{
	pos_ = pos;
	collision_.SetPosition({ pos.x,pos.y - (center_pos_y - cut_height / 2) * 2 });
}

void Player::OnHitBlock(const Collision& coll)
{
	Position2 pos = pos_;
	auto& selfRc = collision_.GetRect();
	auto& targetRc = coll.GetRect();
	pos+=GetAdjustVector(selfRc, targetRc);
	SetPosition(pos);
}

void Player::SetBomb(Input& input)
{
	if (input.IsTrigger("action")) {
		gameScene_.SetBomb(pos_);
	}
	
}
