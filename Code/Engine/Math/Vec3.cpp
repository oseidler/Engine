#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Quaternion.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <math.h>


//
//constructors
//
Vec3::Vec3(float initialX, float initialY, float initialZ)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
{
}


Vec3::Vec3(Vec2 const& vecXY, float initialZ)
	: x(vecXY.x)
	, y(vecXY.y)
	, z(initialZ)
{
}


//
//static creation functions
//
Vec3 const Vec3::MakeFromPolarDegrees(float latitudeDegrees, float longitudeDegrees, float length)
{
	float cosPitch = CosDegrees(-latitudeDegrees);
	float sinPitch = SinDegrees(-latitudeDegrees);
	float cosYaw = CosDegrees(longitudeDegrees);
	float sinYaw = SinDegrees(longitudeDegrees);

	return Vec3(length * cosPitch * cosYaw, length * cosPitch * sinYaw, length * -sinPitch);
}


Vec3 const Vec3::MakeFromPolarRadians(float latitudeRadians, float longitudeRadians, float length)
{
	float cosPitch = cosf(-latitudeRadians);
	float sinPitch = sinf(-latitudeRadians);
	float cosYaw = cosf(longitudeRadians);
	float sinYaw = sinf(longitudeRadians);

	return Vec3(length * cosPitch * cosYaw, length * cosPitch * sinYaw, length * -sinPitch);
}


//
//accessors
//
float Vec3::GetLength() const
{
	return GetDistance3D(Vec3(0.f, 0.f, 0.f), Vec3(x, y, z));
}


float Vec3::GetLengthSquared() const
{
	return GetDistanceSquared3D(Vec3(0.f, 0.f, 0.f), Vec3(x, y, z));
}


float Vec3::GetLengthXY() const
{
	return GetDistanceXY3D(Vec3(0.f, 0.f, 0.f), Vec3(x, y, z));
}


float Vec3::GetLengthXYSquared() const
{
	return GetDistanceXYSquared3D(Vec3(0.f, 0.f, 0.f), Vec3(x, y, z));
}


float Vec3::GetAngleAboutZDegrees() const
{
	return Atan2Degrees(y, x);
}


float Vec3::GetAngleAboutZRadians() const
{
	return atan2f(y, x);
}


Vec3 const Vec3::GetRotatedAboutZDegrees(float deltaDegrees) const
{
	float theta = Atan2Degrees(y, x);
	float r = sqrtf((x * x) + (y * y));

	theta += deltaDegrees;
	
	return Vec3(r * CosDegrees(theta), r * SinDegrees(theta), z);
}


Vec3 const Vec3::GetRotatedAboutZRadians(float deltaRadians) const
{
	float theta = atan2f(y, x);
	float r = sqrtf((x * x) + (y * y));

	theta += deltaRadians;
	
	return Vec3(r * cosf(theta), r * sinf(theta), z);
}


Vec3 const Vec3::GetNormalized() const
{
	float length = GetLength();

	if (length == 0.0f) return *this;

	return *this / length;
}


Vec3 const Vec3::GetRotatedAroundAxisByAngle(Vec3 const& axis, float deltaDegrees) const
{
	Vec3 normalAxis = axis.GetNormalized();
	
	Quaternion pureQuat = Quaternion(*this, 0.0f);
	Quaternion realQuat = Quaternion(normalAxis, deltaDegrees);
	realQuat.ConvertToRotationQuat();

	Quaternion realQuatInverse = realQuat.GetInverse();

	Quaternion rotatedPureQuat = realQuat * pureQuat * realQuatInverse;

	return rotatedPureQuat.v;
}


//
//mutators
//
void Vec3::Normalize()
{
	float length = GetLength();

	if (length == 0.0f) return;

	x /= length;
	y /= length;
	z /= length;
}


void Vec3::SetFromText(char const* text)
{
	Strings splitText = SplitStringOnDelimiter(text, ',');

	//return without changing the vector if there isn't a valid number of entries
	if (splitText.size() != 3)
	{
		return;
	}

	//set x and y to the first and second string, respectively
	x = static_cast<float>(atof(splitText[0].c_str()));
	y = static_cast<float>(atof(splitText[1].c_str()));
	z = static_cast<float>(atof(splitText[2].c_str()));
}


void Vec3::SetLength(float length)
{
	Normalize();

	x *= length;
	y *= length;
	z *= length;
}


//
//const operators
//
bool Vec3::operator==(Vec3 const& compare) const
{
	if (x == compare.x && y == compare.y && z == compare.z) return true;
	else return false;
}


bool Vec3::operator!=(Vec3 const& compare) const
{
	if (x != compare.x || y != compare.y || z != compare.z) return true;
	else return false;
}


Vec3 const Vec3::operator+(Vec3 const& vecToAdd) const
{
	return Vec3(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z);
}


Vec3 const Vec3::operator-(Vec3 const& vecToSubtract) const
{
	return Vec3(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z);
}


Vec3 const Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}


Vec3 const Vec3::operator*(float uniformScale) const
{
	return Vec3(x * uniformScale, y * uniformScale, z * uniformScale);
}


Vec3 const Vec3::operator/(float inverseScale) const
{
	float scale = 1.f / inverseScale;
	return Vec3(x * scale, y * scale, z * scale);
}


//non-const operators
void Vec3::operator+=(Vec3 const& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}


void Vec3::operator-=(Vec3 const& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}


void Vec3::operator*=(float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}


void Vec3::operator/=(float uniformDivisor)
{
	float scale = 1.f / uniformDivisor;
	x *= scale;
	y *= scale;
	z *= scale;
}


void Vec3::operator=(Vec3 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}


//
//standalone friend function
//
Vec3 const operator*(float uniformScale, Vec3 const& vecToScale)
{
	return Vec3(vecToScale.x * uniformScale, vecToScale.y * uniformScale, vecToScale.z * uniformScale);
}
