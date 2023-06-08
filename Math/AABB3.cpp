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
