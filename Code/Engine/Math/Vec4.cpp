#include "Engine/Math/Vec4.hpp"


//
//constructor
//
Vec4::Vec4(float initialX, float initialY, float initialZ, float initialW)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
	, w(initialW)
{
}


//
//const operators
//
bool Vec4::operator==(Vec4 const& compare) const
{
	if (x == compare.x && y == compare.y && z == compare.z && w == compare.w) return true;
	else return false;
}


bool Vec4::operator!=(Vec4 const& compare) const
{
	if (x != compare.x || y != compare.y || z != compare.z || w != compare.w) return true;
	else return false;
}


Vec4 const Vec4::operator+(Vec4 const& vecToAdd) const
{
	return Vec4(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z, w + vecToAdd.w);
}


Vec4 const Vec4::operator-(Vec4 const& vecToSubtract) const
{
	return Vec4(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z, w - vecToSubtract.w);
}


Vec4 const Vec4::operator*(float uniformScale) const
{
	return Vec4(x * uniformScale, y * uniformScale, z * uniformScale, w * uniformScale);
}


Vec4 const Vec4::operator/(float inverseScale) const
{
	float scale = 1.f / inverseScale;
	return Vec4(x * scale, y * scale, z * scale, w * scale);
}


//
//non-const operators
//
void Vec4::operator+=(Vec4 const& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
	w += vecToAdd.w;
}


void Vec4::operator-=(Vec4 const& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
	w -= vecToSubtract.w;
}


void Vec4::operator*=(float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
	w *= uniformScale;
}


void Vec4::operator/=(float uniformDivisor)
{
	float scale = 1.f / uniformDivisor;
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}


void Vec4::operator=(Vec4 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
	w = copyFrom.w;
}
