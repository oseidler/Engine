#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Core/Vertex_PNCU.hpp"


//
//transform utilities
//
void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* vertArray, float scale, float rotationDegrees, Vec2 const& translation)
{
	for (int vertIndex = 0; vertIndex < numVerts; vertIndex++)
	{
		TransformPositionXY3D(vertArray[vertIndex].m_position, scale, rotationDegrees, translation);
	}
}


void TransformVertexArray3D(std::vector<Vertex_PCU>& verts, Mat44 const& transform)
{
	for (int vertIndex = 0; vertIndex < verts.size(); vertIndex++)
	{
		verts[vertIndex].m_position = transform.TransformPosition3D(verts[vertIndex].m_position);
	}
}

void TransformVertexArray3D(std::vector<Vertex_PNCU>& verts, Mat44 const& transform)
{
	for (int vertIndex = 0; vertIndex < verts.size(); vertIndex++)
	{
		verts[vertIndex].m_position = transform.TransformPosition3D(verts[vertIndex].m_position);
		verts[vertIndex].m_normal = transform.TransformVectorQuantity3D(verts[vertIndex].m_normal);
	}
}


//
//2D vertex adding functions
//
void AddVertsForAABB2(std::vector<Vertex_PCU>& verts, AABB2 const& aabb, Rgba8 const& color, AABB2 const& uvs)
{
	float uvs_minsX = uvs.m_mins.x;
	float uvs_minsY = uvs.m_mins.y;
	float uvs_maxsX = uvs.m_maxs.x;
	float uvs_maxsY = uvs.m_maxs.y;
	
	verts.push_back(Vertex_PCU(aabb.m_mins, color, Vec2(uvs_minsX, uvs_minsY)));
	verts.push_back(Vertex_PCU(Vec2(aabb.m_maxs.x, aabb.m_mins.y), color, Vec2(uvs_maxsX, uvs_minsY)));
	verts.push_back(Vertex_PCU(aabb.m_maxs, color, Vec2(uvs_maxsX, uvs_maxsY)));

	verts.push_back(Vertex_PCU(aabb.m_mins, color, Vec2(uvs_minsX, uvs_minsY)));
	verts.push_back(Vertex_PCU(aabb.m_maxs, color, Vec2(uvs_maxsX, uvs_maxsY)));
	verts.push_back(Vertex_PCU(Vec2(aabb.m_mins.x, aabb.m_maxs.y), color, Vec2(uvs_minsX, uvs_maxsY)));
}


void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& obb, Rgba8 const& color)
{
	Vec2 const& center = obb.m_center;
	Vec2 const& iBasisNormal = obb.m_iBasisNormal;
	
	Vec2 jBasisNormal = iBasisNormal.GetRotated90Degrees();
	
	Vec2 rightDisp = iBasisNormal * obb.m_halfDimensions.x;
	Vec2 upDisp = jBasisNormal * obb.m_halfDimensions.y;

	Vec2 TL = center - rightDisp + upDisp;
	Vec2 TR = center + rightDisp + upDisp;
	Vec2 BL = center - rightDisp - upDisp;
	Vec2 BR = center + rightDisp - upDisp;

	verts.push_back(Vertex_PCU(BL, color, Vec2(0.0f, 0.0f)));
	verts.push_back(Vertex_PCU(BR, color, Vec2(1.0f, 0.0f)));
	verts.push_back(Vertex_PCU(TR, color, Vec2(1.0f, 1.0f)));

	verts.push_back(Vertex_PCU(BL, color, Vec2(0.0f, 0.0f)));
	verts.push_back(Vertex_PCU(TR, color, Vec2(1.0f, 1.0f)));
	verts.push_back(Vertex_PCU(TL, color, Vec2(0.0f, 1.0f)));
}

 
void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color, int numEdges)
{
	float degreesPerSide = 360.0f / static_cast<float>(numEdges);
	
	for (int edgeIndex = 0; edgeIndex < numEdges; edgeIndex++)
	{
		float startDegrees = static_cast<float>(edgeIndex) * degreesPerSide;
		float endDegrees = static_cast<float>(edgeIndex + 1) * degreesPerSide;
		
		Vec2 edgeStart = center + Vec2::MakeFromPolarDegrees(startDegrees, radius);
		Vec2 edgeEnd = center + Vec2::MakeFromPolarDegrees(endDegrees, radius);
		
		verts.push_back(Vertex_PCU(center, color));
		verts.push_back(Vertex_PCU(edgeStart, color));
		verts.push_back(Vertex_PCU(edgeEnd, color));
	}
}


