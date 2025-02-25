#include "UnbreakableBlock.h"
#include<DxLib.h>
#include<cassert>

namespace {
	constexpr int chip_idx_x = 3;
	constexpr int chip_idx_y = 11;
	constexpr float chip_scale = 3.0f;
	constexpr int chip_size = 16;
	constexpr float collision_rate = 0.95f;//�摜�̃T�C�Y�ɂ���Ƃł�������̂Œ����l
	//���S�_���摜���S����Y���Ă���̂ŁA�Y�������v�Z���Ă���
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
	DrawRectRotaGraphF(pos_.x, pos_.y,//�\�����W
		chip_size * chip_idx_x,chip_size*chip_idx_y,//�؂蔲������
		chip_size, chip_size,//�؂蔲����
		chip_scale, 0.0f,//�؂蔲������
		handle_, true);
	collision_.GetRect().Draw(0xff0000);
}

void UnbreakableBlock::OnHit(const Collision& collision)
{
}
