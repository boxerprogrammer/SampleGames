#include "Bomb.h"
#include"../Scene/GameScene.h"
#include<DxLib.h>
#include<cassert>

constexpr int draw_center_x = 180;
constexpr int draw_center_y = 204;
constexpr int cut_width = 512;
constexpr int cut_height = 512;
constexpr int anim_num = 5 * 5;
constexpr int anim_interval = 5;
constexpr float draw_scale = 0.5f;
constexpr float collision_rate = 0.25f;
constexpr int coll_size = 96;
constexpr int wait_pict_num = 8;


void Bomb::WaitUpdate()
{
	if (++animFrame_ >= anim_interval * wait_pict_num) {
		update_ = &Bomb::BurstUpdate;
		gameScene_.SetBlastH(pos_, power_);
		gameScene_.SetBlastV(pos_, power_); 
		animFrame_ = 0;
	}
}

void Bomb::BurstUpdate()
{
	if (++animFrame_ >= anim_interval*anim_num) {
		isDead_ = true;
	}
}

Bomb::Bomb(GameScene& gameScene, const Position2& pos):Actor(gameScene)
{
	pos_ = pos;
	handle_ = LoadGraph(L"img/bomb/bomb.png");
	assert(handle_);
	collision_.SetType(CollisionType::Bomb);
	collision_.SetRect(Rect(pos_, { 
						static_cast<int>(coll_size* draw_scale),
						static_cast<int>(coll_size* draw_scale) }));
	update_ = &Bomb::WaitUpdate;
}

void Bomb::Update(Input& input)
{
	(this->*update_)();
}

void Bomb::Draw()
{
	if(animFrame_>=anim_interval*anim_num){
		return;
	}
	auto idx = (animFrame_ / anim_interval);
	auto idxX = idx % 5;
	auto idxY = idx / 5;
	//�C���f�b�N�X����؂蔲���摜��X���W���v�Z���Ă���
	DrawRectRotaGraph2F(pos_.x, pos_.y,//�\�����S���W
		cut_width * idxX,cut_height*idxY,
		cut_width, cut_height,//�؂蔲�����A����
		draw_center_x, draw_center_y,//�摜���S���W
		draw_scale, 0.0f,//�g�嗦�Ɖ�]�p
		handle_, true);
	collision_.GetRect().Draw();
}

void Bomb::OnHit(const Collision& collision)
{
}