void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, Vec2 const& startPos, Vec2 const& endPos, float thickness, Rgba8 const& color)
{
	//get necessary vectors
	Vec2 displacement = endPos - startPos;
	Vec2 forward = displacement.GetNormalized();
	Vec2 forwardStep = forward * thickness;
	Vec2 leftStep = forwardStep.GetRotated90Degrees();

	//get points
	Vec2 startLeft = startPos - forwardStep + leftStep;
	Vec2 startRight = startPos - forwardStep - leftStep;
	Vec2 endLeft = endPos + forwardStep + leftStep;
	Vec2 endRight = endPos + forwardStep - leftStep;

	verts.push_back(Vertex_PCU(startLeft, color));
	verts.push_back(Vertex_PCU(startRight, color));
	verts.push_back(Vertex_PCU(endRight, color));

	verts.push_back(Vertex_PCU(startLeft, color));
	verts.push_back(Vertex_PCU(endRight, color));
	verts.push_back(Vertex_PCU(endLeft, color));
}


void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Vec2 const& boneStart, Vec2 const& boneEnd, float radius, Rgba8 const& color, int numArcEdges)
{
	//add verts for inner rectangle
	Vec2 displacement = boneEnd - boneStart;
	Vec2 forward = displacement.GetNormalized();
	Vec2 forwardStep = forward * radius;
	Vec2 leftStep = forwardStep.GetRotated90Degrees();

	//get points
	Vec2 startLeft = boneStart + leftStep;
	Vec2 startRight = boneStart - leftStep;
	Vec2 endLeft = boneEnd + leftStep;
	Vec2 endRight = boneEnd - leftStep;

	verts.push_back(Vertex_PCU(startLeft, color));
	verts.push_back(Vertex_PCU(startRight, color));
	verts.push_back(Vertex_PCU(endRight, color));

	verts.push_back(Vertex_PCU(startLeft, color));
	verts.push_back(Vertex_PCU(endRight, color));
	verts.push_back(Vertex_PCU(endLeft, color));

	//add verts for semicircles on sides
	float degreesPerSide = 180.0f / static_cast<float>(numArcEdges);

	//arc on start side
	displacement.Rotate90Degrees();
	float boneNormalOrientationDegrees = displacement.GetOrientationDegrees();

	for (int edgeIndex = 0; edgeIndex < numArcEdges; edgeIndex++)
	{
		float startDegrees = static_cast<float>(edgeIndex) * degreesPerSide + boneNormalOrientationDegrees;
		float endDegrees = static_cast<float>(edgeIndex + 1) * degreesPerSide + boneNormalOrientationDegrees;

		Vec2 edgeStart = boneStart + Vec2::MakeFromPolarDegrees(startDegrees, radius);
		Vec2 edgeEnd = boneStart + Vec2::MakeFromPolarDegrees(endDegrees, radius);

		verts.push_back(Vertex_PCU(boneStart, color));
		verts.push_back(Vertex_PCU(edgeStart, color));
		verts.push_back(Vertex_PCU(edgeEnd, color));
	}

	//arc on end side
	displacement = boneStart - boneEnd;
	displacement.Rotate90Degrees();
	boneNormalOrientationDegrees = displacement.GetOrientationDegrees();

	for (int edgeIndex = 0; edgeIndex < numArcEdges; edgeIndex++)
	{
		float startDegrees = static_cast<float>(edgeIndex) * degreesPerSide + boneNormalOrientationDegrees;
		float endDegrees = static_cast<float>(edgeIndex + 1) * degreesPerSide + boneNormalOrientationDegrees;

		Vec2 edgeStart = boneEnd + Vec2::MakeFromPolarDegrees(startDegrees, radius);
		Vec2 edgeEnd = boneEnd + Vec2::MakeFromPolarDegrees(endDegrees, radius);

		verts.push_back(Vertex_PCU(boneEnd, color));
		verts.push_back(Vertex_PCU(edgeStart, color));
		verts.push_back(Vertex_PCU(edgeEnd, color));
	}
}


