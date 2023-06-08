#include "Engine/Math/CubicBezierCurve2D.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"


//
//constructor
//
CubicBezierCurve2D::CubicBezierCurve2D(Vec2 const& controlPointA, Vec2 const& controlPointB, Vec2 const& controlPointC, Vec2 const& controlPointD)
	: A(controlPointA)
	, B(controlPointB)
	, C(controlPointC)
	, D(controlPointD)
{
}


//
//static function to create using hermite parameters
//
CubicBezierCurve2D CubicBezierCurve2D::CreateUsingHermite(Vec2 const& startPos, Vec2 const& startVel, Vec2 const& endPos, Vec2 const& endVel)
{
	Vec2 pointB = startPos + startVel * (1.0f / 3.0f);
	Vec2 pointC = endPos - endVel * (1.0f / 3.0f);

	return CubicBezierCurve2D(startPos, pointB, pointC, endPos);
}


//
//bezier curve utilities
//
Vec2 CubicBezierCurve2D::EvaluateAtParametric(float t) const
{
	float s = 1.0f - t;

	Vec2 AB = s * A + t * B;
	Vec2 BC = s * B + t * C;
	Vec2 CD = s * C + t * D;

	Vec2 ABC = s * AB + t * BC;
	Vec2 BCD = s * BC + t * CD;

	return s * ABC + t * BCD;
}


float CubicBezierCurve2D::GetApproximateLength(int numSubdivisions) const
{
	float segmentStartT = 0.0f;
	Vec2 segmentStart = A;
	float length = 0.0f;

	for (int subdivIndex = 0; subdivIndex < numSubdivisions; subdivIndex++)
	{
		float segmentEndT = static_cast<float>(subdivIndex + 1) * (1.0f / numSubdivisions);
		Vec2 segmentEnd = EvaluateAtParametric(segmentEndT);
		
		length += GetDistance2D(segmentStart, segmentEnd);
		
		segmentStartT = segmentEndT;
		segmentStart = segmentEnd;
	}

	return length;
}


Vec2 CubicBezierCurve2D::EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdivisions) const
{
	float segmentStartT = 0.0f;
	Vec2 segmentStart = A;
	float currentLength = 0.0f;
	
	for (int subdivIndex = 0; subdivIndex < numSubdivisions; subdivIndex++)
	{
		float segmentEndT = static_cast<float>(subdivIndex + 1) * (1.0f / numSubdivisions);
		Vec2 segmentEnd = EvaluateAtParametric(segmentEndT);

		float lengthToAdd = GetDistance2D(segmentStart, segmentEnd);

		if (currentLength + lengthToAdd > distanceAlongCurve)
		{
			float subSegmentT = RangeMap(distanceAlongCurve, currentLength, currentLength + lengthToAdd, 0.0f, 1.0f);

			float newX = Interpolate(segmentStart.x, segmentEnd.x, subSegmentT);
			float newY = Interpolate(segmentStart.y, segmentEnd.y, subSegmentT);
			return Vec2(newX, newY);
		}

		currentLength += lengthToAdd;

		segmentStartT = segmentEndT;
		segmentStart = segmentEnd;
	}
	
	return Vec2(0.0f, 0.0f);
}


//
//hermite utilities
//
Vec2 CubicBezierCurve2D::GetHermiteStartVelocity() const
{
	return (B - A) * 3.0f;
}


Vec2 CubicBezierCurve2D::GetHermiteEndVelocity() const
{
	return (D - C) * 3.0f;
}
