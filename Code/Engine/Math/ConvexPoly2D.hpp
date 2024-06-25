#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/Vec2.hpp"


class ConvexHull2D;


class ConvexPoly2D
{
//public member functions
public:
	ConvexPoly2D() {}
	ConvexPoly2D(std::vector<Vec2> ccwOrderedPoints);

	//accessors
	int GetNumberOfPoints() const { return static_cast<int>(m_ccwOrderedPoints.size()); }
	Vec2 GetPoint(int pointNumber) const;

	//mutators
	void Translate(Vec2 const& translation);
	void RotateAroundPivot(Vec2 const& pivot, float degreesCCW);
	void ScaleAroundPivot(Vec2 const& pivot, float scaleIncrease);

//private member variables
private:
	std::vector<Vec2> m_ccwOrderedPoints;
};
