#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"


//
//constructor
//
EulerAngles::EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees)
	: m_yawDegrees(yawDegrees)
	, m_pitchDegrees(pitchDegrees)
	, m_rollDegrees(rollDegrees)
{
}


//
//accessors
//
void EulerAngles::GetAsVectors_XFwd_YLeft_ZUp(Vec3& out_forwardIBasis, Vec3& out_leftJBasis, Vec3& out_upKBasis) const
{
	float cy = CosDegrees(m_yawDegrees);
	float sy = SinDegrees(m_yawDegrees);
	float cp = CosDegrees(m_pitchDegrees);
	float sp = SinDegrees(m_pitchDegrees);
	float cr = CosDegrees(m_rollDegrees);
	float sr = SinDegrees(m_rollDegrees);

	out_forwardIBasis.x = cy * cp;
	out_forwardIBasis.y = sy * cp;
	out_forwardIBasis.z = -sp;

	out_leftJBasis.x = (cy * sp * sr) - (sy * cr);
	out_leftJBasis.y = (sy * sp * sr) + (cy * cr);
	out_leftJBasis.z = cp * sr;

	out_upKBasis.x = (cy * sp * cr) + (sy * sr);
	out_upKBasis.y = (sy * sp * cr) - (cy * sr);
	out_upKBasis.z = cp * cr;
}


Mat44 EulerAngles::GetAsMatrix_XFwd_YLeft_ZUp() const
{
	Mat44 eulerMatrix;

	float cy = CosDegrees(m_yawDegrees);
	float sy = SinDegrees(m_yawDegrees);
	float cp = CosDegrees(m_pitchDegrees);
	float sp = SinDegrees(m_pitchDegrees);
	float cr = CosDegrees(m_rollDegrees);
	float sr = SinDegrees(m_rollDegrees);

	eulerMatrix.m_values[0] = cy * cp;
	eulerMatrix.m_values[1] = sy * cp;
	eulerMatrix.m_values[2] = -sp;

	eulerMatrix.m_values[4] = (cy * sp * sr) - (sy * cr);
	eulerMatrix.m_values[5] = (sy * sp * sr) + (cy * cr);
	eulerMatrix.m_values[6] = cp * sr;

	eulerMatrix.m_values[8] = (cy * sp * cr) + (sy * sr);
	eulerMatrix.m_values[9] = (sy * sp * cr) - (cy * sr);
	eulerMatrix.m_values[10] = cp * cr;

	return eulerMatrix;
}


//
//mutators
//
void EulerAngles::SetFromText(char const* text)
{
	Strings splitText = SplitStringOnDelimiter(text, ',');

	//return without changing the vector if there isn't a valid number of entries
	if (splitText.size() != 3)
	{
		return;
	}

	//set x and y to the first and second string, respectively
	m_yawDegrees = static_cast<float>(atof(splitText[0].c_str()));
	m_pitchDegrees = static_cast<float>(atof(splitText[1].c_str()));
	m_rollDegrees = static_cast<float>(atof(splitText[2].c_str()));
}


//
//const operators
//
EulerAngles const EulerAngles::operator*(float uniformScale) const
{
	return EulerAngles(m_yawDegrees * uniformScale, m_pitchDegrees * uniformScale, m_rollDegrees * uniformScale);
}


//
//non-const operators
//
void EulerAngles::operator+=(EulerAngles const anglesToAdd)
{
	m_yawDegrees += anglesToAdd.m_yawDegrees;
	m_pitchDegrees += anglesToAdd.m_pitchDegrees;
	m_rollDegrees += anglesToAdd.m_rollDegrees;
}
