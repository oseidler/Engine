#pragma once
#include "Engine/Math/Plane2D.hpp"
#include "Engine/Core/EngineCommon.hpp"


class ConvexPoly2D;


class ConvexHull2D
{
//public member functions
public:
	ConvexHull2D() {}
	ConvexHull2D(std::vector<Plane2D> planes);
	ConvexHull2D(ConvexPoly2D convexPoly);

//public member variables
public:
	std::vector<Plane2D> m_boundingPlanes;
};