void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, Vec2 const& startPos, Vec2 const& endPos, float tipSize, float thickness, Rgba8 const& color)
{
	//get necessary vectors
	Vec2 displacement = endPos - startPos;
	Vec2 forward = displacement.GetNormalized();
	Vec2 forwardStep = forward * thickness;
	Vec2 leftStep = forwardStep.GetRotated90Degrees();

	//get line points
	Vec2 startLeft = startPos - forwardStep + leftStep;
	Vec2 startRight = startPos - forwardStep - leftStep;
	Vec2 endLeft = endPos + forwardStep + leftStep;
	Vec2 endRight = endPos + forwardStep - leftStep;

	//draw line
	verts.push_back(Vertex_PCU(startLeft, color));
	verts.push_back(Vertex_PCU(startRight, color));
	verts.push_back(Vertex_PCU(endRight, color));

	verts.push_back(Vertex_PCU(startLeft, color));
	verts.push_back(Vertex_PCU(endRight, color));
	verts.push_back(Vertex_PCU(endLeft, color));

	//get tip points
	Vec2 tipStep = forward * tipSize;
	Vec2 tipLeft = endLeft + (tipStep.GetRotated90Degrees());
	Vec2 tipRight = endRight + (tipStep.GetRotatedMinus90Degrees());
	Vec2 tipForward = endPos + forwardStep + tipStep;


	//draw tip
	verts.push_back(Vertex_PCU(tipLeft, color));
	verts.push_back(Vertex_PCU(tipRight, color));
	verts.push_back(Vertex_PCU(tipForward, color));
}


//
//3D vertex adding functions
//
void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight, Rgba8 const& color, AABB2 const& uvs)
{
	Vec2 uvBL = Vec2(uvs.m_mins.x, uvs.m_mins.y);
	Vec2 uvBR = Vec2(uvs.m_maxs.x, uvs.m_mins.y);
	Vec2 uvTL = Vec2(uvs.m_mins.x, uvs.m_maxs.y);
	Vec2 uvTR = Vec2(uvs.m_maxs.x, uvs.m_maxs.y);

	verts.emplace_back(Vertex_PCU(bottomLeft, color, uvBL));
	verts.emplace_back(Vertex_PCU(bottomRight, color, uvBR));
	verts.emplace_back(Vertex_PCU(topRight, color, uvTR));

	verts.emplace_back(Vertex_PCU(bottomLeft, color, uvBL));
	verts.emplace_back(Vertex_PCU(topRight, color, uvTR));
	verts.emplace_back(Vertex_PCU(topLeft, color, uvTL));
}


void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight, Rgba8 const& color, AABB2 const& uvs)
{
	Vec2 uvBL = Vec2(uvs.m_mins.x, uvs.m_mins.y);
	Vec2 uvBR = Vec2(uvs.m_maxs.x, uvs.m_mins.y);
	Vec2 uvTL = Vec2(uvs.m_mins.x, uvs.m_maxs.y);
	Vec2 uvTR = Vec2(uvs.m_maxs.x, uvs.m_maxs.y);

	int nextIndex = static_cast<int>(verts.size());

	verts.push_back(Vertex_PCU(bottomLeft, color, uvBL));
	verts.push_back(Vertex_PCU(bottomRight, color, uvBR));
	verts.push_back(Vertex_PCU(topRight, color, uvTR));
	verts.push_back(Vertex_PCU(topLeft, color, uvTL));

	indexes.push_back(nextIndex);
	indexes.push_back(nextIndex + 1);
	indexes.push_back(nextIndex + 2);

	indexes.push_back(nextIndex);
	indexes.push_back(nextIndex + 2);
	indexes.push_back(nextIndex + 3);
}


void AddVertsForQuad3D(std::vector<Vertex_PNCU>& verts, std::vector<unsigned int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight, Rgba8 const& color /*= Rgba8(255, 255, 255)*/, AABB2 const& uvs /*= AABB2(0.0f, 0.0f, 1.0f, 1.0f)*/)
{
	Vec2 uvBL = Vec2(uvs.m_mins.x, uvs.m_mins.y);
	Vec2 uvBR = Vec2(uvs.m_maxs.x, uvs.m_mins.y);
	Vec2 uvTL = Vec2(uvs.m_mins.x, uvs.m_maxs.y);
	Vec2 uvTR = Vec2(uvs.m_maxs.x, uvs.m_maxs.y);

	Vec3 u = (bottomRight - bottomLeft).GetNormalized();
	Vec3 v = (topLeft - bottomLeft).GetNormalized();
	Vec3 normal = CrossProduct3D(u, v).GetNormalized();

	int nextIndex = static_cast<int>(verts.size());

	verts.push_back(Vertex_PNCU(bottomLeft, normal, color, uvBL));
	verts.push_back(Vertex_PNCU(bottomRight, normal, color, uvBR));
	verts.push_back(Vertex_PNCU(topRight, normal, color, uvTR));
	verts.push_back(Vertex_PNCU(topLeft, normal, color, uvTL));

	indexes.push_back(nextIndex);
	indexes.push_back(nextIndex + 1);
	indexes.push_back(nextIndex + 2);

	indexes.push_back(nextIndex);
	indexes.push_back(nextIndex + 2);
	indexes.push_back(nextIndex + 3);
}


