#pragma once
#include "Engine/Math/Vec2.hpp"


class AnalogJoystick
{
//public member functions
public:
	//accessors
	Vec2  GetPosition()	const;
	float GetMagnitude() const;
	float GetOrientationDegrees() const;
	Vec2  GetRawPosition() const;
	float GetInnerDeadZoneFraction() const;
	float GetOuterDeadZoneFraction() const;

	//mutators
	void Reset();
	void SetDeadZoneThresholds(float normalInnerDeadZoneThreshold, float normalOuterDeadZoneThreshold);
	void UpdatePosition(float rawNormalX, float rawNormalY);

//protected member variables
protected:
	Vec2  m_rawPosition;
	Vec2  m_correctedPosition;
	float m_innerDeadZoneFraction = 0.2f;
	float m_outerDeadZoneFraction = 1.f;
};
