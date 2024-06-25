#pragma once
#include "Engine/Math/CubicBezierCurve2D.hpp"
#include "Engine/Core/EngineCommon.hpp"


class CatmullRomSpline
{
//public member functions
public:
	//constructors
	CatmullRomSpline() {}
	CatmullRomSpline(std::vector<Vec2> const& positions);

//public member variables
public:
	std::vector<CubicBezierCurve2D> m_curves;
};