void AddVertsForQuad3D(std::vector<Vertex_PNCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight, Rgba8 const& color /*= Rgba8(255, 255, 255)*/, AABB2 const& uvs /*= AABB2(0.0f, 0.0f, 1.0f, 1.0f)*/)
{
	Vec2 uvBL = Vec2(uvs.m_mins.x, uvs.m_mins.y);
	Vec2 uvBR = Vec2(uvs.m_maxs.x, uvs.m_mins.y);
	Vec2 uvTL = Vec2(uvs.m_mins.x, uvs.m_maxs.y);
	Vec2 uvTR = Vec2(uvs.m_maxs.x, uvs.m_maxs.y);

	Vec3 u = (bottomRight - bottomLeft).GetNormalized();
	Vec3 v = (topLeft - bottomLeft).GetNormalized();
	Vec3 normal = CrossProduct3D(u, v).GetNormalized();

	verts.push_back(Vertex_PNCU(bottomLeft, normal, color, uvBL));
	verts.push_back(Vertex_PNCU(bottomRight, normal, color, uvBR));
	verts.push_back(Vertex_PNCU(topRight, normal, color, uvTR));

	verts.push_back(Vertex_PNCU(bottomLeft, normal, color, uvBL));
	verts.push_back(Vertex_PNCU(topRight, normal, color, uvTR));
	verts.push_back(Vertex_PNCU(topLeft, normal, color, uvTL));
}


void AddVertsForRoundedQuad3D(std::vector<Vertex_PNCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight, Rgba8 const& color /*= Rgba8(255, 255, 255)*/, AABB2 const& uvs /*= AABB2(0.0f, 0.0f, 1.0f, 1.0f)*/)
{
	Vec2 uvBL = Vec2(uvs.m_mins.x, uvs.m_mins.y);
	Vec2 uvBM = Vec2(Interpolate(uvs.m_mins.x, uvs.m_maxs.x, 0.5f), uvs.m_mins.y);
	Vec2 uvBR = Vec2(uvs.m_maxs.x, uvs.m_mins.y);
	Vec2 uvTL = Vec2(uvs.m_mins.x, uvs.m_maxs.y);
	Vec2 uvTM = Vec2(Interpolate(uvs.m_mins.x, uvs.m_maxs.x, 0.5f), uvs.m_maxs.y);
	Vec2 uvTR = Vec2(uvs.m_maxs.x, uvs.m_maxs.y);

	Vec3 bottomMiddle = bottomLeft + (bottomRight - bottomLeft) * 0.5f;
	Vec3 topMiddle = topLeft + (topRight - topLeft) * 0.5f;

	Vec3 u = (bottomRight - bottomLeft).GetNormalized();
	Vec3 v = (topLeft - bottomLeft).GetNormalized();
	Vec3 normal = CrossProduct3D(u, v).GetNormalized();

	verts.push_back(Vertex_PNCU(bottomLeft, Vec3() - u, color, uvBL));
	verts.push_back(Vertex_PNCU(bottomMiddle, normal, color, uvBM));
	verts.push_back(Vertex_PNCU(topMiddle, normal, color, uvTM));

	verts.push_back(Vertex_PNCU(bottomLeft, Vec3() - u, color, uvBL));
	verts.push_back(Vertex_PNCU(topMiddle, normal, color, uvTM));
	verts.push_back(Vertex_PNCU(topLeft, Vec3() - u, color, uvTL));

	verts.push_back(Vertex_PNCU(bottomMiddle, normal, color, uvBM));
	verts.push_back(Vertex_PNCU(bottomRight, u, color, uvBR));
	verts.push_back(Vertex_PNCU(topRight, u, color, uvTR));

	verts.push_back(Vertex_PNCU(bottomMiddle, normal, color, uvBM));
	verts.push_back(Vertex_PNCU(topRight, u, color, uvTR));
	verts.push_back(Vertex_PNCU(topMiddle, normal, color, uvTM));
}


