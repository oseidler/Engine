#pragma once


//forward declaration
struct Vec3;
struct Mat44;


struct EulerAngles
{
//public member functions
public:
	//constructors
	EulerAngles() = default;
	EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees);

	//accessors
	void GetAsVectors_XFwd_YLeft_ZUp(Vec3& out_forwardIBasis, Vec3& out_leftJBasis, Vec3& out_upKBasis) const;
	Mat44 GetAsMatrix_XFwd_YLeft_ZUp() const;

	//mutators
	void SetFromText(char const* text);

	//const operators
	EulerAngles const operator*(float uniformScale) const;

	//nonconst operators
	void operator+=(EulerAngles const anglesToAdd);

//public member variables
public:
	float m_yawDegrees = 0.0f;
	float m_pitchDegrees = 0.0f;
	float m_rollDegrees = 0.0f;
};
