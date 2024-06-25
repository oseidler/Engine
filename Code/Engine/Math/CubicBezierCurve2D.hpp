#pragma once
#include "Engine/Math/Vec2.hpp"


struct CubicBezierCurve2D
{
//public member functions
public:
	//constructor
	CubicBezierCurve2D() {}
	CubicBezierCurve2D(Vec2 const& controlPointA, Vec2 const& controlPointB, Vec2 const& controlPointC, Vec2 const& controlPointD);

	//static function to create using hermite parameters
	static CubicBezierCurve2D CreateUsingHermite(Vec2 const& startPos, Vec2 const& startVel, Vec2 const& endPos, Vec2 const& endVel);

	//bezier curve utilities
	Vec2 EvaluateAtParametric(float t) const;
	float GetApproximateLength(int numSubdivisions) const;
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdivisions) const;

	//hermite curve utilities
	Vec2 GetHermiteStartVelocity() const;
	Vec2 GetHermiteEndVelocity() const;

//public member variables
public:
	Vec2 A;
	Vec2 B;
	Vec2 C;
	Vec2 D;
};
