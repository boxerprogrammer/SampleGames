#include "Geometry.h"
#include<cmath>
#include<DxLib.h>
Vector2 
Vector2::operator+(const Vector2& rval) const
{
	return { x + rval.x, y + rval.y };
}
Vector2 
Vector2::operator-(const Vector2& rval) const
{
	return { x - rval.x, y - rval.y };
}
void Vector2::operator+=(const Vector2& vec)
{
	x += vec.x;
	y += vec.y;
}

void Vector2::operator-=(const Vector2& vec)
{
	x -= vec.x;
	y -= vec.y;
}

Vector2 
Vector2::operator*(float scale) const
{
	return { x * scale,y * scale };
}

void 
Vector2::operator*=(float scale)
{
	x *= scale;
	y *= scale;
}

Vector2 
Vector2::operator/(float div) const
{
	if (div == 0.0f) {
		return {};
	}
	return { x * div,y * div};
}

void Vector2::operator/=(float div)
{
	if (div == 0.0f) {
		x = 0.0f;
		y = 0.0f;
		return;
	}
	x /= div;
	y /= div;
}

void Vector2::Normalize()
{
	auto len = Length();//ëÂÇ´Ç≥Çë™ÇÈ
	if (len == 0.0f) {//É[ÉçèúéZñhé~
		x = 0.0f;
		y = 0.0f;
		return;
	}
	x /= len;
	y /= len;
}

Vector2 
Vector2::Normalized() const
{
	auto len = Length();
	if (len == 0.0f) {
		return {};
	}
	return { x / len,y / len };
}

float Vector2::Length() const
{
	//return sqrt(x * x + y * y);
	return hypotf(x, y);
}

float Rect::Left() const
{
	return center.x - size.w / 2;
}

float Rect::Top() const
{
	return center.y - size.h / 2;
}

float Rect::Right() const
{
	return center.x + size.w / 2;
}

float Rect::Bottom() const
{
	return center.y + size.h / 2;
}

Rect::Rect(const Position2& c, const Size& sz)
{
	center = c;
	size = sz;
}

void Rect::Draw(unsigned int col)const
{
	DrawBoxAA(Left(),  
				Top(), 
				Right(), 
				Bottom(), col, false, 2.0f);
}
