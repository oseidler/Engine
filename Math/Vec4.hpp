#pragma once


struct Vec4
{
//public member variables
public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 0.f;

//public member functions
public:
	//constructor
	Vec4() = default;
	explicit Vec4(float initialX, float initialY, float initialZ, float initialW);

	//const operators
	bool operator==(Vec4 const& compare) const;
	bool operator!=(Vec4 const& compare) const;
	Vec4 const operator+(Vec4 const& vecToAdd) const;
	Vec4 const operator-(Vec4 const& vecToSubtract) const;
	Vec4 const operator*(float uniformScale) const;
	Vec4 const operator/(float inverseScale) const;

	//non-const operators
	void operator+=(Vec4 const& vecToAdd);
	void operator-=(Vec4 const& vecToSubtract);
	void operator*=(float uniformScale);
	void operator/=(float uniformDivisor);
	void operator=(Vec4 const& copyFrom);
};
