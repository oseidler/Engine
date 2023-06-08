#include "Engine/Input/AnalogJoystick.hpp"
#include "Engine/Math/MathUtils.hpp"


//
//accessors
//
Vec2 AnalogJoystick::GetPosition() const
{
	return m_correctedPosition;
}


float AnalogJoystick::GetMagnitude() const
{
	return m_correctedPosition.GetLength();
}


float AnalogJoystick::GetOrientationDegrees() const
{
	return m_correctedPosition.GetOrientationDegrees();
}


Vec2 AnalogJoystick::GetRawPosition() const
{
	return m_rawPosition;
}


float AnalogJoystick::GetInnerDeadZoneFraction() const
{
	return m_innerDeadZoneFraction;
}


float AnalogJoystick::GetOuterDeadZoneFraction() const
{
	return m_outerDeadZoneFraction;
}


//
//mutators
//
void AnalogJoystick::Reset()
{
	m_rawPosition = Vec2(0.f, 0.f);
	m_correctedPosition = Vec2(0.f, 0.f);
}


void AnalogJoystick::SetDeadZoneThresholds(float normalInnerDeadZoneThreshold, float normalOuterDeadZoneThreshold)
{
	m_innerDeadZoneFraction = normalInnerDeadZoneThreshold;
	m_outerDeadZoneFraction = normalOuterDeadZoneThreshold;
}


void AnalogJoystick::UpdatePosition(float rawNormalX, float rawNormalY)
{
	m_rawPosition = Vec2(rawNormalX, rawNormalY);
	
	//convert to polar
	float r = GetDistance2D(Vec2(0.f, 0.f), m_rawPosition);
	float theta = Atan2Degrees(rawNormalY, rawNormalX);

	//correct position through clamped range map
	float correctedR = RangeMapClamped(r, m_innerDeadZoneFraction, m_outerDeadZoneFraction, 0.f, 1.f);

	float correctedX = correctedR * CosDegrees(theta);
	float correctedY = correctedR * SinDegrees(theta);

	m_correctedPosition = Vec2(correctedX, correctedY);
}
