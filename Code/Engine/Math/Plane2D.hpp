#pragma once
#include "Engine/Math/Vec2.hpp"


struct Plane2D
{
//public member variables
public:
	Vec2  m_normal = Vec2();
	float m_distFromOrigin = 0.0f;

//public member functions
public:
	Plane2D() {}
	Plane2D(Vec2 normal, float distFromOrigin)
		: m_normal(normal), m_distFromOrigin(distFromOrigin) {}
};
