#include "Blast.h"
#include "../Scene/GameScene.h"
#include<DxLib.h>
#include<cassert>
#include"Collision.h"
constexpr int block_size = 48;
constexpr int spread_frame = 40;
constexpr int keep_frame = 30;
constexpr int disappear_frame = 30;

void Blast::SpreadUpdate()
{
	const float rate = static_cast<float>(frame_) / spread_frame;
	if (dir_ == BlastDirection::Horizontal) {
		collision_.SetRect(Rect(pos_, 
			{ 
				static_cast<int>(block_size * power_*rate),
				static_cast<int>(block_size*rate) 
			}));
	}
	else {
		collision_.SetRect(Rect(pos_, 
			{ 
				static_cast<int>(block_size*rate) ,
				static_cast<int>(block_size * power_*rate)
			}));
	}
	if(++frame_>=spread_frame){
		update_ = &Blast::KeepUpdate;
		draw_ = &Blast::KeepDraw;
		
		frame_ = 0;
	}
}

void Blast::KeepUpdate()
{
	if(++frame_>=keep_frame){
		update_ = &Blast::DisappearUpdate;
		draw_ = &Blast::DisappearDraw;
		frame_ = 0;
	}
}

void Blast::DisappearUpdate()
{
	if (++frame_ >= disappear_frame) {
		//è¡ñ≈
		isDead_ = true;
	}
}

void Blast::SpreadDraw()
{
	const Collision& coll = GetCollision();
	const Rect& rc = coll.GetRect();
	const float rate = static_cast<float>(frame_) / spread_frame;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, rate*255);
	DrawBoxAA(rc.Left(), rc.Top(), rc.Right(), rc.Bottom(), 0xffaa00, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Blast::KeepDraw()
{
	const Collision& coll = GetCollision();
	const Rect& rc = coll.GetRect();
	DrawBoxAA(rc.Left(), rc.Top(), rc.Right(), rc.Bottom(), 0xffaa00, true);
}

void Blast::DisappearDraw()
{
	const Collision& coll = GetCollision();
	const Rect& rc = coll.GetRect();
	const float rate = static_cast<float>(disappear_frame-frame_) / disappear_frame;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, rate * 255);
	DrawBoxAA(rc.Left(), rc.Top(), rc.Right(), rc.Bottom(), 0xff0000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

Blast::Blast(GameScene& gameScene, const Position2& pos, BlastDirection dir, int power):Actor(gameScene),dir_(dir),power_(power*2+1)
{
	pos_ = pos;
	update_ = &Blast::SpreadUpdate;
	draw_ = &Blast::SpreadDraw;
	collision_.SetType(CollisionType::Blast);
	collision_.SetRect(Rect(pos_, { 0,0}));
}

void Blast::Update(Input& input)
{
	(this->*update_)();
}

void Blast::Draw()
{
	(this->*draw_)();
	collision_.GetRect().Draw(0xff0000);
}

void Blast::OnHit(const Collision& collision)
{
	if (collision.GetType() == CollisionType::Block) {
		auto size = GetOverlappedSize(collision.GetRect(), collision_.GetRect());
		if (dir_ == BlastDirection::Horizontal) {
			if (size.h < block_size / 2) {
				return;
			}
			const auto& blockRc = collision.GetRect();
			Rect rc = collision_.GetRect();
			if (pos_.x<blockRc.center.x) {
				rc.SetLTRB(rc.Left(), rc.Top(), blockRc.Left(), rc.Bottom());
			}else{
				rc.SetLTRB(blockRc.Right(), rc.Top(), rc.Right(), rc.Bottom());
			}
			collision_.SetRect(rc);
		}
		else {
			if (size.w < block_size / 2) {
				return;
			}
			const auto& blockRc = collision.GetRect();
			Rect rc = collision_.GetRect();
			if (pos_.y < blockRc.center.y) {
				rc.SetLTRB(rc.Left(), rc.Top(), rc.Right(), blockRc.Top());
			}
			else {
				rc.SetLTRB(rc.Left(), blockRc.Bottom(), rc.Right(), rc.Bottom());
			}
			collision_.SetRect(rc);
		}
	}
}
