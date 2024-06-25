#pragma once
#include "Engine/Math/Vec3.hpp"


struct Mat44;


struct Quaternion
{
//public member variables
public:
	Vec3 v = Vec3();
	float s = 0;

//public member functions
public:
	Quaternion() = default;
	explicit Quaternion(Vec3 initialV, float initialS)
		: v(initialV), s(initialS) {}
	explicit Quaternion(float initialVx, float initialVy, float initialVz, float initialS)
		: v(Vec3(initialVx, initialVy, initialVz)), s(initialS) {}
	
	//conversion functions
	Mat44 const GetAsRotMatrix() const;

	//quaternion utilities
	float GetLength() const;
	void Normalize();
	void ConvertToRotationQuat();
	Quaternion const GetConjugate();
	Quaternion const GetInverse();

	//const operators
	bool operator==(Quaternion const& compare) const;
	bool operator!=(Quaternion const& compare) const;

	Quaternion const operator+(Quaternion const& toAdd) const;
	Quaternion const operator-(Quaternion const& toSubtract) const;
	Quaternion const operator-() const;
	Quaternion const operator*(Quaternion const& toMultiply) const;
	Quaternion const operator*(float toMultiply) const;

	//non-const operators
	void operator=(Quaternion const& copyFrom);
	void operator+=(Quaternion const& toAdd);
	void operator-=(Quaternion const& toSubtract);
	void operator*=(Quaternion const& toMultiply);
	void operator*=(float toMultiply);
};
