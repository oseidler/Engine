#include "Engine/Math/ConvexPoly2D.hpp"
#include "Engine/Math/ConvexHull2D.hpp"


//
//constructor
//
ConvexPoly2D::ConvexPoly2D(std::vector<Vec2> ccwOrderedPoints)
	: m_ccwOrderedPoints(ccwOrderedPoints)
{
}


//
//accessors
//
Vec2 ConvexPoly2D::GetPoint(int pointNumber) const
{
	if (pointNumber < 0 || pointNumber >= m_ccwOrderedPoints.size())
	{
		ERROR_AND_DIE("Point number requested is outside of range!");
	}

	return m_ccwOrderedPoints[pointNumber];
}


void ConvexPoly2D::Translate(Vec2 const& translation)
{
	for (int pointIndex = 0; pointIndex < m_ccwOrderedPoints.size(); pointIndex++)
	{
		m_ccwOrderedPoints[pointIndex] += translation;
	}
}


void ConvexPoly2D::RotateAroundPivot(Vec2 const& pivot, float degreesCCW)
{
	for (int pointIndex = 0; pointIndex < m_ccwOrderedPoints.size(); pointIndex++)
	{
		Vec2& point = m_ccwOrderedPoints[pointIndex];

		point -= pivot;
		point.RotateDegrees(degreesCCW);
		point += pivot;
	}
}


void ConvexPoly2D::ScaleAroundPivot(Vec2 const& pivot, float scaleIncrease)
{
	for (int pointIndex = 0; pointIndex < m_ccwOrderedPoints.size(); pointIndex++)
	{
		Vec2& point = m_ccwOrderedPoints[pointIndex];

		Vec2 dispToPivot = point - pivot;
		point += dispToPivot * scaleIncrease;
	}
}
