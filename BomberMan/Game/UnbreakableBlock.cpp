#include "UnbreakableBlock.h"
#include<DxLib.h>
#include<cassert>

namespace {
	constexpr int chip_idx_x = 3;
	constexpr int chip_idx_y = 11;
	constexpr float chip_scale = 3.0f;
	constexpr int chip_size = 16;
	constexpr float collision_rate = 0.95f;//画像のサイズにするとでかすぎるので調整値
	//中心点が画像中心からズレているので、ズレ分を計算しておく
}

UnbreakableBlock::UnbreakableBlock(GameScene& gameScene, const Position2& pos):Actor(gameScene)
{
	pos_ = pos;
	handle_ = LoadGraph(L"img/texture.png");
	collision_.SetType(CollisionType::Block);
	collision_.SetRect(Rect(pos_, { 
									static_cast<int>(chip_size * (chip_scale* collision_rate)),
									static_cast<int>(chip_size * (chip_scale* collision_rate)) }));
	assert(handle_ >= 0);
}

void UnbreakableBlock::Update(Input& input)
{
}

void UnbreakableBlock::Draw()
{
	DrawRectRotaGraphF(pos_.x, pos_.y,//表示座標
		chip_size * chip_idx_x,chip_size*chip_idx_y,//切り抜き左上
		chip_size, chip_size,//切り抜き幅
		chip_scale, 0.0f,//切り抜き高さ
		handle_, true);
	collision_.GetRect().Draw(0xff0000);
}

void UnbreakableBlock::OnHit(const Collision& collision)
{
}
