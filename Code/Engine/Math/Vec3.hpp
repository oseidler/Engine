#pragma once

struct Vec2;
struct Quaternion;

struct Vec3
{
//public member variables
public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

//public member functions
public:
	//constructors and destructor
	Vec3() {}
	~Vec3() {}
	Vec3(float initialX, float initialY, float initialZ);
	Vec3(Vec2 const& vecXY, float initialZ);

	//creation functions
	static Vec3 const MakeFromPolarDegrees(float latitudeDegrees, float longitudeDegrees, float length = 1.0f);
	static Vec3 const MakeFromPolarRadians(float latitudeRadians, float longitudeRadians, float length = 1.0f);

	//accessors
	float GetLength() const;
	float GetLengthXY() const;
	float GetLengthSquared() const;
	float GetLengthXYSquared() const;
	float GetAngleAboutZDegrees() const;
	float GetAngleAboutZRadians() const;
	Vec3 const GetRotatedAboutZDegrees(float deltaDegrees) const;
	Vec3 const GetRotatedAboutZRadians(float deltaRadians) const;
	Vec3 const GetNormalized() const;
	Vec3 const GetRotatedAroundAxisByAngle(Vec3 const& axis, float deltaDegrees) const;

	//mutators
	void Normalize();
	void SetFromText(char const* text);
	void SetLength(float length);

	//const operators
	bool operator==(Vec3 const& compare) const;
	bool operator!=(Vec3 const& compare) const;
	Vec3 const operator+(Vec3 const& vecToAdd) const;
	Vec3 const operator-(Vec3 const& vecToSubtract) const;
	Vec3 const operator-() const;
	Vec3 const operator*(float uniformScale) const;
	Vec3 const operator/(float inverseScale) const;
	
	//non-const operators
	void operator+=(Vec3 const& vecToAdd);
	void operator-=(Vec3 const& vecToSubtract);
	void operator*=(float uniformScale);
	void operator/=(float uniformDivisor);
	void operator=(Vec3 const& copyFrom);

	//standalone friend function
	friend Vec3 const operator*(float uniformScale, Vec3 const& vecToScale);
};
