#pragma once
#include "Engine/Math/Vec3.hpp"


struct Plane3D
{
//public member variables
public:
	Vec3  m_normal = Vec3();
	float m_distFromOrigin = 0.0f;

//public member functions
public:
	Plane3D() {}
	Plane3D(Vec3 normal, float distFromOrigin)
		: m_normal(normal), m_distFromOrigin(distFromOrigin) {}
};
