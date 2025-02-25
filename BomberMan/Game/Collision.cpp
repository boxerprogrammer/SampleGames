#include "Collision.h"

Collision::Collision(Actor& owner, const Rect& rect, CollisionType type):
	owner_(owner),
	rect_(rect),
	type_(type)
{
}

Collision::Collision(Actor& owner):owner_(owner)
{
}

void Collision::SetRect(const Rect& rc)
{
	rect_ = rc;
	isMoved_ = true;
}

void Collision::SetType(CollisionType type)
{
	type_ = type;
}

void Collision::SetPosition(const Position2& pos)
{
	if (rect_.center.x == pos.x && rect_.center.y == pos.y) {
		isMoved_ = false;
		return;
	}
	rect_.center = pos;
	isMoved_ = true;
}

CollisionType Collision::GetType() const
{
	return type_;
}

const Rect& Collision::GetRect() const
{
	return rect_;
}

bool Collision::IsMoved() const
{
	return isMoved_;
}

bool Collision::IsDisable() const
{
	return isDisable_;
}

void Collision::Disable()
{
	isDisable_ = true;
}

void Collision::Enable()
{
	isDisable_ = false;
}
