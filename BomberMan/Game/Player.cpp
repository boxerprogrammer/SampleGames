#include<DxLib.h>
#include "Player.h"
#include"../Input.h"
#include"../Scene/GameScene.h"
#include<cmath>
#include<cassert>
#include<algorithm>
constexpr int anim_interval = 7;//�A�j���̊G�ꖇ������̕\���t���[��
constexpr int anim_num = 8;//��̓���ɑ΂���A�j���̖���
constexpr int cut_width = 48;//�؂蔲����
constexpr int cut_height = 64;//�؂蔲������
constexpr float draw_scale = 2.0f;//�g�嗦
constexpr int center_pos_x = 48/2;//�摜���SX
constexpr int center_pos_y = 42;//�摜���SY
constexpr float move_speed = 3.0f;//�ړ����x
constexpr float collision_rate = 0.95f;//�摜�̃T�C�Y�ɂ���Ƃł�������̂Œ����l
//���S�_���摜���S����Y���Ă���̂ŁA�Y�������v�Z���Ă���
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
	//���݂̕\�����ׂ��C���f�b�N�X�ɕϊ�
	auto idx = (animFrame_ / anim_interval) % anim_num;
	//idx = std::max(idx, anim_num-1);
	idx = std::clamp(idx, 0, anim_num - 1);
	//�C���f�b�N�X����؂蔲���摜��X���W���v�Z���Ă���
	DrawRectRotaGraph2F(pos_.x, pos_.y,//�\�����S���W
		cut_width * idx, cut_height * static_cast<int>(currentDir_),//�؂���J�n���W
		cut_width, cut_height,//�؂蔲�����A����
		center_pos_x, center_pos_y,//�摜���S���W
		draw_scale, 0.0f,//�g�嗦�Ɖ�]�p
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
	//���݂̕\�����ׂ��C���f�b�N�X�ɕϊ�
	auto idx = (animFrame_ / anim_interval) % anim_num;
	//�C���f�b�N�X����؂蔲���摜��X���W���v�Z���Ă���
	DrawRectRotaGraph2F(pos_.x, pos_.y,//�\�����S���W
		cut_width * idx, cut_height * static_cast<int>(currentDir_),//�؂���J�n���W
		cut_width, cut_height,//�؂蔲�����A����
		center_pos_x, center_pos_y,//�摜���S���W
		draw_scale, 0.0f,//�g�嗦�Ɖ�]�p
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
		//����
		SetPosition(Position2(center_pos_x, center_pos_y));//�������W��
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
	//���݂̕\�����ׂ��C���f�b�N�X�ɕϊ�
	auto idx = (animFrame_ / anim_interval);
	//�C���f�b�N�X����؂蔲���摜��X���W���v�Z���Ă���
	DrawRectRotaGraph2F(pos_.x, pos_.y,//�\�����S���W
		cut_width * idx, cut_height * static_cast<int>(currentDir_),//�؂���J�n���W
		cut_width, cut_height,//�؂蔲�����A����
		center_pos_x, center_pos_y,//�摜���S���W
		draw_scale, 0.0f,//�g�嗦�Ɖ�]�p
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
							pos_.y+ collsion_adjust_y *draw_scale},//Y�����ɒ��S�_������Ă�̂ŕ␳
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
