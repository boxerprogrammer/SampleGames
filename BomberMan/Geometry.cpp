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
	auto len = Length();//�傫���𑪂�
	if (len == 0.0f) {//�[�����Z�h�~
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

void Rect::SetLTRB(float l, float t, float r, float b)
{
	center.x = (r + l) / 2;
	center.y = (b + t) / 2;
	size.w = static_cast<int>(r - l);
	size.h = static_cast<int>(b - t);
}

void Rect::Draw(unsigned int col)const
{
	DrawBoxAA(Left(),  
				Top(), 
				Right(), 
				Bottom(), col, false, 2.0f);
}

Size GetOverlappedSize(const Rect& rcA, const Rect& rcB)
{
	auto diff = rcB.center - rcA.center;
	return {
	static_cast<int>((rcA.size.w + rcB.size.w) / 2 - fabsf(diff.x)),
	static_cast<int>((rcA.size.h + rcB.size.h) / 2 - fabsf(diff.y))
	};
}

Vector2 GetAdjustVector(const Rect& rcA, const Rect& rcB)
{
	auto diff = rcB.center - rcA.center;
	float xsign = (diff.x) / fabsf(diff.x);
	float ysign = (diff.y) / fabsf(diff.y);
	Size overlap = GetOverlappedSize(rcA, rcB);
	if (overlap.w < overlap.h) {
		return  { -overlap.w * xsign ,0.0f};
	}
	else {
		return { 0.0f , -overlap.h * ysign };
	}
}

