#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <math.h>


//
//constructors
//
Vec2::Vec2(Vec2 const& copy)
	: x(copy.x)
	, y(copy.y)
{
}


Vec2::Vec2(float initialX, float initialY)
	: x(initialX)
	, y(initialY)
{
}


//
//creation functions
//
Vec2 const Vec2::MakeFromPolarDegrees(float orientationDegrees, float length)
{
	return Vec2(length * CosDegrees(orientationDegrees), length * SinDegrees(orientationDegrees));
}


Vec2 const Vec2::MakeFromPolarRadians(float orientationRadians, float length)
{
	return Vec2(length * cosf(orientationRadians), length * sinf(orientationRadians));
}


//
//accessors
//
float Vec2::GetLength() const
{
	return GetDistance2D(Vec2(0.f, 0.f), Vec2(x, y));
}


float Vec2::GetLengthSquared() const
{
	return GetDistanceSquared2D(Vec2(0.f, 0.f), Vec2(x, y));
}


float Vec2::GetOrientationDegrees() const
{
	return Atan2Degrees(y, x);
}


float Vec2::GetOrientationRadians() const
{
	return atan2f(y, x);
}


Vec2 const Vec2::GetRotated90Degrees() const
{
	return Vec2(-y, x);
}


Vec2 const Vec2::GetRotatedMinus90Degrees() const
{
	return Vec2(y, -x);
}


Vec2 const Vec2::GetRotatedDegrees(float degrees) const
{
	float theta = Atan2Degrees(y, x);
	float r = sqrtf((x * x) + (y * y));

	theta += degrees;
	
	return Vec2(r * CosDegrees(theta), r * SinDegrees(theta));
}


Vec2 const Vec2::GetRotatedRadians(float radians) const
{
	float theta = atan2f(y, x);
	float r = sqrtf((x * x) + (y * y));

	theta += radians;
	
	return Vec2(r * cosf(theta), r * sinf(theta));
}


Vec2 const Vec2::GetNormalized() const
{
	float length = GetLength();

	if (length == 0.0f) return *this;

	return *this / length;
}


Vec2 const Vec2::GetReflected(Vec2 const& surfaceNormal) const
{
	Vec2 projectedOnNormal = surfaceNormal * DotProduct2D(*this, surfaceNormal);

	Vec2 projectedOnSurface = *this - projectedOnNormal;

	return projectedOnSurface - projectedOnNormal;
}


Vec2 const Vec2::GetClamped(float maxLength) const
{
	float theta = atan2f(y, x);
	float r = sqrtf((x * x) + (y * y));

	if (r > maxLength) r = maxLength;
	
	return Vec2(r * cosf(theta), r * sinf(theta));
}


//
//mutators
//
void Vec2::SetOrientationDegrees(float newOrientationDegrees)
{
	float theta = Atan2Degrees(y, x);
	float r = sqrtf((x * x) + (y * y));

	theta = newOrientationDegrees;

	x = r * CosDegrees(theta);
	y = r * SinDegrees(theta);
}


void Vec2::SetOrientationRadians(float newOrientationRadians)
{
	float theta = atan2f(y, x);
	float r = sqrtf((x * x) + (y * y));

	theta = newOrientationRadians;

	x = r * cosf(theta);
	y = r * sinf(theta);
}


void Vec2::SetPolarDegrees(float newOrientationDegrees, float newLength)
{
	x = newLength * CosDegrees(newOrientationDegrees);
	y = newLength * SinDegrees(newOrientationDegrees);
}


void Vec2::SetPolarRadians(float newOrientationRadians, float newLength)
{
	x = newLength * cosf(newOrientationRadians);
	y = newLength * sinf(newOrientationRadians);
}


void Vec2::Rotate90Degrees()
{
	float temp = x;
	x = -y;
	y = temp;
}


void Vec2::RotateMinus90Degrees()
{
	float temp = y;
	y = -x;
	x = temp;
}


