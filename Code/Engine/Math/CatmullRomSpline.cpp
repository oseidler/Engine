#include "Engine/Math/CatmullRomSpline.hpp"


//
//constructor
//
CatmullRomSpline::CatmullRomSpline(std::vector<Vec2> const& positions)
{
	Vec2 velA = Vec2();

	for (int posIndex = 0; posIndex < positions.size() - 1; posIndex++)
	{
		Vec2 velB = Vec2();
		
		if (posIndex < positions.size() - 2)
		{
			velB = (positions[posIndex + 2] - positions[posIndex]) * 0.5f;
		}

		CubicBezierCurve2D curve = CubicBezierCurve2D::CreateUsingHermite(positions[posIndex], velA, positions[posIndex+1], velB);
		m_curves.push_back(curve);

		velA = velB;
	}
}
