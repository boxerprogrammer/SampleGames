#include "Bomb.h"
#include<DxLib.h>
#include<cassert>

constexpr int draw_center_x = 180;
constexpr int draw_center_y = 204;
constexpr int cut_width = 512;
constexpr int cut_height = 512;
constexpr int anim_num = 5 * 5;
constexpr int anim_interval = 7;
constexpr float draw_scale = 2.0f;


Bomb::Bomb(GameScene& gameScene, const Position2& pos):Actor(gameScene)
{
	pos_ = pos;
	handle_ = LoadGraph(L"img/bomb/bomb.png");
	assert(handle_);
}

void Bomb::Update(Input& input)
{
	animFrame_ = (animFrame_ + 1) % (anim_interval * anim_num);
}

void Bomb::Draw()
{
	auto idx = (animFrame_ / anim_interval);
	auto idxX = idx % 5;
	auto idxY = idx / 5;
	//インデックスから切り抜く画像のX座標を計算している
	DrawRectRotaGraph2F(pos_.x, pos_.y,//表示中心座標
		cut_width * idxX,cut_height*idxY,
		cut_width, cut_height,//切り抜く幅、高さ
		draw_center_x, draw_center_y,//画像中心座標
		draw_scale, 0.0f,//拡大率と回転角
		handle_, true);
}

void Bomb::OnHit(const Collision& collision)
{
}