void Vec2::RotateDegrees(float deltaDegrees)
{
	float theta = Atan2Degrees(y, x);
	float r = sqrtf((x * x) + (y * y));

	theta += deltaDegrees;

	x = r * CosDegrees(theta);
	y = r * SinDegrees(theta);
}


void Vec2::RotateRadians(float deltaRadians)
{
	float theta = atan2f(y, x);
	float r = sqrtf((x * x) + (y * y));

	theta += deltaRadians;

	x = r * cosf(theta);
	y = r * sinf(theta);
}


void Vec2::SetLength(float newLength)
{
	float theta = atan2f(y, x);
	float r = sqrtf((x * x) + (y * y));

	r = newLength;

	x = r * cosf(theta);
	y = r * sinf(theta);
}


void Vec2::ClampLength(float maxLength)
{
	float theta = atan2f(y, x);
	float r = sqrtf((x * x) + (y * y));

	if(r > maxLength) r = maxLength;

	x = r * cosf(theta);
	y = r * sinf(theta);
}


void Vec2::Normalize()
{
	float length = GetLength();

	if (length == 0.0f) return;

	x /= length;
	y /= length;
}


float Vec2::NormalizeAndGetPreviousLength()
{
	float theta = atan2f(y, x);
	float r = sqrtf((x * x) + (y * y));

	x = cosf(theta);
	y = sinf(theta);
	
	return r;
}


void Vec2::Reflect(Vec2 const& surfaceNormal)
{
	Vec2 projectedOnNormal = surfaceNormal * DotProduct2D(*this, surfaceNormal);

	Vec2 projectedOnSurface = *this - projectedOnNormal;

	*this = projectedOnSurface - projectedOnNormal;
}


void Vec2::SetFromText(char const* text)
{
	Strings splitText = SplitStringOnDelimiter(text, ',');

	//return without changing the vector if there isn't a valid number of entries
	if (splitText.size() != 2)
	{
		return;
	}

	//set x and y to the first and second string, respectively
	x = static_cast<float>(atof(splitText[0].c_str()));
	y = static_cast<float>(atof(splitText[1].c_str()));
}


//
//const operators
//
bool Vec2::operator==(Vec2 const& compare) const
{
	if (x == compare.x && y == compare.y) return true;
	else return false;
}


bool Vec2::operator!=(Vec2 const& compare) const
{
	if (x != compare.x || y != compare.y) return true;
	else return false;
}

Vec2 const Vec2::operator + (Vec2 const& vecToAdd) const
{
	return Vec2(x + vecToAdd.x, y + vecToAdd.y);
}


Vec2 const Vec2::operator-(Vec2 const& vecToSubtract) const
{
	return Vec2(x - vecToSubtract.x, y - vecToSubtract.y);
}


Vec2 const Vec2::operator-() const
{
	return Vec2(-x, -y);
}


Vec2 const Vec2::operator*(float uniformScale) const
{
	return Vec2(x * uniformScale, y * uniformScale);
}


Vec2 const Vec2::operator*(Vec2 const& vecToMultiply) const
{
	return Vec2(x * vecToMultiply.x, y * vecToMultiply.y);
}


Vec2 const Vec2::operator/(float inverseScale) const
{
	float scale = 1 / inverseScale;	//faster to only divide once, then multiply by result twice
	return Vec2( x * scale, y * scale );
}


//
//non-const operators
//
void Vec2::operator+=(Vec2 const& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}


void Vec2::operator-=(Vec2 const& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}


void Vec2::operator*=(float const uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
}


void Vec2::operator/=(float const uniformDivisor)
{
	float scale = 1 / uniformDivisor;
	x *= scale;
	y *= scale;
}


void Vec2::operator=(Vec2 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}


//
//standalone friend function
//
Vec2 const operator*(float uniformScale, Vec2 const& vecToScale)
{
	return Vec2(vecToScale.x * uniformScale, vecToScale.y * uniformScale);
}
