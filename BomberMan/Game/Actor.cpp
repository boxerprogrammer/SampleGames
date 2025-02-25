#include "Actor.h"

Actor::Actor(GameScene& gameScene):gameScene_(gameScene),collision_(*this)
{
}

void 
Actor::SetPosition(const Position2& pos)
{
	pos_ = pos;
}

const Collision& Actor::GetCollision() const
{
	return collision_;
}