void AddVertsForCube3D(std::vector<Vertex_PCU>& verts, Vec3 const& forwardBottomLeft, Vec3 const& forwardBottomRight, Vec3 const& forwardTopLeft, Vec3 const& forwardTopRight, Vec3 const& backBottomLeft, Vec3 const& backBottomRight, Vec3 const& backTopLeft, Vec3 const& backTopRight, Rgba8 const& colorForward, Rgba8 const& colorBack, Rgba8 const& colorLeft, Rgba8 const& colorRight, Rgba8 const& colorBottom, Rgba8 const& colorTop, AABB2 const& uvs /*= AABB2(0.0f, 0.0f, 1.0f, 1.0f)*/)
{
	AddVertsForQuad3D(verts, forwardBottomRight, forwardBottomLeft, forwardTopRight, forwardTopLeft, colorForward, uvs); //+x (front face)
	AddVertsForQuad3D(verts, backBottomLeft, backBottomRight, backTopLeft, backTopRight, colorBack, uvs); //-x (back face)
	AddVertsForQuad3D(verts, forwardBottomLeft, backBottomLeft, forwardTopLeft, backTopLeft, colorLeft, uvs); //+y (left face)
	AddVertsForQuad3D(verts, backBottomRight, forwardBottomRight, backTopRight, forwardTopRight, colorRight, uvs); //-y (right face)
	AddVertsForQuad3D(verts, backTopLeft, backTopRight, forwardTopLeft, forwardTopRight, colorTop, uvs); //+z (top face)
	AddVertsForQuad3D(verts, forwardBottomLeft, forwardBottomRight, backBottomLeft, backBottomRight, colorBottom, uvs); //-z (bottom face)
}


void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& bounds, Rgba8 const& color, AABB2 const& uvs)
{
	Vec3 forwardBottomLeft = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 forwardBottomRight = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 forwardTopLeft = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 forwardTopRight = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_maxs.z);
	Vec3 backBottomLeft = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 backBottomRight = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 backTopLeft = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 backTopRight = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_maxs.z);

	AddVertsForCube3D(verts, forwardBottomLeft, forwardBottomRight, forwardTopLeft, forwardTopRight, backBottomLeft, backBottomRight, backTopLeft, backTopRight, 
		color, color, color, color, color, color, uvs);
}


void AddVertsForSphere3D(std::vector<Vertex_PCU>& verts, Vec3 const& center, float radius, int numSlices, int numStacks, Rgba8 const& color, AABB2 const& uvs)
{
	float degreesPerSlice = 360.0f / static_cast<float>(numSlices);
	float degreesPerStack = 180.0f / static_cast<float>(numStacks);

	for (int stackIndex = 0; stackIndex < numStacks; stackIndex++)
	{
		float topDegreesLat = -90.0f + (static_cast<float>(stackIndex) * degreesPerStack);
		float bottomDegreesLat = -90.0f + (static_cast<float>(stackIndex + 1) * degreesPerStack);
		
		for (int sliceIndex = 0; sliceIndex < numSlices; sliceIndex++)
		{
			float leftDegreesLong = static_cast<float>(sliceIndex) * degreesPerSlice;
			float rightDegreesLong = static_cast<float>(sliceIndex + 1) * degreesPerSlice;

			Vec3 bottomLeftCoords = Vec3::MakeFromPolarDegrees(bottomDegreesLat, leftDegreesLong, radius) + center;
			Vec3 bottomRightCoords = Vec3::MakeFromPolarDegrees(bottomDegreesLat, rightDegreesLong, radius) + center;
			Vec3 topLeftCoords = Vec3::MakeFromPolarDegrees(topDegreesLat, leftDegreesLong, radius) + center;
			Vec3 topRightCoords = Vec3::MakeFromPolarDegrees(topDegreesLat, rightDegreesLong, radius) + center;

			Vec2 bottomLeftUVs = Vec2(RangeMap(leftDegreesLong, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), RangeMap(bottomDegreesLat, -90.0f, 90.0f, uvs.m_mins.y, uvs.m_maxs.y));
			Vec2 bottomRightUVs = Vec2(RangeMap(rightDegreesLong, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), RangeMap(bottomDegreesLat, -90.0f, 90.0f, uvs.m_mins.y, uvs.m_maxs.y));
			Vec2 topLeftUVs = Vec2(RangeMap(leftDegreesLong, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), RangeMap(topDegreesLat, -90.0f, 90.0f, uvs.m_mins.y, uvs.m_maxs.y));
			Vec2 topRightUVs = Vec2(RangeMap(rightDegreesLong, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), RangeMap(topDegreesLat, -90.0f, 90.0f, uvs.m_mins.y, uvs.m_maxs.y));
			
			verts.push_back(Vertex_PCU(bottomLeftCoords, color, bottomLeftUVs));
			verts.push_back(Vertex_PCU(topRightCoords, color, topRightUVs));
			verts.push_back(Vertex_PCU(bottomRightCoords, color, bottomRightUVs));

			verts.push_back(Vertex_PCU(bottomLeftCoords, color, bottomLeftUVs));
			verts.push_back(Vertex_PCU(topLeftCoords, color, topLeftUVs));
			verts.push_back(Vertex_PCU(topRightCoords, color, topRightUVs));
		}
	}
}


