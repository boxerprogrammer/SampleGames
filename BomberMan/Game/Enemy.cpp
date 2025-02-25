#include "Enemy.h"
#include"../Scene/GameScene.h"
#include<DxLib.h>
#include<cassert>
#include"Collision.h"
constexpr int anim_interval = 7;
constexpr int idle_num = 4;
constexpr float draw_scale = 2.0f;
constexpr int cut_size_w = 64;
constexpr int cut_size_h = 64;
constexpr int coll_size = 20;

Enemy::Enemy(GameScene& gameScene):Actor(gameScene),animFrame_(0)
{
	idleH_=LoadGraph(L"img/enemy/slime_idle.png");
	deathH_ = LoadGraph(L"img/enemy/slime_death.png");
	assert(idleH_ >= 0);
	assert(deathH_ >= 0);
	update_ = &Enemy::IdleUpdate;
	draw_ = &Enemy::IdleDraw;
	collision_.SetRect({ { pos_.x, pos_.y },
		{static_cast<int>(coll_size * draw_scale),static_cast<int>(coll_size * draw_scale)} });
	collision_.SetType(CollisionType::Enemy);
}

Enemy::~Enemy()
{
}

void Enemy::IdleUpdate(Input&)
{
	animFrame_ = (animFrame_ + 1) % (idle_num * anim_interval);
}

void Enemy::IdleDraw()
{
	auto idx = (animFrame_ / anim_interval) % idle_num;
	DrawRectRotaGraph2F(pos_.x, pos_.y,
		idx*cut_size_w,0,
		cut_size_w,cut_size_h,
		32, 42,
		draw_scale, 0.0f,
		idleH_,
		true);
}

void Enemy::WalkUpdate(Input&)
{
}

void Enemy::WalkDraw()
{
}

void Enemy::DeadUpdate(Input&)
{
}

void Enemy::DeadDraw()
{
}

bool Enemy::Move(Input& input)
{
	return false;
}



void Enemy::Update(Input& input)
{
	(this->*update_)(input);
}

void Enemy::Draw()
{
	(this->*draw_)();
	collision_.GetRect().Draw(0x0000ff);
}

void Enemy::OnHit(const Collision& coll)
{
	if(coll.GetType()==CollisionType::Blast){
	}
}

void Enemy::SetPosition(const Position2& pos)
{
	pos_ = pos;
	
	collision_.SetPosition({ pos_.x, pos_.y });
}
