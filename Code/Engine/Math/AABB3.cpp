#include "Engine/Math/AABB3.hpp"


//
//constructors
//
AABB3::AABB3(AABB3 const& copyFrom)
	: m_mins(copyFrom.m_mins)
	, m_maxs(copyFrom.m_maxs)
{
}


AABB3::AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
	: m_mins(minX, minY, minZ)
	, m_maxs(maxX, maxY, maxZ)
{
}


AABB3::AABB3(Vec3 const& initialMins, Vec3 const& initialMaxs)
	: m_mins(initialMins)
	, m_maxs(initialMaxs)
{
}


//
//accessors
//
Vec3 const AABB3::GetCenter() const
{
	float centerX = (m_maxs.x + m_mins.x) * 0.5f;
	float centerY = (m_maxs.y + m_mins.y) * 0.5f;
	float centerZ = (m_maxs.z + m_mins.z) * 0.5f;

	return Vec3(centerX, centerY, centerZ);
}