void AddVertsForXYGrid3D(std::vector<Vertex_PCU>& verts, int numGridsX, int numGridsY, float gridSize, int sectionSize, float gridLineHalfWidth, float sectionLineHalfWidth, float originLineHalfWidth, Rgba8 const& gridLineColor, Rgba8 const& sectionLineXColor, Rgba8 const& sectionLineYColor, Rgba8 const& originLineXColor, Rgba8 const& originLineYColor)
{
	float xLength = static_cast<float>(numGridsX) * gridSize;
	float yLength = static_cast<float>(numGridsY) * gridSize;

	float xEndPosition = xLength * 0.5f;
	float yEndPosition = yLength * 0.5f;

	for (int xLineIndex = 0; xLineIndex < numGridsX; xLineIndex++)
	{
		float gridHeight = 0.005f;
		
		float xCurrentPosition = -xEndPosition + gridSize * static_cast<float>(xLineIndex);

		Rgba8 currentLineColor = gridLineColor;
		float currentLineHalfWidth = gridLineHalfWidth;

		if (xLineIndex % sectionSize == 0)
		{
			if (xLineIndex % (numGridsX / 2) == 0)
			{
				currentLineColor = originLineXColor;
				currentLineHalfWidth = originLineHalfWidth;
				gridHeight *= 2.5f;
			}
			else
			{
				currentLineColor = sectionLineXColor;
				currentLineHalfWidth = sectionLineHalfWidth;
				gridHeight *= 2.0f;
			}
		}
		
		Vec3 forwardBottomLeft = Vec3(xCurrentPosition - currentLineHalfWidth, yEndPosition, -gridHeight);
		Vec3 forwardBottomRight = Vec3(xCurrentPosition + currentLineHalfWidth, yEndPosition, -gridHeight);
		Vec3 forwardTopLeft = Vec3(xCurrentPosition - currentLineHalfWidth, yEndPosition, gridHeight);
		Vec3 forwardTopRight = Vec3(xCurrentPosition + currentLineHalfWidth, yEndPosition, gridHeight);
		Vec3 backBottomLeft = Vec3(xCurrentPosition - currentLineHalfWidth, -yEndPosition, -gridHeight);
		Vec3 backBottomRight = Vec3(xCurrentPosition + currentLineHalfWidth, -yEndPosition, -gridHeight);
		Vec3 backTopLeft = Vec3(xCurrentPosition - currentLineHalfWidth, -yEndPosition, gridHeight);
		Vec3 backTopRight = Vec3(xCurrentPosition + currentLineHalfWidth, -yEndPosition, gridHeight);

		AddVertsForCube3D(verts, forwardBottomLeft, forwardBottomRight, forwardTopLeft, forwardTopRight, backBottomLeft, backBottomRight, backTopLeft, backTopRight, currentLineColor, currentLineColor, currentLineColor, currentLineColor, currentLineColor, currentLineColor);
	}

	for (int yLineIndex = 0; yLineIndex < numGridsY; yLineIndex++)
	{
		float gridHeight = 0.005f;
		
		float yCurrentPosition = -yEndPosition + gridSize * static_cast<float>(yLineIndex);

		Rgba8 currentLineColor = gridLineColor;
		float currentLineHalfWidth = gridLineHalfWidth;

		if (yLineIndex % sectionSize == 0)
		{
			if (yLineIndex % (numGridsY / 2) == 0)
			{
				currentLineColor = originLineYColor;
				currentLineHalfWidth = originLineHalfWidth;
				gridHeight *= 2.5f;
			}
			else
			{
				currentLineColor = sectionLineYColor;
				currentLineHalfWidth = sectionLineHalfWidth;
				gridHeight *= 2.0f;
			}
		}

		Vec3 forwardBottomLeft = Vec3(-xEndPosition, yCurrentPosition - currentLineHalfWidth, -gridHeight);
		Vec3 forwardBottomRight = Vec3(-xEndPosition, yCurrentPosition + currentLineHalfWidth, -gridHeight);
		Vec3 forwardTopLeft = Vec3(-xEndPosition, yCurrentPosition - currentLineHalfWidth, gridHeight);
		Vec3 forwardTopRight = Vec3(-xEndPosition, yCurrentPosition + currentLineHalfWidth, gridHeight);
		Vec3 backBottomLeft = Vec3(xEndPosition, yCurrentPosition - currentLineHalfWidth, -gridHeight);
		Vec3 backBottomRight = Vec3(xEndPosition, yCurrentPosition + currentLineHalfWidth, -gridHeight);
		Vec3 backTopLeft = Vec3(xEndPosition, yCurrentPosition - currentLineHalfWidth, gridHeight);
		Vec3 backTopRight = Vec3(xEndPosition, yCurrentPosition + currentLineHalfWidth, gridHeight);

		AddVertsForCube3D(verts, forwardBottomLeft, forwardBottomRight, forwardTopLeft, forwardTopRight, backBottomLeft, backBottomRight, backTopLeft, backTopRight, currentLineColor, currentLineColor, currentLineColor, currentLineColor, currentLineColor, currentLineColor);
	}
}


