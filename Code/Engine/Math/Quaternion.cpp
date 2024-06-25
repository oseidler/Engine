#include "Engine/Math/Quaternion.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Mat44.hpp"
#include <math.h>


//
//
//
Mat44 const Quaternion::GetAsRotMatrix() const
{
	Mat44 rotMatrix = Mat44();

	rotMatrix.m_values[0] = 1 - (2 * v.y * v.y) - (2 * v.z * v.z);
	rotMatrix.m_values[1] = (2 * v.x * v.y) + (2 * v.z * s);
	rotMatrix.m_values[2] = (2 * v.x * v.z) - (2 * v.y * s);

	rotMatrix.m_values[4] = (2 * v.x * v.y) - (2 * v.z * s);
	rotMatrix.m_values[5] = 1 - (2 * v.x * v.x) - (2 * v.z * v.z);
	rotMatrix.m_values[6] = (2 * v.y * v.z) + (2 * v.x * s);

	rotMatrix.m_values[8] = (2 * v.x * v.z) + (2 * v.y * s);
	rotMatrix.m_values[9] = (2 * v.y * v.z) - (2 * v.x * s);
	rotMatrix.m_values[10] = 1 - (2 * v.x * v.x) - (2 * v.y * v.y);

	return rotMatrix;
}


//
//quaternion utilities
//
float Quaternion::GetLength() const
{
	return sqrtf((s * s) + (v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}


void Quaternion::Normalize()
{
	float length = GetLength();

	if (length != 0)
	{
		float inverseLength = 1 / length;

		v *= inverseLength;
		s *= inverseLength;
	}
}


void Quaternion::ConvertToRotationQuat()
{
	v.Normalize();
	s = CosDegrees(s * 0.5f);
	v = v * SinDegrees(s * 0.5f);
}


Quaternion const Quaternion::GetConjugate()
{
	return Quaternion(Vec3() - v, s);
}


Quaternion const Quaternion::GetInverse()
{
	float length = GetLength();
	float inverseLengthSquared = 1 / (length * length);

	Quaternion conjugate = GetConjugate();

	return conjugate * inverseLengthSquared;
}


//
//const operators
//
bool Quaternion::operator==(Quaternion const& compare) const
{
	return (v == compare.v && s == compare.s);
}


bool Quaternion::operator!=(Quaternion const& compare) const
{
	return (v != compare.v || s != compare.s);
}


Quaternion const Quaternion::operator+(Quaternion const& toAdd) const
{
	return Quaternion(v + toAdd.v, s + toAdd.s);
}


Quaternion const Quaternion::operator-(Quaternion const& toSubtract) const
{
	return Quaternion(v - toSubtract.v, s - toSubtract.s);
}


Quaternion const Quaternion::operator-() const
{
	return Quaternion(-v, -s);
}


Quaternion const Quaternion::operator*(Quaternion const& toMultiply) const
{
	float newS = (s * toMultiply.s) - (DotProduct3D(v, toMultiply.v));

	Vec3 newV = (v * toMultiply.s) + (toMultiply.v * s) + CrossProduct3D(v, toMultiply.v);

	return Quaternion(newV, newS);
}


Quaternion const Quaternion::operator*(float toMultiply) const
{
	return Quaternion(v * toMultiply, s * toMultiply);
}


//
//non-const operators
//
void Quaternion::operator=(Quaternion const& copyFrom)
{
	v = copyFrom.v;
	s = copyFrom.s;
}


void Quaternion::operator+=(Quaternion const& toAdd)
{
	v += toAdd.v;
	s += toAdd.s;
}


void Quaternion::operator-=(Quaternion const& toSubtract)
{
	v -= toSubtract.v;
	s -= toSubtract.s;
}


void Quaternion::operator*=(Quaternion const& toMultiply)
{
	float newS = (s * toMultiply.s) - (DotProduct3D(v, toMultiply.v));

	Vec3 newV = (v * toMultiply.s) + (toMultiply.v * s) + CrossProduct3D(v, toMultiply.v);

	v = newV;
	s = newS;
}


void Quaternion::operator*=(float toMultiply)
{
	v *= toMultiply;
	s *= toMultiply;
}
