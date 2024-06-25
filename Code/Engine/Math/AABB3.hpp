#pragma once
#include "Engine/Math/Vec3.hpp"


struct AABB3
{
//public member variables
public:
	Vec3 m_mins;
	Vec3 m_maxs;

//public member functions
public:
	//constructors
	AABB3() {}
	~AABB3() {}
	AABB3(AABB3 const& copyFrom);
	explicit AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	explicit AABB3(Vec3 const& initialMins, Vec3 const& initialMaxs);

	//accessors
	Vec3 const GetCenter() const;
};