void AddVertsForZCylinder3D(std::vector<Vertex_PCU>& verts, Vec3 const& baseCenter, float radius, float height, int numEdges, Rgba8 const& color, AABB2 const& uvs)
{
	float degreesPerSide = 360.0f / static_cast<float>(numEdges);

	Vec3 topCenter = Vec3(baseCenter.x, baseCenter.y, baseCenter.z + height);

	for (int edgeIndex = 0; edgeIndex < numEdges; edgeIndex++)
	{
		float startDegrees = static_cast<float>(edgeIndex) * degreesPerSide;
		float endDegrees = static_cast<float>(edgeIndex + 1) * degreesPerSide;

		//draw triangle at base
		Vec3 baseEdgeStart = baseCenter + Vec3::MakeFromPolarDegrees(0.0f, startDegrees, radius);
		Vec3 baseEdgeEnd = baseCenter + Vec3::MakeFromPolarDegrees(0.0f, endDegrees, radius);

		Vec2 edgeStartUVs = Vec2(RangeMap(CosDegrees(startDegrees), -1.0f, 1.0f, 0.0f, 1.0f), RangeMap(SinDegrees(startDegrees), -1.0f, 1.0f, 0.0f, 1.0f));
		Vec2 edgeEndUVs = Vec2(RangeMap(CosDegrees(endDegrees), -1.0f, 1.0f, 0.0f, 1.0f), RangeMap(SinDegrees(endDegrees), -1.0f, 1.0f, 0.0f, 1.0f));

		verts.push_back(Vertex_PCU(baseCenter, color, Vec2(0.5f, 0.5f)));
		verts.push_back(Vertex_PCU(baseEdgeEnd, color, Vec2(1.0f - edgeEndUVs.x, edgeEndUVs.y)));
		verts.push_back(Vertex_PCU(baseEdgeStart, color, Vec2(1.0f - edgeStartUVs.x, edgeStartUVs.y)));

		//draw triangle at top
		Vec3 topEdgeStart = topCenter + Vec3::MakeFromPolarDegrees(0.0f, startDegrees, radius);
		Vec3 topEdgeEnd = topCenter + Vec3::MakeFromPolarDegrees(0.0f, endDegrees, radius);

		verts.push_back(Vertex_PCU(topCenter, color, Vec2(0.5f, 0.5f)));
		verts.push_back(Vertex_PCU(topEdgeStart, color, edgeStartUVs));
		verts.push_back(Vertex_PCU(topEdgeEnd, color, edgeEndUVs));

		//draw side quad
		Vec2 bottomLeftUVs = Vec2(RangeMap(startDegrees, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), uvs.m_mins.y);
		Vec2 topRightUVs = Vec2(RangeMap(endDegrees, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), uvs.m_maxs.y);

		AddVertsForQuad3D(verts, baseEdgeStart, baseEdgeEnd, topEdgeStart, topEdgeEnd, color, AABB2(bottomLeftUVs, topRightUVs));
	}
}


