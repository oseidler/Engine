#pragma once
#include "Engine/Math/Vec2.hpp"

struct OBB2
{
//public member variables
public:
	Vec2 m_center;
	Vec2 m_iBasisNormal;
	Vec2 m_halfDimensions;

//public member functions
public:
	OBB2() = default;
	explicit OBB2(Vec2 const& center, Vec2 const& iBasisNormal, Vec2 const halfDimensions);
};
