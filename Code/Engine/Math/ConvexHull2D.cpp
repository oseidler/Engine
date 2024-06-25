#include "Engine/Math/ConvexHull2D.hpp"
#include "Engine/Math/ConvexPoly2D.hpp"
#include "Engine/Math/MathUtils.hpp"


//
//constructor
//
ConvexHull2D::ConvexHull2D(ConvexPoly2D convexPoly)
{
	for (int pointIndex = 0; pointIndex < convexPoly.GetNumberOfPoints(); pointIndex++)
	{
		Vec2 pointA = convexPoly.GetPoint(pointIndex);
		Vec2 pointB;
		if (pointIndex == convexPoly.GetNumberOfPoints() - 1)
		{
			pointB = convexPoly.GetPoint(0);
		}
		else
		{
			pointB = convexPoly.GetPoint(pointIndex + 1);
		}
		
		Vec2 aToB = (pointB - pointA).GetNormalized();
		Vec2 normal = aToB.GetRotatedMinus90Degrees();
		float distFromOrigin = DotProduct2D(pointA, normal);
		Plane2D newPlane = Plane2D(normal, distFromOrigin);

		m_boundingPlanes.emplace_back(newPlane);
	}
}


ConvexHull2D::ConvexHull2D(std::vector<Plane2D> planes)
	: m_boundingPlanes(planes)
{
}