void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, int numEdges, Rgba8 const& color, AABB2 const& uvs)
{
	std::vector<Vertex_PCU> cylinderVerts;

	float height = GetDistance3D(start, end);

	AddVertsForZCylinder3D(cylinderVerts, Vec3(), radius, height, numEdges, color, uvs);

	if ((end - start).GetNormalized() != Vec3(0.0f, 0.0f, 1.0f))
	{
		Mat44 xAxisAlign = Mat44::CreateYRotationDegrees(90.0f);

		TransformVertexArray3D(cylinderVerts, xAxisAlign);

		Vec3 iBasis = (end - start).GetNormalized();
		Vec3 jBasis = CrossProduct3D(Vec3(0.0f, 0.0f, 1.0f), iBasis).GetNormalized();
		Vec3 kBasis = CrossProduct3D(iBasis, jBasis).GetNormalized();

		Mat44 rotation = Mat44(iBasis, jBasis, kBasis, Vec3(0.0f, 0.0f, 0.0f));

		TransformVertexArray3D(cylinderVerts, rotation);
	}

	Mat44 translation = Mat44::CreateTranslation3D(start);

	TransformVertexArray3D(cylinderVerts, translation);

	verts.insert(verts.begin(), cylinderVerts.begin(), cylinderVerts.end());
}


void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, int numEdges, Rgba8 const& color, AABB2 const& uvs)
{
	std::vector<Vertex_PCU> coneVerts;

	float degreesPerSide = 360.0f / static_cast<float>(numEdges);

	float height = GetDistance3D(start, end);

	Vec3 topCenter = Vec3(0.0f, 0.0f, height);

	for (int edgeIndex = 0; edgeIndex < numEdges; edgeIndex++)
	{
		float startDegrees = static_cast<float>(edgeIndex) * degreesPerSide;
		float endDegrees = static_cast<float>(edgeIndex + 1) * degreesPerSide;

		//draw triangle at base
		Vec3 baseEdgeStart = Vec3::MakeFromPolarDegrees(0.0f, startDegrees, radius);
		Vec3 baseEdgeEnd = Vec3::MakeFromPolarDegrees(0.0f, endDegrees, radius);

		Vec2 edgeStartUVs = Vec2(RangeMap(CosDegrees(startDegrees), -1.0f, 1.0f, 0.0f, 1.0f), RangeMap(SinDegrees(startDegrees), -1.0f, 1.0f, 0.0f, 1.0f));
		Vec2 edgeEndUVs = Vec2(RangeMap(CosDegrees(endDegrees), -1.0f, 1.0f, 0.0f, 1.0f), RangeMap(SinDegrees(endDegrees), -1.0f, 1.0f, 0.0f, 1.0f));

		coneVerts.push_back(Vertex_PCU(Vec3(), color, Vec2(0.5f, 0.5f)));
		coneVerts.push_back(Vertex_PCU(baseEdgeEnd, color, Vec2(1.0f - edgeEndUVs.x, edgeEndUVs.y)));
		coneVerts.push_back(Vertex_PCU(baseEdgeStart, color, Vec2(1.0f - edgeStartUVs.x, edgeStartUVs.y)));

		Vec2 bottomLeftUVs = Vec2(RangeMap(startDegrees, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), uvs.m_mins.y);
		Vec2 bottomRightUVs = Vec2(RangeMap(endDegrees, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), uvs.m_mins.y);
		Vec2 topUVs = Vec2(RangeMap(endDegrees, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), uvs.m_maxs.y);

		//draw triangle from side to top
		coneVerts.push_back(Vertex_PCU(baseEdgeStart, color, bottomLeftUVs));
		coneVerts.push_back(Vertex_PCU(baseEdgeEnd, color, bottomRightUVs));
		coneVerts.push_back(Vertex_PCU(topCenter, color, topUVs));
	}

	if ((end - start).GetNormalized() != Vec3(0.0f, 0.0f, 1.0f))
	{
		Mat44 xAxisAlign = Mat44::CreateYRotationDegrees(90.0f);

		TransformVertexArray3D(coneVerts, xAxisAlign);

		Vec3 iBasis = (end - start).GetNormalized();
		Vec3 jBasis = CrossProduct3D(Vec3(0.0f, 0.0f, 1.0f), iBasis).GetNormalized();
		Vec3 kBasis = CrossProduct3D(iBasis, jBasis).GetNormalized();

		Mat44 rotation = Mat44(iBasis, jBasis, kBasis, Vec3(0.0f, 0.0f, 0.0f));

		TransformVertexArray3D(coneVerts, rotation);
	}

	Mat44 translation = Mat44::CreateTranslation3D(start);

	TransformVertexArray3D(coneVerts, translation);

	verts.insert(verts.begin(), coneVerts.begin(), coneVerts.end());
}


AABB2 GetVertexBounds2D(std::vector<Vertex_PCU> const& verts)
{
	//INCOMPLETE

	UNUSED(verts);

	return AABB2();
}
