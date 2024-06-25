#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"


//
//constructors
//
AABB2::AABB2(AABB2 const& copyFrom)
	: m_mins(copyFrom.m_mins)
	, m_maxs(copyFrom.m_maxs)
{
}


AABB2::AABB2(float minX, float minY, float maxX, float maxY)
	: m_mins(minX, minY)
	, m_maxs(maxX, maxY)
{
}


AABB2::AABB2(Vec2 const& initialMins, Vec2 const& initialMaxs)
	: m_mins(initialMins)
	, m_maxs(initialMaxs)
{
}


//
//accessors
//
bool AABB2::IsPointInside(Vec2 const& point) const
{
	if ((point.x > m_mins.x && point.x < m_maxs.x) && (point.y > m_mins.y && point.y < m_maxs.y))
	{
		return true;
	}
	else
	{
		return false;
	}
}


Vec2 const AABB2::GetCenter() const
{
	float centerX = (m_maxs.x + m_mins.x) * 0.5f;
	float centerY = (m_maxs.y + m_mins.y) * 0.5f;

	return Vec2(centerX, centerY);
}


Vec2 const AABB2::GetDimensions() const
{
	float dimX = m_maxs.x - m_mins.x;
	float dimY = m_maxs.y - m_mins.y;

	return Vec2(dimX, dimY);
}


Vec2 const AABB2::GetNearestPoint(Vec2 const& point) const
{
	float nearestX = GetClamped(point.x, m_mins.x, m_maxs.x);
	float nearestY = GetClamped(point.y, m_mins.y, m_maxs.y);
	
	return Vec2(nearestX, nearestY);
}


Vec2 const AABB2::GetPointAtUV(Vec2 const& uv) const
{
	float pointX = Interpolate(m_mins.x, m_maxs.x, uv.x);
	float pointY = Interpolate(m_mins.y, m_maxs.y, uv.y);
	
	return Vec2(pointX, pointY);
}


Vec2 const AABB2::GetUVForPoint(Vec2 const& point) const
{
	float u = GetFractionWithinRange(point.x, m_mins.x, m_maxs.x);
	float v = GetFractionWithinRange(point.y, m_mins.y, m_maxs.y);
	
	return Vec2(u, v);
}


AABB2 const AABB2::GetAABB2Within(Vec2 const& mins, Vec2 const& maxs) const
{
	Vec2 pointAtMins = GetPointAtUV(mins);
	Vec2 pointAtMaxs = GetPointAtUV(maxs);
	return AABB2(pointAtMins, pointAtMaxs);
}


//
//mutators
//
void AABB2::Translate(Vec2 const& translation)
{
	m_mins.x += translation.x;
	m_maxs.x += translation.x;
	m_mins.y += translation.y;
	m_maxs.y += translation.y;
}


void AABB2::SetCenter(Vec2 const& newCenter)
{
	Vec2 center = GetCenter();

	Vec2 translation = newCenter - center;

	Translate(translation);
}


void AABB2::SetDimensions(Vec2 const& newDimensions)
{
	float scaleX = newDimensions.x - (m_maxs.x - m_mins.x);
	float scaleY = newDimensions.y - (m_maxs.y - m_mins.y);
	
	m_mins.x -= scaleX * 0.5f;
	m_maxs.x += scaleX * 0.5f;
	m_mins.y -= scaleY * 0.5f;
	m_maxs.y += scaleY * 0.5f;
}


void AABB2::StretchToIncludePoint(Vec2 const& point)
{
	if (m_mins.x > point.x)
	{
		m_mins.x = point.x;
	}
	else if (m_maxs.x < point.x)
	{
		m_maxs.x = point.x;
	}
	
	if (m_mins.y > point.y)
	{
		m_mins.y = point.y;
	}
	else if (m_maxs.y < point.y)
	{
		m_maxs.y = point.y;
	}
}


void AABB2::SetFromText(char const* text)
{
	Strings splitText = SplitStringOnDelimiter(text, ',');

	//return without changing the vector if there isn't a valid number of entries
	if (splitText.size() != 4)
	{
		return;
	}

	//set x and y to the first and second string, respectively
	m_mins.x = static_cast<float>(atof(splitText[0].c_str()));
	m_mins.y = static_cast<float>(atof(splitText[1].c_str()));
	m_maxs.x = static_cast<float>(atof(splitText[2].c_str()));
	m_maxs.y = static_cast<float>(atof(splitText[3].c_str()));
}


void AABB2::operator=(AABB2 const& copyFrom)
{
	m_mins = copyFrom.m_mins;
	m_maxs = copyFrom.m_maxs;
}
