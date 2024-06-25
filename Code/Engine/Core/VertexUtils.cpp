#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Core/Vertex_PCUTBN.hpp"
#include "Engine/Math/ConvexPoly2D.hpp"


//
//calculation functions
//
void CalculateTangentSpaceVectors(std::vector<Vertex_PCUTBN>& verts, std::vector<int>& indexes)
{
	for (int currentIndex = 0; currentIndex < indexes.size(); currentIndex += 3)
	{
		//NOTE: this will crash when using an index buffer with a size not equal to a multiple of 3
		int index0 = indexes[currentIndex];
		int index1 = indexes[currentIndex + 1];
		int index2 = indexes[currentIndex + 2];
		Vertex_PCUTBN& vertex0 = verts[index0];
		Vertex_PCUTBN& vertex1 = verts[index1];
		Vertex_PCUTBN& vertex2 = verts[index2];
		Vec3 const& pos0 = vertex0.m_position;
		Vec3 const& pos1 = vertex1.m_position;
		Vec3 const& pos2 = vertex2.m_position;
		Vec2 const& uv0 = vertex0.m_uvTexCoords;
		Vec2 const& uv1 = vertex1.m_uvTexCoords;
		Vec2 const& uv2 = vertex2.m_uvTexCoords;

		Vec3 edge01 = pos1 - pos0;
		Vec3 edge02 = pos2 - pos0;
		float u01 = uv1.x - uv0.x;
		float u02 = uv2.x - uv0.x;
		float v01 = uv1.y - uv0.y;
		float v02 = uv2.y - uv0.y;

		float r = 1.0f / ((u01 * v02) - (u02 * v01));
		Vec3 tangent = ((edge01 * v02) - (edge02 * v01)) * r;
		Vec3 bitangent = ((edge02 * u01) - (edge01 * u02)) * r;

		vertex0.m_tangent += tangent;
		vertex0.m_bitangent += bitangent;
		vertex1.m_tangent += tangent;
		vertex1.m_bitangent += bitangent;
		vertex2.m_tangent += tangent;
		vertex2.m_bitangent += bitangent;
	}

	//orthonormalize
	for (int vertIndex = 0; vertIndex < verts.size(); vertIndex++)
	{
		Vertex_PCUTBN& vert = verts[vertIndex];
		vert.m_normal.Normalize();
		vert.m_tangent.Normalize();
		vert.m_bitangent.Normalize();

		float tDotN = DotProduct3D(vert.m_tangent, vert.m_normal);
		Vec3 tProjectedOnN = vert.m_normal * tDotN;
		vert.m_tangent -= tProjectedOnN;
		vert.m_tangent.Normalize();

		float bDotN = DotProduct3D(vert.m_bitangent, vert.m_normal);
		Vec3 bProjectedOnN = vert.m_normal * bDotN;
		vert.m_bitangent -= bProjectedOnN;
		float bDotT = DotProduct3D(vert.m_bitangent, vert.m_tangent);
		Vec3 bProjectedOnT = vert.m_tangent * bDotT;
		vert.m_bitangent -= bProjectedOnT;
		vert.m_bitangent.Normalize();
	}
}


void CalculateTangentSpaceVectorsPlusNormals(std::vector<Vertex_PCUTBN>& verts, std::vector<int>& indexes)
{
	for (int currentIndex = 0; currentIndex < indexes.size(); currentIndex += 3)
	{
		//NOTE: this will crash when using an index buffer with a size not equal to a multiple of 3
		int index0 = indexes[currentIndex];
		int index1 = indexes[currentIndex + 1];
		int index2 = indexes[currentIndex + 2];
		Vertex_PCUTBN& vertex0 = verts[index0];
		Vertex_PCUTBN& vertex1 = verts[index1];
		Vertex_PCUTBN& vertex2 = verts[index2];
		Vec3 const& pos0 = vertex0.m_position;
		Vec3 const& pos1 = vertex1.m_position;
		Vec3 const& pos2 = vertex2.m_position;
		Vec2 const& uv0 = vertex0.m_uvTexCoords;
		Vec2 const& uv1 = vertex1.m_uvTexCoords;
		Vec2 const& uv2 = vertex2.m_uvTexCoords;

		Vec3 edge01 = pos1 - pos0;
		Vec3 edge02 = pos2 - pos0;
		float u01 = uv1.x - uv0.x;
		float u02 = uv2.x - uv0.x;
		float v01 = uv1.y - uv0.y;
		float v02 = uv2.y - uv0.y;

		float r = 1.0f / ((u01 * v02) - (u02 * v01));
		Vec3 tangent = ((edge01 * v02) - (edge02 * v01)) * r;
		Vec3 bitangent = ((edge02 * u01) - (edge01 * u02)) * r;

		Vec3 normal = CrossProduct3D(edge01, edge02);

		vertex0.m_normal += normal;
		vertex0.m_tangent += tangent;
		vertex0.m_bitangent += bitangent;
		vertex1.m_normal += normal;
		vertex1.m_tangent += tangent;
		vertex1.m_bitangent += bitangent;
		vertex2.m_normal += normal;
		vertex2.m_tangent += tangent;
		vertex2.m_bitangent += bitangent;
	}

	//orthonormalize
	for (int vertIndex = 0; vertIndex < verts.size(); vertIndex++)
	{
		Vertex_PCUTBN& vert = verts[vertIndex];
		vert.m_normal.Normalize();
		vert.m_tangent.Normalize();
		vert.m_bitangent.Normalize();

		float tDotN = DotProduct3D(vert.m_tangent, vert.m_normal);
		Vec3 tProjectedOnN = vert.m_normal * tDotN;
		vert.m_tangent -= tProjectedOnN;
		vert.m_tangent.Normalize();

		float bDotN = DotProduct3D(vert.m_bitangent, vert.m_normal);
		Vec3 bProjectedOnN = vert.m_normal * bDotN;
		vert.m_bitangent -= bProjectedOnN;
		float bDotT = DotProduct3D(vert.m_bitangent, vert.m_tangent);
		Vec3 bProjectedOnT = vert.m_tangent * bDotT;
		vert.m_bitangent -= bProjectedOnT;
		vert.m_bitangent.Normalize();
	}
}


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

void TransformVertexArray3D(std::vector<Vertex_PCUTBN>& verts, Mat44 const& transform)
{
	for (int vertIndex = 0; vertIndex < verts.size(); vertIndex++)
	{
		verts[vertIndex].m_position = transform.TransformPosition3D(verts[vertIndex].m_position);
		verts[vertIndex].m_normal = transform.TransformVectorQuantity3D(verts[vertIndex].m_normal);
		verts[vertIndex].m_tangent = transform.TransformVectorQuantity3D(verts[vertIndex].m_tangent);
		verts[vertIndex].m_bitangent = transform.TransformVectorQuantity3D(verts[vertIndex].m_bitangent);
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


void AddVertsForConvexPoly2D(std::vector<Vertex_PCU>& verts, ConvexPoly2D const& convexPoly, Rgba8 const& color)
{
	for (int triIndex = 0; triIndex < convexPoly.GetNumberOfPoints() - 2; triIndex++)
	{
		verts.push_back(Vertex_PCU(convexPoly.GetPoint(0), color));
		verts.push_back(Vertex_PCU(convexPoly.GetPoint(triIndex + 1), color));
		verts.push_back(Vertex_PCU(convexPoly.GetPoint(triIndex + 2), color));
	}
}


void AddVertsForConvexPoly2D(std::vector<Vertex_PCU>& verts, std::vector<int>& indexes, ConvexPoly2D const& convexPoly, Rgba8 const& color)
{
	for (int pointIndex = 0; pointIndex < convexPoly.GetNumberOfPoints(); pointIndex++)
	{
		verts.push_back(Vertex_PCU(convexPoly.GetPoint(pointIndex), color));
	}

	for (int triIndex = 0; triIndex < convexPoly.GetNumberOfPoints() - 2; triIndex++)
	{
		indexes.push_back(0);
		indexes.push_back(triIndex + 1);
		indexes.push_back(triIndex + 2);
	}
}


void AddVertsForConvexPoly2D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, ConvexPoly2D const& convexPoly, Rgba8 const& color)
{
	for (int pointIndex = 0; pointIndex < convexPoly.GetNumberOfPoints(); pointIndex++)
	{
		verts.push_back(Vertex_PCU(convexPoly.GetPoint(pointIndex), color));
	}

	for (int triIndex = 0; triIndex < convexPoly.GetNumberOfPoints() - 2; triIndex++)
	{
		indexes.push_back(0);
		indexes.push_back(triIndex + 1);
		indexes.push_back(triIndex + 2);
	}
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


void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight, Rgba8 const& color /*= Rgba8(255, 255, 255)*/, AABB2 const& uvs /*= AABB2(0.0f, 0.0f, 1.0f, 1.0f)*/)
{
	Vec2 uvBL = Vec2(uvs.m_mins.x, uvs.m_mins.y);
	Vec2 uvBR = Vec2(uvs.m_maxs.x, uvs.m_mins.y);
	Vec2 uvTL = Vec2(uvs.m_mins.x, uvs.m_maxs.y);
	Vec2 uvTR = Vec2(uvs.m_maxs.x, uvs.m_maxs.y);

	Vec3 u = (bottomRight - bottomLeft).GetNormalized();
	Vec3 v = (topLeft - bottomLeft).GetNormalized();
	Vec3 normal = CrossProduct3D(u, v).GetNormalized();

	int nextIndex = static_cast<int>(verts.size());

	verts.push_back(Vertex_PCUTBN(bottomLeft, normal, color, uvBL));
	verts.push_back(Vertex_PCUTBN(bottomRight, normal, color, uvBR));
	verts.push_back(Vertex_PCUTBN(topRight, normal, color, uvTR));
	verts.push_back(Vertex_PCUTBN(topLeft, normal, color, uvTL));

	indexes.push_back(nextIndex);
	indexes.push_back(nextIndex + 1);
	indexes.push_back(nextIndex + 2);

	indexes.push_back(nextIndex);
	indexes.push_back(nextIndex + 2);
	indexes.push_back(nextIndex + 3);
}


void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight, Rgba8 const& color /*= Rgba8(255, 255, 255)*/, AABB2 const& uvs /*= AABB2(0.0f, 0.0f, 1.0f, 1.0f)*/)
{
	Vec2 uvBL = Vec2(uvs.m_mins.x, uvs.m_mins.y);
	Vec2 uvBR = Vec2(uvs.m_maxs.x, uvs.m_mins.y);
	Vec2 uvTL = Vec2(uvs.m_mins.x, uvs.m_maxs.y);
	Vec2 uvTR = Vec2(uvs.m_maxs.x, uvs.m_maxs.y);

	Vec3 u = (bottomRight - bottomLeft).GetNormalized();
	Vec3 v = (topLeft - bottomLeft).GetNormalized();
	Vec3 normal = CrossProduct3D(u, v).GetNormalized();

	verts.push_back(Vertex_PCUTBN(bottomLeft, normal, color, uvBL));
	verts.push_back(Vertex_PCUTBN(bottomRight, normal, color, uvBR));
	verts.push_back(Vertex_PCUTBN(topRight, normal, color, uvTR));

	verts.push_back(Vertex_PCUTBN(bottomLeft, normal, color, uvBL));
	verts.push_back(Vertex_PCUTBN(topRight, normal, color, uvTR));
	verts.push_back(Vertex_PCUTBN(topLeft, normal, color, uvTL));
}


void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight, Rgba8 const& color /*= Rgba8(255, 255, 255)*/, AABB2 const& uvs /*= AABB2(0.0f, 0.0f, 1.0f, 1.0f)*/)
{
	Vec2 uvBL = Vec2(uvs.m_mins.x, uvs.m_mins.y);
	Vec2 uvBR = Vec2(uvs.m_maxs.x, uvs.m_mins.y);
	Vec2 uvTL = Vec2(uvs.m_mins.x, uvs.m_maxs.y);
	Vec2 uvTR = Vec2(uvs.m_maxs.x, uvs.m_maxs.y);

	Vec3 u = (bottomRight - bottomLeft).GetNormalized();
	Vec3 v = (topLeft - bottomLeft).GetNormalized();
	Vec3 normal = CrossProduct3D(u, v).GetNormalized();

	int nextIndex = static_cast<int>(verts.size());

	verts.push_back(Vertex_PCUTBN(bottomLeft, normal, color, uvBL));
	verts.push_back(Vertex_PCUTBN(bottomRight, normal, color, uvBR));
	verts.push_back(Vertex_PCUTBN(topRight, normal, color, uvTR));
	verts.push_back(Vertex_PCUTBN(topLeft, normal, color, uvTL));

	indexes.push_back(nextIndex);
	indexes.push_back(nextIndex + 1);
	indexes.push_back(nextIndex + 2);

	indexes.push_back(nextIndex);
	indexes.push_back(nextIndex + 2);
	indexes.push_back(nextIndex + 3);
}


void AddVertsForRoundedQuad3D(std::vector<Vertex_PCUTBN>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight, Rgba8 const& color /*= Rgba8(255, 255, 255)*/, AABB2 const& uvs /*= AABB2(0.0f, 0.0f, 1.0f, 1.0f)*/)
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

	verts.push_back(Vertex_PCUTBN(bottomLeft, Vec3() - u, color, uvBL));
	verts.push_back(Vertex_PCUTBN(bottomMiddle, normal, color, uvBM));
	verts.push_back(Vertex_PCUTBN(topMiddle, normal, color, uvTM));

	verts.push_back(Vertex_PCUTBN(bottomLeft, Vec3() - u, color, uvBL));
	verts.push_back(Vertex_PCUTBN(topMiddle, normal, color, uvTM));
	verts.push_back(Vertex_PCUTBN(topLeft, Vec3() - u, color, uvTL));

	verts.push_back(Vertex_PCUTBN(bottomMiddle, normal, color, uvBM));
	verts.push_back(Vertex_PCUTBN(bottomRight, u, color, uvBR));
	verts.push_back(Vertex_PCUTBN(topRight, u, color, uvTR));

	verts.push_back(Vertex_PCUTBN(bottomMiddle, normal, color, uvBM));
	verts.push_back(Vertex_PCUTBN(topRight, u, color, uvTR));
	verts.push_back(Vertex_PCUTBN(topMiddle, normal, color, uvTM));
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


void AddVertsForCube3D(std::vector<Vertex_PCUTBN>& verts, std::vector<int>& indexes, Vec3 const& forwardBottomLeft, Vec3 const& forwardBottomRight, Vec3 const& forwardTopLeft, 
	Vec3 forwardTopRight, Vec3 const& backBottomLeft, Vec3 const& backBottomRight, Vec3 const& backTopLeft, Vec3 const& backTopRight, Rgba8 const& color, AABB2 const& uvs)
{
	AddVertsForQuad3D(verts, indexes, forwardBottomRight, forwardBottomLeft, forwardTopRight, forwardTopLeft, color, uvs); //+x (front face)
	AddVertsForQuad3D(verts, indexes, backBottomLeft, backBottomRight, backTopLeft, backTopRight, color, uvs); //-x (back face)
	AddVertsForQuad3D(verts, indexes, forwardBottomLeft, backBottomLeft, forwardTopLeft, backTopLeft, color, uvs); //+y (left face)
	AddVertsForQuad3D(verts, indexes, backBottomRight, forwardBottomRight, backTopRight, forwardTopRight, color, uvs); //-y (right face)
	AddVertsForQuad3D(verts, indexes, backTopLeft, backTopRight, forwardTopLeft, forwardTopRight, color, uvs); //+z (top face)
	AddVertsForQuad3D(verts, indexes, forwardBottomLeft, forwardBottomRight, backBottomLeft, backBottomRight, color, uvs); //-z (bottom face)

	CalculateTangentSpaceVectors(verts, indexes);
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


void AddVertsForSphere3D(std::vector<Vertex_PCUTBN>& verts, Vec3 const& center, float radius, int numSlices, int numStacks, Rgba8 const& color, AABB2 const& uvs)
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

			Vec3 bottomLeftNormal = Vec3::MakeFromPolarDegrees(bottomDegreesLat, leftDegreesLong);
			Vec3 bottomLeftCoords = bottomLeftNormal * radius + center;
			Vec3 bottomRightNormal = Vec3::MakeFromPolarDegrees(bottomDegreesLat, rightDegreesLong);
			Vec3 bottomRightCoords = bottomRightNormal * radius + center;
			Vec3 topLeftNormal = Vec3::MakeFromPolarDegrees(topDegreesLat, leftDegreesLong);
			Vec3 topLeftCoords = topLeftNormal * radius + center;
			Vec3 topRightNormal = Vec3::MakeFromPolarDegrees(topDegreesLat, rightDegreesLong);
			Vec3 topRightCoords = topRightNormal * radius + center;

			Vec2 bottomLeftUVs = Vec2(RangeMap(leftDegreesLong, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), RangeMap(bottomDegreesLat, -90.0f, 90.0f, uvs.m_mins.y, uvs.m_maxs.y));
			Vec2 bottomRightUVs = Vec2(RangeMap(rightDegreesLong, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), RangeMap(bottomDegreesLat, -90.0f, 90.0f, uvs.m_mins.y, uvs.m_maxs.y));
			Vec2 topLeftUVs = Vec2(RangeMap(leftDegreesLong, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), RangeMap(topDegreesLat, -90.0f, 90.0f, uvs.m_mins.y, uvs.m_maxs.y));
			Vec2 topRightUVs = Vec2(RangeMap(rightDegreesLong, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), RangeMap(topDegreesLat, -90.0f, 90.0f, uvs.m_mins.y, uvs.m_maxs.y));

			verts.push_back(Vertex_PCUTBN(bottomLeftCoords, bottomLeftNormal, color, bottomLeftUVs));
			verts.push_back(Vertex_PCUTBN(topRightCoords, topRightNormal, color, topRightUVs));
			verts.push_back(Vertex_PCUTBN(bottomRightCoords, bottomRightNormal, color, bottomRightUVs));

			verts.push_back(Vertex_PCUTBN(bottomLeftCoords, bottomLeftNormal, color, bottomLeftUVs));
			verts.push_back(Vertex_PCUTBN(topLeftCoords, topLeftNormal, color, topLeftUVs));
			verts.push_back(Vertex_PCUTBN(topRightCoords, topRightNormal, color, topRightUVs));
		}
	}
}


void AddVertsForSphere3D(std::vector<Vertex_PCUTBN>& verts, std::vector<int>& indexes, Vec3 const& center, float radius, int numSlices, int numStacks, Rgba8 const& color, 
	AABB2 const& uvs)
{
	//add one vertex at bottom pole
	verts.emplace_back(Vertex_PCUTBN(Vec3(0.0f, 0.0f, -radius), Vec3(0.0f, 0.0f, -radius - 1.0f), color, Vec2()));
	
	//add vertexes
	float degreesPerSlice = 360.0f / static_cast<float>(numSlices);
	float degreesPerStack = 180.0f / static_cast<float>(numStacks);

	for (int stackIndex = 1; stackIndex < numStacks - 1; stackIndex++)
	{
		float bottomDegreesLat = -90.0f + (static_cast<float>(stackIndex + 1) * degreesPerStack);

		for (int sliceIndex = 0; sliceIndex < numSlices; sliceIndex++)
		{
			float leftDegreesLong = static_cast<float>(sliceIndex) * degreesPerSlice;

			Vec3 bottomLeftNormal = Vec3::MakeFromPolarDegrees(bottomDegreesLat, leftDegreesLong);
			Vec3 bottomLeftCoords = bottomLeftNormal * radius + center;
			Vec2 bottomLeftUVs = Vec2(RangeMap(leftDegreesLong, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), RangeMap(bottomDegreesLat, -90.0f, 90.0f, uvs.m_mins.y, uvs.m_maxs.y));

			verts.emplace_back(Vertex_PCUTBN(bottomLeftCoords, bottomLeftNormal, color, bottomLeftUVs));
		}
	}

	//add one vertex at top pole
	verts.emplace_back(Vertex_PCUTBN(Vec3(0.0f, 0.0f, radius), Vec3(0.0f, 0.0f, radius + 1.0f), color, Vec2(1.0f, 1.0f)));

	//add indexes
	//int currentIndex = 0;
	for (int stackIndex = 0; stackIndex < numStacks; stackIndex++)
	{
		for (int sliceIndex = 0; sliceIndex < numSlices; sliceIndex++)
		{
			if (stackIndex == 0)
			{
				indexes.emplace_back(0);
				if (sliceIndex == numSlices - 1)
				{
					indexes.emplace_back(1);
					indexes.emplace_back(numSlices);
				}
				else
				{
					indexes.emplace_back(sliceIndex + 2);
					indexes.emplace_back(sliceIndex + 1);
				}
			}
			else if (stackIndex == numStacks - 1)
			{
				if (sliceIndex == numSlices - 1)
				{
					indexes.emplace_back(static_cast<int>(verts.size()) - 3);
					indexes.emplace_back(static_cast<int>(verts.size()) - 2);
				}
				else if (sliceIndex == 0)
				{
					indexes.emplace_back(static_cast<int>(verts.size()) - 2);
					indexes.emplace_back(static_cast<int>(verts.size()) - numSlices + sliceIndex - 1);
				}
				else
				{
					indexes.emplace_back(static_cast<int>(verts.size()) - numSlices + sliceIndex - 2);
					indexes.emplace_back(static_cast<int>(verts.size()) - numSlices + sliceIndex - 1);
				}
				indexes.emplace_back(static_cast<int>(verts.size()) - 1);
			}
			else if(stackIndex != numStacks - 2)
			{
				if (sliceIndex == numSlices - 1)
				{
					indexes.emplace_back((stackIndex - 1) * numSlices + sliceIndex + 1);
					indexes.emplace_back((stackIndex - 1) * numSlices + 1);
					indexes.emplace_back(stackIndex * numSlices + 2);
					indexes.emplace_back((stackIndex - 1) * numSlices + sliceIndex + 1);
					indexes.emplace_back(stackIndex * numSlices + 2);
					indexes.emplace_back(stackIndex * numSlices + sliceIndex + 1);
				}
				else
				{
					indexes.emplace_back((stackIndex - 1) * numSlices + sliceIndex + 1);
					indexes.emplace_back((stackIndex - 1) * numSlices + sliceIndex + 2);
					indexes.emplace_back(stackIndex * numSlices + sliceIndex + 2);
					indexes.emplace_back((stackIndex - 1) * numSlices + sliceIndex + 1);
					indexes.emplace_back(stackIndex * numSlices + sliceIndex + 2);
					indexes.emplace_back(stackIndex * numSlices + sliceIndex + 1);
				}
			}
		}
	}

	CalculateTangentSpaceVectors(verts, indexes);
}


void AddVertsForXYGrid3D(std::vector<Vertex_PCU>& verts, int numGridsX, int numGridsY, float gridSize, int sectionSize, float gridLineHalfWidth, float sectionLineHalfWidth, 
	float originLineHalfWidth, Rgba8 const& gridLineColor, Rgba8 const& sectionLineXColor, Rgba8 const& sectionLineYColor, Rgba8 const& originLineXColor, Rgba8 const& originLineYColor)
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

	if ((end - start).GetNormalized() == Vec3(0.0f, 0.0f, -1.0f))
	{
		AddVertsForZCylinder3D(cylinderVerts, Vec3(0.0f, 0.0f, -height), radius, height, numEdges, color, uvs);
	}
	else
	{
		AddVertsForZCylinder3D(cylinderVerts, Vec3(), radius, height, numEdges, color, uvs);
	}

	if ((end - start).GetNormalized() != Vec3(0.0f, 0.0f, 1.0f) && (end - start).GetNormalized() != Vec3(0.0f, 0.0f, -1.0f))
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


void AddVertsForZTube3D(std::vector<Vertex_PCU>& verts, Vec3 const& baseCenter, float radius, float height, int numEdges, Rgba8 const& color, AABB2 const& uvs)
{
	float degreesPerSide = 360.0f / static_cast<float>(numEdges);

	Vec3 topCenter = Vec3(baseCenter.x, baseCenter.y, baseCenter.z + height);

	for (int edgeIndex = 0; edgeIndex < numEdges; edgeIndex++)
	{
		float startDegrees = static_cast<float>(edgeIndex) * degreesPerSide;
		float endDegrees = static_cast<float>(edgeIndex + 1) * degreesPerSide;

		Vec3 baseEdgeStart = baseCenter + Vec3::MakeFromPolarDegrees(0.0f, startDegrees, radius);
		Vec3 baseEdgeEnd = baseCenter + Vec3::MakeFromPolarDegrees(0.0f, endDegrees, radius);

		Vec2 edgeStartUVs = Vec2(RangeMap(CosDegrees(startDegrees), -1.0f, 1.0f, 0.0f, 1.0f), RangeMap(SinDegrees(startDegrees), -1.0f, 1.0f, 0.0f, 1.0f));
		Vec2 edgeEndUVs = Vec2(RangeMap(CosDegrees(endDegrees), -1.0f, 1.0f, 0.0f, 1.0f), RangeMap(SinDegrees(endDegrees), -1.0f, 1.0f, 0.0f, 1.0f));

		Vec3 topEdgeStart = topCenter + Vec3::MakeFromPolarDegrees(0.0f, startDegrees, radius);
		Vec3 topEdgeEnd = topCenter + Vec3::MakeFromPolarDegrees(0.0f, endDegrees, radius);

		//draw side quad
		Vec2 bottomLeftUVs = Vec2(RangeMap(startDegrees, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), uvs.m_mins.y);
		Vec2 topRightUVs = Vec2(RangeMap(endDegrees, 0.0f, 360.0f, uvs.m_mins.x, uvs.m_maxs.x), uvs.m_maxs.y);

		AddVertsForQuad3D(verts, baseEdgeStart, baseEdgeEnd, topEdgeStart, topEdgeEnd, color, AABB2(bottomLeftUVs, topRightUVs));
	}
}


void AddVertsForTube3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, int numEdges /*= 16*/, Rgba8 const& color /*= Rgba8(255, 255, 255)*/, AABB2 const& uvs /*= AABB2(0.0f, 0.0f, 1.0f, 1.0f)*/)
{
	std::vector<Vertex_PCU> tubeVerts;

	float height = GetDistance3D(start, end);

	AddVertsForZTube3D(tubeVerts, Vec3(), radius, height, numEdges, color, uvs);

	if ((end - start).GetNormalized() != Vec3(0.0f, 0.0f, 1.0f))
	{
		Mat44 xAxisAlign = Mat44::CreateYRotationDegrees(90.0f);

		TransformVertexArray3D(tubeVerts, xAxisAlign);

		Vec3 iBasis = (end - start).GetNormalized();
		Vec3 jBasis = CrossProduct3D(Vec3(0.0f, 0.0f, 1.0f), iBasis).GetNormalized();
		Vec3 kBasis = CrossProduct3D(iBasis, jBasis).GetNormalized();

		Mat44 rotation = Mat44(iBasis, jBasis, kBasis, Vec3(0.0f, 0.0f, 0.0f));

		TransformVertexArray3D(tubeVerts, rotation);
	}

	Mat44 translation = Mat44::CreateTranslation3D(start);

	TransformVertexArray3D(tubeVerts, translation);

	verts.insert(verts.begin(), tubeVerts.begin(), tubeVerts.end());
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

	if ((end - start).GetNormalized() != Vec3(0.0f, 0.0f, 1.0f) && (end - start).GetNormalized() != Vec3(0.0f, 0.0f, -1.0f))
	{
		Mat44 xAxisAlign = Mat44::CreateYRotationDegrees(90.0f);

		TransformVertexArray3D(coneVerts, xAxisAlign);

		Vec3 iBasis = (end - start).GetNormalized();
		Vec3 jBasis = CrossProduct3D(Vec3(0.0f, 0.0f, 1.0f), iBasis).GetNormalized();
		Vec3 kBasis = CrossProduct3D(iBasis, jBasis).GetNormalized();

		Mat44 rotation = Mat44(iBasis, jBasis, kBasis, Vec3(0.0f, 0.0f, 0.0f));

		TransformVertexArray3D(coneVerts, rotation);
	}
	else if ((end - start).GetNormalized() == Vec3(0.0f, 0.0f, -1.0f))
	{
		Mat44 upsideDown = Mat44::CreateXRotationDegrees(180.0f);
		TransformVertexArray3D(coneVerts, upsideDown);
	}

	Mat44 translation = Mat44::CreateTranslation3D(start);

	TransformVertexArray3D(coneVerts, translation);

	verts.insert(verts.begin(), coneVerts.begin(), coneVerts.end());
}


void AddVertsForCapsule3D(std::vector<Vertex_PCU>& verts, Vec3 const& boneStart, Vec3 const& boneEnd, float radius, int numEdges, int numEndStacks, Rgba8 const& color)
{
	//add cylinder
	std::vector<Vertex_PCU> capsuleVerts;

	float height = GetDistance3D(boneStart, boneEnd);

	float degreesPerSide = 360.0f / static_cast<float>(numEdges);

	Vec3 topCenter = Vec3(0.0f, 0.0f, height);

	for (int edgeIndex = 0; edgeIndex < numEdges; edgeIndex++)
	{
		float startDegrees = static_cast<float>(edgeIndex) * degreesPerSide;
		float endDegrees = static_cast<float>(edgeIndex + 1) * degreesPerSide;

		Vec3 baseEdgeStart = Vec3::MakeFromPolarDegrees(0.0f, startDegrees, radius);
		Vec3 baseEdgeEnd = Vec3::MakeFromPolarDegrees(0.0f, endDegrees, radius);

		Vec3 topEdgeStart = topCenter + Vec3::MakeFromPolarDegrees(0.0f, startDegrees, radius);
		Vec3 topEdgeEnd = topCenter + Vec3::MakeFromPolarDegrees(0.0f, endDegrees, radius);

		//draw side quad
		AddVertsForQuad3D(capsuleVerts, baseEdgeStart, baseEdgeEnd, topEdgeStart, topEdgeEnd, color);
	}

	//add hemispheres on ends
	float degreesPerSlice = 360.0f / static_cast<float>(numEdges);
	float degreesPerStack = 180.0f / static_cast<float>(numEndStacks);

	for (int stackIndex = 0; stackIndex < numEndStacks; stackIndex++)
	{
		float topDegreesLat = -90.0f + (static_cast<float>(stackIndex) * degreesPerStack * 0.5f);
		float bottomDegreesLat = -90.0f + (static_cast<float>(stackIndex + 1) * degreesPerStack * 0.5f);

		for (int sliceIndex = 0; sliceIndex < numEdges; sliceIndex++)
		{
			float leftDegreesLong = static_cast<float>(sliceIndex) * degreesPerSlice;
			float rightDegreesLong = static_cast<float>(sliceIndex + 1) * degreesPerSlice;

			Vec3 bottomLeftCoords = Vec3::MakeFromPolarDegrees(bottomDegreesLat, leftDegreesLong, radius);
			Vec3 bottomRightCoords = Vec3::MakeFromPolarDegrees(bottomDegreesLat, rightDegreesLong, radius);
			Vec3 topLeftCoords = Vec3::MakeFromPolarDegrees(topDegreesLat, leftDegreesLong, radius);
			Vec3 topRightCoords = Vec3::MakeFromPolarDegrees(topDegreesLat, rightDegreesLong, radius);

			capsuleVerts.emplace_back(Vertex_PCU(bottomLeftCoords, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCU(topRightCoords, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCU(bottomRightCoords, color, Vec2()));
			
			capsuleVerts.emplace_back(Vertex_PCU(bottomLeftCoords, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCU(topLeftCoords, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCU(topRightCoords, color, Vec2()));
		}
	}
	for (int stackIndex = 0; stackIndex < numEndStacks; stackIndex++)
	{
		float topDegreesLat = (static_cast<float>(stackIndex) * degreesPerStack * 0.5f);
		float bottomDegreesLat = (static_cast<float>(stackIndex + 1) * degreesPerStack * 0.5f);

		for (int sliceIndex = 0; sliceIndex < numEdges; sliceIndex++)
		{
			float leftDegreesLong = static_cast<float>(sliceIndex) * degreesPerSlice;
			float rightDegreesLong = static_cast<float>(sliceIndex + 1) * degreesPerSlice;

			Vec3 bottomLeftCoords = Vec3::MakeFromPolarDegrees(bottomDegreesLat, leftDegreesLong, radius) + topCenter;
			Vec3 bottomRightCoords = Vec3::MakeFromPolarDegrees(bottomDegreesLat, rightDegreesLong, radius) + topCenter;
			Vec3 topLeftCoords = Vec3::MakeFromPolarDegrees(topDegreesLat, leftDegreesLong, radius) + topCenter;
			Vec3 topRightCoords = Vec3::MakeFromPolarDegrees(topDegreesLat, rightDegreesLong, radius) + topCenter;

			capsuleVerts.emplace_back(Vertex_PCU(bottomLeftCoords, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCU(topRightCoords, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCU(bottomRightCoords, color, Vec2()));

			capsuleVerts.emplace_back(Vertex_PCU(bottomLeftCoords, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCU(topLeftCoords, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCU(topRightCoords, color, Vec2()));
		}
	}

	if ((boneEnd - boneStart).GetNormalized() != Vec3(0.0f, 0.0f, 1.0f))
	{
		Mat44 xAxisAlign = Mat44::CreateYRotationDegrees(90.0f);

		TransformVertexArray3D(capsuleVerts, xAxisAlign);

		Vec3 iBasis = (boneEnd - boneStart).GetNormalized();
		Vec3 jBasis = CrossProduct3D(Vec3(0.0f, 0.0f, 1.0f), iBasis).GetNormalized();
		Vec3 kBasis = CrossProduct3D(iBasis, jBasis).GetNormalized();

		Mat44 rotation = Mat44(iBasis, jBasis, kBasis, Vec3(0.0f, 0.0f, 0.0f));

		TransformVertexArray3D(capsuleVerts, rotation);
	}

	Mat44 translation = Mat44::CreateTranslation3D(boneStart);

	TransformVertexArray3D(capsuleVerts, translation);

	verts.insert(verts.begin(), capsuleVerts.begin(), capsuleVerts.end());
}


void AddVertsForCapsule3D(std::vector<Vertex_PCUTBN>& verts, Vec3 const& boneStart, Vec3 const& boneEnd, float radius, int numEdges, int numEndStacks, Rgba8 const& color)
{
	//add cylinder
	std::vector<Vertex_PCUTBN> capsuleVerts;

	float height = GetDistance3D(boneStart, boneEnd);

	float degreesPerSide = 360.0f / static_cast<float>(numEdges);

	Vec3 topCenter = Vec3(0.0f, 0.0f, height);

	for (int edgeIndex = 0; edgeIndex < numEdges; edgeIndex++)
	{
		float startDegrees = static_cast<float>(edgeIndex) * degreesPerSide;
		float endDegrees = static_cast<float>(edgeIndex + 1) * degreesPerSide;

		Vec3 startNormal = Vec3::MakeFromPolarDegrees(0.0f, startDegrees);
		Vec3 endNormal = Vec3::MakeFromPolarDegrees(0.0f, endDegrees);

		Vec3 baseEdgeStart = startNormal * radius;
		Vec3 baseEdgeEnd = endNormal * radius;
		Vec3 topEdgeStart = topCenter + startNormal * radius;
		Vec3 topEdgeEnd = topCenter + endNormal * radius;

		capsuleVerts.push_back(Vertex_PCUTBN(baseEdgeStart, startNormal, color, Vec2()));
		capsuleVerts.push_back(Vertex_PCUTBN(baseEdgeEnd, endNormal, color, Vec2()));
		capsuleVerts.push_back(Vertex_PCUTBN(topEdgeEnd, endNormal, color, Vec2()));

		capsuleVerts.push_back(Vertex_PCUTBN(baseEdgeStart, startNormal, color, Vec2()));
		capsuleVerts.push_back(Vertex_PCUTBN(topEdgeEnd, endNormal, color, Vec2()));
		capsuleVerts.push_back(Vertex_PCUTBN(topEdgeStart, startNormal, color, Vec2()));
	}

	//add hemispheres on ends
	float degreesPerSlice = 360.0f / static_cast<float>(numEdges);
	float degreesPerStack = 180.0f / static_cast<float>(numEndStacks);

	for (int stackIndex = 0; stackIndex < numEndStacks; stackIndex++)
	{
		float topDegreesLat = -90.0f + (static_cast<float>(stackIndex) * degreesPerStack * 0.5f);
		float bottomDegreesLat = -90.0f + (static_cast<float>(stackIndex + 1) * degreesPerStack * 0.5f);

		for (int sliceIndex = 0; sliceIndex < numEdges; sliceIndex++)
		{
			float leftDegreesLong = static_cast<float>(sliceIndex) * degreesPerSlice;
			float rightDegreesLong = static_cast<float>(sliceIndex + 1) * degreesPerSlice;

			Vec3 bottomLeftNormal = Vec3::MakeFromPolarDegrees(bottomDegreesLat, leftDegreesLong);
			Vec3 bottomLeftCoords = bottomLeftNormal * radius;
			Vec3 bottomRightNormal = Vec3::MakeFromPolarDegrees(bottomDegreesLat, rightDegreesLong);
			Vec3 bottomRightCoords = bottomRightNormal * radius;
			Vec3 topLeftNormal = Vec3::MakeFromPolarDegrees(topDegreesLat, leftDegreesLong);
			Vec3 topLeftCoords = topLeftNormal * radius;
			Vec3 topRightNormal = Vec3::MakeFromPolarDegrees(topDegreesLat, rightDegreesLong);
			Vec3 topRightCoords = topRightNormal * radius;

			capsuleVerts.emplace_back(Vertex_PCUTBN(bottomLeftCoords, bottomLeftNormal, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCUTBN(topRightCoords, topRightNormal, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCUTBN(bottomRightCoords, bottomRightNormal, color, Vec2()));

			capsuleVerts.emplace_back(Vertex_PCUTBN(bottomLeftCoords, bottomLeftNormal, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCUTBN(topLeftCoords, topLeftNormal, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCUTBN(topRightCoords, topRightNormal, color, Vec2()));
		}
	}
	for (int stackIndex = 0; stackIndex < numEndStacks; stackIndex++)
	{
		float topDegreesLat = (static_cast<float>(stackIndex) * degreesPerStack * 0.5f);
		float bottomDegreesLat = (static_cast<float>(stackIndex + 1) * degreesPerStack * 0.5f);

		for (int sliceIndex = 0; sliceIndex < numEdges; sliceIndex++)
		{
			float leftDegreesLong = static_cast<float>(sliceIndex) * degreesPerSlice;
			float rightDegreesLong = static_cast<float>(sliceIndex + 1) * degreesPerSlice;

			Vec3 bottomLeftNormal = Vec3::MakeFromPolarDegrees(bottomDegreesLat, leftDegreesLong);
			Vec3 bottomLeftCoords = bottomLeftNormal * radius + topCenter;
			Vec3 bottomRightNormal = Vec3::MakeFromPolarDegrees(bottomDegreesLat, rightDegreesLong);
			Vec3 bottomRightCoords = bottomRightNormal * radius + topCenter;
			Vec3 topLeftNormal = Vec3::MakeFromPolarDegrees(topDegreesLat, leftDegreesLong);
			Vec3 topLeftCoords = topLeftNormal * radius + topCenter;
			Vec3 topRightNormal = Vec3::MakeFromPolarDegrees(topDegreesLat, rightDegreesLong);
			Vec3 topRightCoords = topRightNormal * radius + topCenter;

			capsuleVerts.emplace_back(Vertex_PCUTBN(bottomLeftCoords, bottomLeftNormal, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCUTBN(topRightCoords, topRightNormal, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCUTBN(bottomRightCoords, bottomRightNormal, color, Vec2()));

			capsuleVerts.emplace_back(Vertex_PCUTBN(bottomLeftCoords, bottomLeftNormal, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCUTBN(topLeftCoords, topLeftNormal, color, Vec2()));
			capsuleVerts.emplace_back(Vertex_PCUTBN(topRightCoords, topRightNormal, color, Vec2()));
		}
	}

	if ((boneEnd - boneStart).GetNormalized() != Vec3(0.0f, 0.0f, 1.0f))
	{
		Mat44 xAxisAlign = Mat44::CreateYRotationDegrees(90.0f);

		TransformVertexArray3D(capsuleVerts, xAxisAlign);

		Vec3 iBasis = (boneEnd - boneStart).GetNormalized();
		Vec3 jBasis = CrossProduct3D(Vec3(0.0f, 0.0f, 1.0f), iBasis).GetNormalized();
		Vec3 kBasis = CrossProduct3D(iBasis, jBasis).GetNormalized();

		Mat44 rotation = Mat44(iBasis, jBasis, kBasis, Vec3(0.0f, 0.0f, 0.0f));

		TransformVertexArray3D(capsuleVerts, rotation);
	}

	Mat44 translation = Mat44::CreateTranslation3D(boneStart);

	TransformVertexArray3D(capsuleVerts, translation);

	verts.insert(verts.begin(), capsuleVerts.begin(), capsuleVerts.end());
}


void AddVertsForEllipsoid3D(std::vector<Vertex_PCU>& verts, Vec3 const& center, float xRadius, float yRadius, float zRadius, int numSlices, int numStacks, Rgba8 const& color)
{
	std::vector<Vertex_PCU> ellipsoidVerts;

	//first make a sphere with the xradius
	AddVertsForSphere3D(ellipsoidVerts, center, xRadius, numSlices, numStacks, color);

	//then scale it along the y and z axes by the ratio between that radius and the xradius
	float yToXRatio = yRadius / xRadius;
	float zToXRatio = zRadius / xRadius;

	Mat44 scaleMatrix = Mat44::CreateNonUniformScale3D(Vec3(1.0f, yToXRatio, zToXRatio));
	TransformVertexArray3D(ellipsoidVerts, scaleMatrix);

	verts.insert(verts.begin(), ellipsoidVerts.begin(), ellipsoidVerts.end());
}


void AddVertsForEllipsoid3D(std::vector<Vertex_PCUTBN>& verts, Vec3 const& center, float xRadius, float yRadius, float zRadius, int numSlices /*= 16*/, int numStacks /*= 8*/, Rgba8 const& color /*= Rgba8()*/)
{
	std::vector<Vertex_PCUTBN> ellipsoidVerts;

	//first make a sphere with the xradius
	AddVertsForSphere3D(ellipsoidVerts, center, xRadius, numSlices, numStacks, color);

	//then scale it along the y and z axes by the ratio between that radius and the xradius
	float yToXRatio = yRadius / xRadius;
	float zToXRatio = zRadius / xRadius;

	Mat44 scaleMatrix = Mat44::CreateNonUniformScale3D(Vec3(1.0f, yToXRatio, zToXRatio));
	TransformVertexArray3D(ellipsoidVerts, scaleMatrix);

	verts.insert(verts.begin(), ellipsoidVerts.begin(), ellipsoidVerts.end());
}


void AddVertsForRoundedCube3D(std::vector<Vertex_PCU>& verts, Vec3 const& center, float halfLength, float halfWidth, float halfHeight, float roundedness, Rgba8 const& color)
{	
	std::vector<Vertex_PCU> sphubeVerts;
	
	//draw flat quads
	roundedness = GetClamped(roundedness, 0.0f, 1.0f);
	float roundLength = roundedness * halfLength;
	float flatLength = halfLength - roundLength;

	//don't need to draw quads for perfect roundedness
	if (roundedness != 1.0f)
	{
		//quad 1 (front face, +x)
		Vec3 quad1BL = Vec3(halfLength, -flatLength, -flatLength) + center;
		Vec3 quad1BR = Vec3(halfLength, flatLength, -flatLength) + center;
		Vec3 quad1TL = Vec3(halfLength, -flatLength, flatLength) + center;
		Vec3 quad1TR = Vec3(halfLength, flatLength, flatLength) + center;
		sphubeVerts.emplace_back(Vertex_PCU(quad1BL, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad1BR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad1TR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad1BL, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad1TR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad1TL, color, Vec2()));

		//quad 2 (back face, -x)
		Vec3 quad2BL = Vec3(-halfLength, flatLength, -flatLength) + center;
		Vec3 quad2BR = Vec3(-halfLength, -flatLength, -flatLength) + center;
		Vec3 quad2TL = Vec3(-halfLength, flatLength, flatLength) + center;
		Vec3 quad2TR = Vec3(-halfLength, -flatLength, flatLength) + center;
		sphubeVerts.emplace_back(Vertex_PCU(quad2BL, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad2BR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad2TR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad2BL, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad2TR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad2TL, color, Vec2()));

		//quad 3 (left face, +y)
		Vec3 quad3BL = Vec3(flatLength, halfLength, -flatLength) + center;
		Vec3 quad3BR = Vec3(-flatLength, halfLength, -flatLength) + center;
		Vec3 quad3TL = Vec3(flatLength, halfLength, flatLength) + center;
		Vec3 quad3TR = Vec3(-flatLength, halfLength, flatLength) + center;
		sphubeVerts.emplace_back(Vertex_PCU(quad3BL, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad3BR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad3TR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad3BL, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad3TR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad3TL, color, Vec2()));

		//quad 4 (right face, -y)
		Vec3 quad4BL = Vec3(-flatLength, -halfLength, -flatLength) + center;
		Vec3 quad4BR = Vec3(flatLength, -halfLength, -flatLength) + center;
		Vec3 quad4TL = Vec3(-flatLength, -halfLength, flatLength) + center;
		Vec3 quad4TR = Vec3(flatLength, -halfLength, flatLength) + center;
		sphubeVerts.emplace_back(Vertex_PCU(quad4BL, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad4BR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad4TR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad4BL, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad4TR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad4TL, color, Vec2()));

		//quad 5 (top face, +z)
		Vec3 quad5BL = Vec3(-flatLength, flatLength, halfLength) + center;
		Vec3 quad5BR = Vec3(-flatLength, -flatLength, halfLength) + center;
		Vec3 quad5TL = Vec3(flatLength, flatLength, halfLength) + center;
		Vec3 quad5TR = Vec3(flatLength, -flatLength, halfLength) + center;
		sphubeVerts.emplace_back(Vertex_PCU(quad5BL, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad5BR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad5TR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad5BL, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad5TR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad5TL, color, Vec2()));

		//quad 6 (bottom face, -z)
		Vec3 quad6BL = Vec3(flatLength, flatLength, -halfLength) + center;
		Vec3 quad6BR = Vec3(flatLength, -flatLength, -halfLength) + center;
		Vec3 quad6TL = Vec3(-flatLength, flatLength, -halfLength) + center;
		Vec3 quad6TR = Vec3(-flatLength, -flatLength, -halfLength) + center;
		sphubeVerts.emplace_back(Vertex_PCU(quad6BL, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad6BR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad6TR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad6BL, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad6TR, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCU(quad6TL, color, Vec2()));
	}

	//early out if roundedness is 0.0f
	if (roundedness == 0.0f)
	{
		//scale to make width and height match
		float widthToLengthRatio = halfWidth / halfLength;
		float heightToLengthRatio = halfHeight / halfLength;

		Mat44 scaleMatrix = Mat44::CreateNonUniformScale3D(Vec3(1.0f, widthToLengthRatio, heightToLengthRatio));
		TransformVertexArray3D(sphubeVerts, scaleMatrix);

		verts.insert(verts.begin(), sphubeVerts.begin(), sphubeVerts.end());
		return;
	}

	//draw connecting arcs
	int numSidesPerArc = 4;
	float degreesPerSide = 90.0f / static_cast<float>(numSidesPerArc);

	for (int sideIndex = 0; sideIndex < numSidesPerArc; sideIndex++)
	{
		float sideStartDegrees = static_cast<float>(sideIndex) * degreesPerSide;
		float sideEndDegrees = static_cast<float>(sideIndex + 1) * degreesPerSide;
		
		//don't need to do arcs for perfect roundedness
		if (roundedness != 1.0f)
		{
			//top forward arc
			Vec3 BL = Vec3(flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideStartDegrees, 0.0f, roundLength);
			Vec3 BR = Vec3(flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideStartDegrees, 0.0f, roundLength);
			Vec3 TL = Vec3(flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideEndDegrees, 0.0f, roundLength);
			Vec3 TR = Vec3(flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideEndDegrees, 0.0f, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//top back arc
			BL = Vec3(-flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideStartDegrees, 180.0f, roundLength);
			BR = Vec3(-flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideStartDegrees, 180.0f, roundLength);
			TL = Vec3(-flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideEndDegrees, 180.0f, roundLength);
			TR = Vec3(-flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideEndDegrees, 180.0f, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//top left arc
			BL = Vec3(flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideStartDegrees, 90.0f, roundLength);
			BR = Vec3(-flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideStartDegrees, 90.0f, roundLength);
			TL = Vec3(flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideEndDegrees, 90.0f, roundLength);
			TR = Vec3(-flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideEndDegrees, 90.0f, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//top right arc
			BL = Vec3(-flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideStartDegrees, 270.0f, roundLength);
			BR = Vec3(flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideStartDegrees, 270.0f, roundLength);
			TL = Vec3(-flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideEndDegrees, 270.0f, roundLength);
			TR = Vec3(flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(sideEndDegrees, 270.0f, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//buttom forward arc
			BL = Vec3(flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideStartDegrees, 0.0f, roundLength);
			BR = Vec3(flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideStartDegrees, 0.0f, roundLength);
			TL = Vec3(flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideEndDegrees, 0.0f, roundLength);
			TR = Vec3(flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideEndDegrees, 0.0f, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//bottom back arc
			BL = Vec3(-flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideStartDegrees, 180.0f, roundLength);
			BR = Vec3(-flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideStartDegrees, 180.0f, roundLength);
			TL = Vec3(-flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideEndDegrees, 180.0f, roundLength);
			TR = Vec3(-flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideEndDegrees, 180.0f, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//bottom left arc
			BL = Vec3(-flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideStartDegrees, 90.0f, roundLength);
			BR = Vec3(flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideStartDegrees, 90.0f, roundLength);
			TL = Vec3(-flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideEndDegrees, 90.0f, roundLength);
			TR = Vec3(flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideEndDegrees, 90.0f, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//bottom right arc
			BL = Vec3(flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideStartDegrees, 270.0f, roundLength);
			BR = Vec3(-flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideStartDegrees, 270.0f, roundLength);
			TL = Vec3(flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideEndDegrees, 270.0f, roundLength);
			TR = Vec3(-flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-sideEndDegrees, 270.0f, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//forward left arc
			BL = Vec3(flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(0.0f, sideStartDegrees, roundLength);
			BR = Vec3(flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(0.0f, sideEndDegrees, roundLength);
			TL = Vec3(flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(0.0f, sideStartDegrees, roundLength);
			TR = Vec3(flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(0.0f, sideEndDegrees, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//forward right arc
			BL = Vec3(flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(0.0f, -sideEndDegrees, roundLength);
			BR = Vec3(flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(0.0f, -sideStartDegrees, roundLength);
			TL = Vec3(flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(0.0f, -sideEndDegrees, roundLength);
			TR = Vec3(flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(0.0f, -sideStartDegrees, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//backward left arc
			BL = Vec3(-flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(0.0f, 90.0f + sideStartDegrees, roundLength);
			BR = Vec3(-flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(0.0f, 90.0f + sideEndDegrees, roundLength);
			TL = Vec3(-flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(0.0f, 90.0f + sideStartDegrees, roundLength);
			TR = Vec3(-flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(0.0f, 90.0f + sideEndDegrees, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//backward right arc
			BL = Vec3(-flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(0.0f, 180.0f + sideStartDegrees, roundLength);
			BR = Vec3(-flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(0.0f, 180.0f + sideEndDegrees, roundLength);
			TL = Vec3(-flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(0.0f, 180.0f + sideStartDegrees, roundLength);
			TR = Vec3(-flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(0.0f, 180.0f + sideEndDegrees, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));
		}

		//corners
		for (int stackIndex = 0; stackIndex < numSidesPerArc; stackIndex++)
		{
			float stackStartDegrees = static_cast<float>(stackIndex) * degreesPerSide;
			float stackEndDegrees = static_cast<float>(stackIndex + 1) * degreesPerSide;

			//forward top left corner
			Vec3 BL = Vec3(flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackStartDegrees, sideStartDegrees, roundLength);
			Vec3 BR = Vec3(flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackStartDegrees, sideEndDegrees, roundLength);
			Vec3 TL = Vec3(flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackEndDegrees, sideStartDegrees, roundLength);
			Vec3 TR = Vec3(flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackEndDegrees, sideEndDegrees, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//forward top right corner
			BL = Vec3(flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackStartDegrees, -sideEndDegrees, roundLength);
			BR = Vec3(flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackStartDegrees, -sideStartDegrees, roundLength);
			TL = Vec3(flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackEndDegrees, -sideEndDegrees, roundLength);
			TR = Vec3(flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackEndDegrees, -sideStartDegrees, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//back top left corner
			BL = Vec3(-flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackStartDegrees, 90.0f + sideStartDegrees, roundLength);
			BR = Vec3(-flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackStartDegrees, 90.0f + sideEndDegrees, roundLength);
			TL = Vec3(-flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackEndDegrees, 90.0f + sideStartDegrees, roundLength);
			TR = Vec3(-flatLength, flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackEndDegrees, 90.0f + sideEndDegrees, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//back top right corner
			BL = Vec3(-flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackStartDegrees, 180.0f + sideStartDegrees, roundLength);
			BR = Vec3(-flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackStartDegrees, 180.0f + sideEndDegrees, roundLength);
			TL = Vec3(-flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackEndDegrees, 180.0f + sideStartDegrees, roundLength);
			TR = Vec3(-flatLength, -flatLength, flatLength) + Vec3::MakeFromPolarDegrees(stackEndDegrees, 180.0f + sideEndDegrees, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//forward bottom left corner
			BL = Vec3(flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackStartDegrees, sideEndDegrees, roundLength);
			BR = Vec3(flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackStartDegrees, sideStartDegrees, roundLength);
			TL = Vec3(flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackEndDegrees, sideEndDegrees, roundLength);
			TR = Vec3(flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackEndDegrees, sideStartDegrees, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//forward bottom right corner
			BL = Vec3(flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackStartDegrees, -sideStartDegrees, roundLength);
			BR = Vec3(flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackStartDegrees, -sideEndDegrees, roundLength);
			TL = Vec3(flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackEndDegrees, -sideStartDegrees, roundLength);
			TR = Vec3(flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackEndDegrees, -sideEndDegrees, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//back bottom left corner
			BL = Vec3(-flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackStartDegrees, 90.0f + sideEndDegrees, roundLength);
			BR = Vec3(-flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackStartDegrees, 90.0f + sideStartDegrees, roundLength);
			TL = Vec3(-flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackEndDegrees, 90.0f + sideEndDegrees, roundLength);
			TR = Vec3(-flatLength, flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackEndDegrees, 90.0f + sideStartDegrees, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));

			//back bottom right corner
			BL = Vec3(-flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackStartDegrees, 180.0f + sideEndDegrees, roundLength);
			BR = Vec3(-flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackStartDegrees, 180.0f + sideStartDegrees, roundLength);
			TL = Vec3(-flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackEndDegrees, 180.0f + sideEndDegrees, roundLength);
			TR = Vec3(-flatLength, -flatLength, -flatLength) + Vec3::MakeFromPolarDegrees(-stackEndDegrees, 180.0f + sideStartDegrees, roundLength);
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(BL, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TR, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCU(TL, color, Vec2()));
		}
	}

	//scale to make width and height match
	float widthToLengthRatio = halfWidth / halfLength;
	float heightToLengthRatio = halfHeight / halfLength;

	Mat44 scaleMatrix = Mat44::CreateNonUniformScale3D(Vec3(1.0f, widthToLengthRatio, heightToLengthRatio));
	TransformVertexArray3D(sphubeVerts, scaleMatrix);

	verts.insert(verts.begin(), sphubeVerts.begin(), sphubeVerts.end());
}


void AddVertsForRoundedCube3D(std::vector<Vertex_PCUTBN>& verts, Vec3 const& center, float halfLength, float halfWidth, float halfHeight, float roundedness, Rgba8 const& color)
{
	std::vector<Vertex_PCUTBN> sphubeVerts;

	//draw flat quads
	roundedness = GetClamped(roundedness, 0.0f, 1.0f);
	float roundLength = roundedness * halfLength;
	float flatLength = halfLength - roundLength;

	//don't need to draw quads for perfect roundedness
	if (roundedness != 1.0f)
	{
		//quad 1 (front face, +x)
		Vec3 quad1BL = Vec3(halfLength, -flatLength, -flatLength) + center;
		Vec3 quad1BR = Vec3(halfLength, flatLength, -flatLength) + center;
		Vec3 quad1TL = Vec3(halfLength, -flatLength, flatLength) + center;
		Vec3 quad1TR = Vec3(halfLength, flatLength, flatLength) + center;
		Vec3 quad1Normal = Vec3(1.0f, 0.0f, 0.0f);
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad1BL, quad1Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad1BR, quad1Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad1TR, quad1Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad1BL, quad1Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad1TR, quad1Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad1TL, quad1Normal, color, Vec2()));

		//quad 2 (back face, -x)
		Vec3 quad2BL = Vec3(-halfLength, flatLength, -flatLength) + center;
		Vec3 quad2BR = Vec3(-halfLength, -flatLength, -flatLength) + center;
		Vec3 quad2TL = Vec3(-halfLength, flatLength, flatLength) + center;
		Vec3 quad2TR = Vec3(-halfLength, -flatLength, flatLength) + center;
		Vec3 quad2Normal = Vec3(-1.0f, 0.0f, 0.0f);
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad2BL, quad2Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad2BR, quad2Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad2TR, quad2Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad2BL, quad2Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad2TR, quad2Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad2TL, quad2Normal, color, Vec2()));

		//quad 3 (left face, +y)
		Vec3 quad3BL = Vec3(flatLength, halfLength, -flatLength) + center;
		Vec3 quad3BR = Vec3(-flatLength, halfLength, -flatLength) + center;
		Vec3 quad3TL = Vec3(flatLength, halfLength, flatLength) + center;
		Vec3 quad3TR = Vec3(-flatLength, halfLength, flatLength) + center;
		Vec3 quad3Normal = Vec3(0.0f, 1.0f, 0.0f);
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad3BL, quad3Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad3BR, quad3Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad3TR, quad3Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad3BL, quad3Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad3TR, quad3Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad3TL, quad3Normal, color, Vec2()));

		//quad 4 (right face, -y)
		Vec3 quad4BL = Vec3(-flatLength, -halfLength, -flatLength) + center;
		Vec3 quad4BR = Vec3(flatLength, -halfLength, -flatLength) + center;
		Vec3 quad4TL = Vec3(-flatLength, -halfLength, flatLength) + center;
		Vec3 quad4TR = Vec3(flatLength, -halfLength, flatLength) + center;
		Vec3 quad4Normal = Vec3(0.0f, -1.0f, 0.0f);
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad4BL, quad4Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad4BR, quad4Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad4TR, quad4Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad4BL, quad4Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad4TR, quad4Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad4TL, quad4Normal, color, Vec2()));

		//quad 5 (top face, +z)
		Vec3 quad5BL = Vec3(-flatLength, flatLength, halfLength) + center;
		Vec3 quad5BR = Vec3(-flatLength, -flatLength, halfLength) + center;
		Vec3 quad5TL = Vec3(flatLength, flatLength, halfLength) + center;
		Vec3 quad5TR = Vec3(flatLength, -flatLength, halfLength) + center;
		Vec3 quad5Normal = Vec3(0.0f, 0.0f, 1.0f);
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad5BL, quad5Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad5BR, quad5Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad5TR, quad5Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad5BL, quad5Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad5TR, quad5Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad5TL, quad5Normal, color, Vec2()));

		//quad 6 (bottom face, -z)
		Vec3 quad6BL = Vec3(flatLength, flatLength, -halfLength) + center;
		Vec3 quad6BR = Vec3(flatLength, -flatLength, -halfLength) + center;
		Vec3 quad6TL = Vec3(-flatLength, flatLength, -halfLength) + center;
		Vec3 quad6TR = Vec3(-flatLength, -flatLength, -halfLength) + center;
		Vec3 quad6Normal = Vec3(0.0f, 0.0f, -1.0f);
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad6BL, quad6Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad6BR, quad6Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad6TR, quad6Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad6BL, quad6Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad6TR, quad6Normal, color, Vec2()));
		sphubeVerts.emplace_back(Vertex_PCUTBN(quad6TL, quad6Normal, color, Vec2()));
	}

	//early out if roundedness is 0.0f
	if (roundedness == 0.0f)
	{
		//scale to make width and height match
		float widthToLengthRatio = halfWidth / halfLength;
		float heightToLengthRatio = halfHeight / halfLength;

		Mat44 scaleMatrix = Mat44::CreateNonUniformScale3D(Vec3(1.0f, widthToLengthRatio, heightToLengthRatio));
		TransformVertexArray3D(sphubeVerts, scaleMatrix);

		verts.insert(verts.begin(), sphubeVerts.begin(), sphubeVerts.end());
		return;
	}

	//draw connecting arcs
	int numSidesPerArc = 4;
	float degreesPerSide = 90.0f / static_cast<float>(numSidesPerArc);

	for (int sideIndex = 0; sideIndex < numSidesPerArc; sideIndex++)
	{
		float sideStartDegrees = static_cast<float>(sideIndex) * degreesPerSide;
		float sideEndDegrees = static_cast<float>(sideIndex + 1) * degreesPerSide;

		//don't need to do arcs for perfect roundedness
		if (roundedness != 1.0f)
		{
			//top forward arc
			Vec3 startNormal = Vec3::MakeFromPolarDegrees(sideStartDegrees, 0.0f);
			Vec3 BL = Vec3(flatLength, -flatLength, flatLength) + startNormal * roundLength;
			Vec3 BR = Vec3(flatLength, flatLength, flatLength) + startNormal * roundLength;
			Vec3 endNormal = Vec3::MakeFromPolarDegrees(sideEndDegrees, 0.0f);
			Vec3 TL = Vec3(flatLength, -flatLength, flatLength) + endNormal * roundLength;
			Vec3 TR = Vec3(flatLength, flatLength, flatLength) + endNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, endNormal, color, Vec2()));

			//top back arc
			startNormal = Vec3::MakeFromPolarDegrees(sideStartDegrees, 180.0f);
			BL = Vec3(-flatLength, flatLength, flatLength) + startNormal * roundLength;
			BR = Vec3(-flatLength, -flatLength, flatLength) + startNormal * roundLength;
			endNormal = Vec3::MakeFromPolarDegrees(sideEndDegrees, 180.0f);
			TL = Vec3(-flatLength, flatLength, flatLength) + endNormal * roundLength;
			TR = Vec3(-flatLength, -flatLength, flatLength) + endNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, endNormal, color, Vec2()));

			//top left arc
			startNormal = Vec3::MakeFromPolarDegrees(sideStartDegrees, 90.0f);
			BL = Vec3(flatLength, flatLength, flatLength) + startNormal * roundLength;
			BR = Vec3(-flatLength, flatLength, flatLength) + startNormal * roundLength;
			endNormal = Vec3::MakeFromPolarDegrees(sideEndDegrees, 90.0f);
			TL = Vec3(flatLength, flatLength, flatLength) + endNormal * roundLength;
			TR = Vec3(-flatLength, flatLength, flatLength) + endNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, endNormal, color, Vec2()));

			//top right arc
			startNormal = Vec3::MakeFromPolarDegrees(sideStartDegrees, 270.0f);
			BL = Vec3(-flatLength, -flatLength, flatLength) + startNormal * roundLength;
			BR = Vec3(flatLength, -flatLength, flatLength) + startNormal * roundLength;
			endNormal = Vec3::MakeFromPolarDegrees(sideEndDegrees, 270.0f);
			TL = Vec3(-flatLength, -flatLength, flatLength) + endNormal * roundLength;
			TR = Vec3(flatLength, -flatLength, flatLength) + endNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, endNormal, color, Vec2()));

			//buttom forward arc
			startNormal = Vec3::MakeFromPolarDegrees(-sideStartDegrees, 0.0f);
			BL = Vec3(flatLength, flatLength, -flatLength) + startNormal * roundLength;
			BR = Vec3(flatLength, -flatLength, -flatLength) + startNormal * roundLength;
			endNormal = Vec3::MakeFromPolarDegrees(-sideEndDegrees, 0.0f);
			TL = Vec3(flatLength, flatLength, -flatLength) + endNormal * roundLength;
			TR = Vec3(flatLength, -flatLength, -flatLength) + endNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, endNormal, color, Vec2()));

			//bottom back arc
			startNormal = Vec3::MakeFromPolarDegrees(-sideStartDegrees, 180.0f);
			BL = Vec3(-flatLength, -flatLength, -flatLength) + startNormal * roundLength;
			BR = Vec3(-flatLength, flatLength, -flatLength) + startNormal * roundLength;
			endNormal = Vec3::MakeFromPolarDegrees(-sideEndDegrees, 180.0f);
			TL = Vec3(-flatLength, -flatLength, -flatLength) + endNormal * roundLength;
			TR = Vec3(-flatLength, flatLength, -flatLength) + endNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, endNormal, color, Vec2()));

			//bottom left arc
			startNormal = Vec3::MakeFromPolarDegrees(-sideStartDegrees, 90.0f);
			BL = Vec3(-flatLength, flatLength, -flatLength) + startNormal * roundLength;
			BR = Vec3(flatLength, flatLength, -flatLength) + startNormal * roundLength;
			endNormal = Vec3::MakeFromPolarDegrees(-sideEndDegrees, 90.0f);
			TL = Vec3(-flatLength, flatLength, -flatLength) + endNormal * roundLength;
			TR = Vec3(flatLength, flatLength, -flatLength) + endNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, endNormal, color, Vec2()));

			//bottom right arc
			startNormal = Vec3::MakeFromPolarDegrees(-sideStartDegrees, 270.0f);
			BL = Vec3(flatLength, -flatLength, -flatLength) + startNormal * roundLength;
			BR = Vec3(-flatLength, -flatLength, -flatLength) + startNormal * roundLength;
			endNormal = Vec3::MakeFromPolarDegrees(-sideEndDegrees, 270.0f);
			TL = Vec3(flatLength, -flatLength, -flatLength) + endNormal * roundLength;
			TR = Vec3(-flatLength, -flatLength, -flatLength) + endNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, endNormal, color, Vec2()));

			//forward left arc
			startNormal = Vec3::MakeFromPolarDegrees(0.0f, sideStartDegrees);
			endNormal = Vec3::MakeFromPolarDegrees(0.0f, sideEndDegrees);
			BL = Vec3(flatLength, flatLength, -flatLength) + startNormal * roundLength;
			BR = Vec3(flatLength, flatLength, -flatLength) + endNormal * roundLength;
			TL = Vec3(flatLength, flatLength, flatLength) + startNormal * roundLength;
			TR = Vec3(flatLength, flatLength, flatLength) + endNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, startNormal, color, Vec2()));

			//forward right arc
			startNormal = Vec3::MakeFromPolarDegrees(0.0f, -sideStartDegrees);
			endNormal = Vec3::MakeFromPolarDegrees(0.0f, -sideEndDegrees);
			BL = Vec3(flatLength, -flatLength, -flatLength) + endNormal * roundLength;
			BR = Vec3(flatLength, -flatLength, -flatLength) + startNormal * roundLength;
			TL = Vec3(flatLength, -flatLength, flatLength) + endNormal * roundLength;
			TR = Vec3(flatLength, -flatLength, flatLength) + startNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, endNormal, color, Vec2()));

			//backward left arc
			startNormal = Vec3::MakeFromPolarDegrees(0.0f, 90.0f + sideStartDegrees);
			endNormal = Vec3::MakeFromPolarDegrees(0.0f, 90.0f + sideEndDegrees);
			BL = Vec3(-flatLength, flatLength, -flatLength) + startNormal * roundLength;
			BR = Vec3(-flatLength, flatLength, -flatLength) + endNormal * roundLength;
			TL = Vec3(-flatLength, flatLength, flatLength) + startNormal * roundLength;
			TR = Vec3(-flatLength, flatLength, flatLength) + endNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, startNormal, color, Vec2()));

			//backward right arc
			startNormal = Vec3::MakeFromPolarDegrees(0.0f, 180.0f + sideStartDegrees);
			endNormal = Vec3::MakeFromPolarDegrees(0.0f, 180.0f + sideEndDegrees);
			BL = Vec3(-flatLength, -flatLength, -flatLength) + startNormal * roundLength;
			BR = Vec3(-flatLength, -flatLength, -flatLength) + endNormal * roundLength;
			TL = Vec3(-flatLength, -flatLength, flatLength) + startNormal * roundLength;
			TR = Vec3(-flatLength, -flatLength, flatLength) + endNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, startNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, endNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, startNormal, color, Vec2()));
		}

		//corners
		for (int stackIndex = 0; stackIndex < numSidesPerArc; stackIndex++)
		{
			float stackStartDegrees = static_cast<float>(stackIndex) * degreesPerSide;
			float stackEndDegrees = static_cast<float>(stackIndex + 1) * degreesPerSide;

			//forward top left corner
			Vec3 blNormal = Vec3::MakeFromPolarDegrees(stackStartDegrees, sideStartDegrees);
			Vec3 BL = Vec3(flatLength, flatLength, flatLength) + blNormal * roundLength;
			Vec3 brNormal = Vec3::MakeFromPolarDegrees(stackStartDegrees, sideEndDegrees);
			Vec3 BR = Vec3(flatLength, flatLength, flatLength) + brNormal * roundLength;
			Vec3 tlNormal = Vec3::MakeFromPolarDegrees(stackEndDegrees, sideStartDegrees);
			Vec3 TL = Vec3(flatLength, flatLength, flatLength) + tlNormal * roundLength;
			Vec3 trNormal = Vec3::MakeFromPolarDegrees(stackEndDegrees, sideEndDegrees);
			Vec3 TR = Vec3(flatLength, flatLength, flatLength) + trNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, brNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, tlNormal, color, Vec2()));

			//forward top right corner
			blNormal = Vec3::MakeFromPolarDegrees(stackStartDegrees, -sideEndDegrees);
			BL = Vec3(flatLength, -flatLength, flatLength) + blNormal * roundLength;
			brNormal = Vec3::MakeFromPolarDegrees(stackStartDegrees, -sideStartDegrees);
			BR = Vec3(flatLength, -flatLength, flatLength) + brNormal * roundLength;
			tlNormal = Vec3::MakeFromPolarDegrees(stackEndDegrees, -sideEndDegrees);
			TL = Vec3(flatLength, -flatLength, flatLength) + tlNormal * roundLength;
			trNormal = Vec3::MakeFromPolarDegrees(stackEndDegrees, -sideStartDegrees);
			TR = Vec3(flatLength, -flatLength, flatLength) + trNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, brNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, tlNormal, color, Vec2()));

			//back top left corner
			blNormal = Vec3::MakeFromPolarDegrees(stackStartDegrees, 90.0f + sideStartDegrees);
			BL = Vec3(-flatLength, flatLength, flatLength) + blNormal * roundLength;
			brNormal = Vec3::MakeFromPolarDegrees(stackStartDegrees, 90.0f + sideEndDegrees);
			BR = Vec3(-flatLength, flatLength, flatLength) + brNormal * roundLength;
			tlNormal = Vec3::MakeFromPolarDegrees(stackEndDegrees, 90.0f + sideStartDegrees);
			TL = Vec3(-flatLength, flatLength, flatLength) + tlNormal * roundLength;
			trNormal = Vec3::MakeFromPolarDegrees(stackEndDegrees, 90.0f + sideEndDegrees);
			TR = Vec3(-flatLength, flatLength, flatLength) + trNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, brNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, tlNormal, color, Vec2()));

			//back top right corner
			blNormal = Vec3::MakeFromPolarDegrees(stackStartDegrees, 180.0f + sideStartDegrees);
			BL = Vec3(-flatLength, -flatLength, flatLength) + blNormal * roundLength;
			brNormal = Vec3::MakeFromPolarDegrees(stackStartDegrees, 180.0f + sideEndDegrees);
			BR = Vec3(-flatLength, -flatLength, flatLength) + brNormal * roundLength;
			tlNormal = Vec3::MakeFromPolarDegrees(stackEndDegrees, 180.0f + sideStartDegrees);
			TL = Vec3(-flatLength, -flatLength, flatLength) + tlNormal * roundLength;
			trNormal = Vec3::MakeFromPolarDegrees(stackEndDegrees, 180.0f + sideEndDegrees);
			TR = Vec3(-flatLength, -flatLength, flatLength) + trNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, brNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, tlNormal, color, Vec2()));

			//forward bottom left corner
			blNormal = Vec3::MakeFromPolarDegrees(-stackStartDegrees, sideEndDegrees);
			BL = Vec3(flatLength, flatLength, -flatLength) + blNormal * roundLength;
			brNormal = Vec3::MakeFromPolarDegrees(-stackStartDegrees, sideStartDegrees);
			BR = Vec3(flatLength, flatLength, -flatLength) + brNormal * roundLength;
			tlNormal = Vec3::MakeFromPolarDegrees(-stackEndDegrees, sideEndDegrees);
			TL = Vec3(flatLength, flatLength, -flatLength) + tlNormal * roundLength;
			trNormal = Vec3::MakeFromPolarDegrees(-stackEndDegrees, sideStartDegrees);
			TR = Vec3(flatLength, flatLength, -flatLength) + trNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, brNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, tlNormal, color, Vec2()));

			//forward bottom right corner
			blNormal = Vec3::MakeFromPolarDegrees(-stackStartDegrees, -sideStartDegrees);
			BL = Vec3(flatLength, -flatLength, -flatLength) + blNormal * roundLength;
			brNormal = Vec3::MakeFromPolarDegrees(-stackStartDegrees, -sideEndDegrees);
			BR = Vec3(flatLength, -flatLength, -flatLength) + brNormal * roundLength;
			tlNormal = Vec3::MakeFromPolarDegrees(-stackEndDegrees, -sideStartDegrees);
			TL = Vec3(flatLength, -flatLength, -flatLength) + tlNormal * roundLength;
			trNormal = Vec3::MakeFromPolarDegrees(-stackEndDegrees, -sideEndDegrees);
			TR = Vec3(flatLength, -flatLength, -flatLength) + trNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, brNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, tlNormal, color, Vec2()));

			//back bottom left corner
			blNormal = Vec3::MakeFromPolarDegrees(-stackStartDegrees, 90.0f + sideEndDegrees);
			BL = Vec3(-flatLength, flatLength, -flatLength) + blNormal * roundLength;
			brNormal = Vec3::MakeFromPolarDegrees(-stackStartDegrees, 90.0f + sideStartDegrees);
			BR = Vec3(-flatLength, flatLength, -flatLength) + brNormal * roundLength;
			tlNormal = Vec3::MakeFromPolarDegrees(-stackEndDegrees, 90.0f + sideEndDegrees);
			TL = Vec3(-flatLength, flatLength, -flatLength) + tlNormal * roundLength;
			trNormal = Vec3::MakeFromPolarDegrees(-stackEndDegrees, 90.0f + sideStartDegrees);
			TR = Vec3(-flatLength, flatLength, -flatLength) + trNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, brNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, tlNormal, color, Vec2()));

			//back bottom right corner
			blNormal = Vec3::MakeFromPolarDegrees(-stackStartDegrees, 180.0f + sideEndDegrees);
			BL = Vec3(-flatLength, -flatLength, -flatLength) + blNormal * roundLength;
			brNormal = Vec3::MakeFromPolarDegrees(-stackStartDegrees, 180.0f + sideStartDegrees);
			BR = Vec3(-flatLength, -flatLength, -flatLength) + brNormal * roundLength;
			tlNormal = Vec3::MakeFromPolarDegrees(-stackEndDegrees, 180.0f + sideEndDegrees);
			TL = Vec3(-flatLength, -flatLength, -flatLength) + tlNormal * roundLength;
			trNormal = Vec3::MakeFromPolarDegrees(-stackEndDegrees, 180.0f + sideStartDegrees);
			TR = Vec3(-flatLength, -flatLength, -flatLength) + trNormal * roundLength;
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BR, brNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(BL, blNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TR, trNormal, color, Vec2()));
			sphubeVerts.emplace_back(Vertex_PCUTBN(TL, tlNormal, color, Vec2()));
		}
	}

	//scale to make width and height match
	float widthToLengthRatio = halfWidth / halfLength;
	float heightToLengthRatio = halfHeight / halfLength;

	Mat44 scaleMatrix = Mat44::CreateNonUniformScale3D(Vec3(1.0f, widthToLengthRatio, heightToLengthRatio));
	TransformVertexArray3D(sphubeVerts, scaleMatrix);

	verts.insert(verts.begin(), sphubeVerts.begin(), sphubeVerts.end());
}


void AddVertsForTorus3D(std::vector<Vertex_PCU>& verts, Vec3 const& center, float tubeRadius, float holeRadius, int numSlices, int numSegments, Rgba8 const& color)
{
	float tubeCenterDist = tubeRadius + holeRadius;

	float degreesPerSegment = 360.0f / static_cast<float>(numSegments);
	float degreesPerSlice = 360.0f / static_cast<float>(numSlices);

	//loop around and add each segment
	for (int segIndex = 0; segIndex < numSegments; segIndex++)
	{
		float segStartDegrees = static_cast<float>(segIndex) * degreesPerSegment;
		float segEndDegrees = static_cast<float>(segIndex + 1) * degreesPerSegment;
		float cosSegStart = CosDegrees(segStartDegrees);
		float sinSegStart = SinDegrees(segStartDegrees);
		float cosSegEnd = CosDegrees(segEndDegrees);
		float sinSegEnd = SinDegrees(segEndDegrees);

		Vec3 segStartCenter = Vec3(tubeCenterDist * cosSegStart, tubeCenterDist * sinSegStart, 0.0f) + center;
		Vec3 segEndCenter = Vec3(tubeCenterDist * cosSegEnd, tubeCenterDist * sinSegEnd, 0.0f) + center;

		for (int sliceIndex = 0; sliceIndex < numSlices; sliceIndex++)
		{
			float sliceStartDegrees = static_cast<float>(sliceIndex) * degreesPerSlice;
			float sliceEndDegrees = static_cast<float>(sliceIndex + 1) * degreesPerSlice;
			float cosSliceStart = CosDegrees(sliceStartDegrees);
			float sinSliceStart = SinDegrees(sliceStartDegrees);
			float cosSliceEnd = CosDegrees(sliceEndDegrees);
			float sinSliceEnd = SinDegrees(sliceEndDegrees);

			Vec3 quadBottomLeft = Vec3(tubeRadius * cosSliceEnd * cosSegEnd, tubeRadius * cosSliceEnd * sinSegEnd, tubeRadius * sinSliceEnd) + segEndCenter;
			Vec3 quadBottomRight = Vec3(tubeRadius * cosSliceStart * cosSegEnd, tubeRadius * cosSliceStart * sinSegEnd, tubeRadius * sinSliceStart) + segEndCenter;
			Vec3 quadTopLeft = Vec3(tubeRadius * cosSliceEnd * cosSegStart, tubeRadius * cosSliceEnd * sinSegStart, tubeRadius * sinSliceEnd) + segStartCenter;
			Vec3 quadTopRight = Vec3(tubeRadius * cosSliceStart * cosSegStart, tubeRadius * cosSliceStart * sinSegStart, tubeRadius * sinSliceStart) + segStartCenter;

			verts.emplace_back(Vertex_PCU(quadBottomLeft, color, Vec2()));
			verts.emplace_back(Vertex_PCU(quadBottomRight, color, Vec2()));
			verts.emplace_back(Vertex_PCU(quadTopRight, color, Vec2()));

			verts.emplace_back(Vertex_PCU(quadBottomLeft, color, Vec2()));
			verts.emplace_back(Vertex_PCU(quadTopRight, color, Vec2()));
			verts.emplace_back(Vertex_PCU(quadTopLeft, color, Vec2()));
		}
	}
}


void AddVertsForTorus3D(std::vector<Vertex_PCUTBN>& verts, Vec3 const& center, float tubeRadius, float holeRadius, int numSlices, int numSegments, Rgba8 const& color)
{
	float tubeCenterDist = tubeRadius + holeRadius;

	float degreesPerSegment = 360.0f / static_cast<float>(numSegments);
	float degreesPerSlice = 360.0f / static_cast<float>(numSlices);

	//loop around and add each segment
	for (int segIndex = 0; segIndex < numSegments; segIndex++)
	{
		float segStartDegrees = static_cast<float>(segIndex) * degreesPerSegment;
		float segEndDegrees = static_cast<float>(segIndex + 1) * degreesPerSegment;
		float cosSegStart = CosDegrees(segStartDegrees);
		float sinSegStart = SinDegrees(segStartDegrees);
		float cosSegEnd = CosDegrees(segEndDegrees);
		float sinSegEnd = SinDegrees(segEndDegrees);

		Vec3 segStartCenter = Vec3(tubeCenterDist * cosSegStart, tubeCenterDist * sinSegStart, 0.0f) + center;
		Vec3 segEndCenter = Vec3(tubeCenterDist * cosSegEnd, tubeCenterDist * sinSegEnd, 0.0f) + center;

		for (int sliceIndex = 0; sliceIndex < numSlices; sliceIndex++)
		{
			float sliceStartDegrees = static_cast<float>(sliceIndex) * degreesPerSlice;
			float sliceEndDegrees = static_cast<float>(sliceIndex + 1) * degreesPerSlice;
			float cosSliceStart = CosDegrees(sliceStartDegrees);
			float sinSliceStart = SinDegrees(sliceStartDegrees);
			float cosSliceEnd = CosDegrees(sliceEndDegrees);
			float sinSliceEnd = SinDegrees(sliceEndDegrees);

			Vec3 quadBottomLeftNormal = Vec3(cosSliceEnd * cosSegEnd, cosSliceEnd * sinSegEnd, sinSliceEnd);
			Vec3 quadBottomLeft = quadBottomLeftNormal * tubeRadius + segEndCenter;
			Vec3 quadBottomRightNormal = Vec3(cosSliceStart * cosSegEnd, cosSliceStart * sinSegEnd, sinSliceStart);
			Vec3 quadBottomRight = quadBottomRightNormal * tubeRadius + segEndCenter;
			Vec3 quadTopLeftNormal = Vec3(cosSliceEnd * cosSegStart, cosSliceEnd * sinSegStart, sinSliceEnd);
			Vec3 quadTopLeft = quadTopLeftNormal * tubeRadius + segStartCenter;
			Vec3 quadTopRightNormal = Vec3(cosSliceStart * cosSegStart, cosSliceStart * sinSegStart, sinSliceStart);
			Vec3 quadTopRight = quadTopRightNormal * tubeRadius + segStartCenter;

			verts.emplace_back(Vertex_PCUTBN(quadBottomLeft, quadBottomLeftNormal, color, Vec2()));
			verts.emplace_back(Vertex_PCUTBN(quadTopRight, quadTopRightNormal, color, Vec2()));
			verts.emplace_back(Vertex_PCUTBN(quadBottomRight, quadBottomRightNormal, color, Vec2()));

			verts.emplace_back(Vertex_PCUTBN(quadBottomLeft, quadBottomLeftNormal, color, Vec2()));
			verts.emplace_back(Vertex_PCUTBN(quadTopLeft, quadTopLeftNormal, color, Vec2()));
			verts.emplace_back(Vertex_PCUTBN(quadTopRight, quadTopRightNormal, color, Vec2()));
		}
	}
}


void AddVertsForZSector3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, float height, float radius, float forwardDegrees, float apertureDegrees, int numEdges, Rgba8 const& color)
{
	float degreesPerSide = apertureDegrees / static_cast<float>(numEdges);

	Vec3 topCenter = Vec3(start.x, start.y, start.z + height);

	for (int edgeIndex = 0; edgeIndex < numEdges; edgeIndex++)
	{
		float startDegrees = static_cast<float>(edgeIndex) * degreesPerSide + (forwardDegrees - (apertureDegrees * 0.5f));
		float endDegrees = startDegrees + degreesPerSide;

		//draw triangle at base
		Vec3 baseEdgeStart = start + Vec3::MakeFromPolarDegrees(0.0f, startDegrees, radius);
		Vec3 baseEdgeEnd = start + Vec3::MakeFromPolarDegrees(0.0f, endDegrees, radius);

		//NOTE: Not bothering to support UVs for this
		
		verts.push_back(Vertex_PCU(start, color, Vec2()));
		verts.push_back(Vertex_PCU(baseEdgeEnd, color, Vec2()));
		verts.push_back(Vertex_PCU(baseEdgeStart, color, Vec2()));

		//draw triangle at top
		Vec3 topEdgeStart = topCenter + Vec3::MakeFromPolarDegrees(0.0f, startDegrees, radius);
		Vec3 topEdgeEnd = topCenter + Vec3::MakeFromPolarDegrees(0.0f, endDegrees, radius);

		verts.push_back(Vertex_PCU(topCenter, color, Vec2()));
		verts.push_back(Vertex_PCU(topEdgeStart, color, Vec2()));
		verts.push_back(Vertex_PCU(topEdgeEnd, color, Vec2()));

		AddVertsForQuad3D(verts, baseEdgeStart, baseEdgeEnd, topEdgeStart, topEdgeEnd, color);

		if (edgeIndex == 0)
		{
			AddVertsForQuad3D(verts, baseEdgeStart, topEdgeStart, start, topCenter, color);
		}
		else if (edgeIndex == numEdges - 1)
		{
			AddVertsForQuad3D(verts, topEdgeEnd, baseEdgeEnd, topCenter, start, color);
		}
	}
}


void AddVertsForSector3D(std::vector<Vertex_PCU>& verts, float radius, Vec3 const& start, Vec3 const& end, float forwardDegrees, float apertureDegrees, int numEdges, 
	Rgba8 const& color)
{
	std::vector<Vertex_PCU> sectorVerts;

	float height = GetDistance3D(start, end);

	if ((end - start).GetNormalized() == Vec3(0.0f, 0.0f, -1.0f))
	{
		AddVertsForZSector3D(sectorVerts, Vec3(0.0f, 0.0f, -height), height, radius, forwardDegrees, apertureDegrees, numEdges, color);
	}
	else
	{
		AddVertsForZSector3D(sectorVerts, Vec3(), height, radius, forwardDegrees, apertureDegrees, numEdges, color);
	}

	Vec3 kBasis = end - start;
	kBasis.Normalize();
	if (kBasis != Vec3(0.0f, 0.0f, 1.0f) && kBasis != Vec3(0.0f, 0.0f, 1.0f))
	{
		Vec3 jBasis = CrossProduct3D(kBasis, Vec3(0.0f, 0.0f, 1.0f)).GetNormalized();
		Vec3 iBasis = CrossProduct3D(jBasis, kBasis).GetNormalized();

		Mat44 rotation = Mat44(iBasis, jBasis, kBasis, Vec3(0.0f, 0.0f, 0.0f));

		TransformVertexArray3D(sectorVerts, rotation);
	}

	Mat44 translation = Mat44::CreateTranslation3D(start);

	TransformVertexArray3D(sectorVerts, translation);

	verts.insert(verts.begin(), sectorVerts.begin(), sectorVerts.end());
}


void AddVertsForMobiusStrip3D(std::vector<Vertex_PCU>& verts, Vec3 const& center, float radius, float halfWidth, int numSegments, Rgba8 const& color)
{
	float degreesPerSegment = 360.0f / static_cast<float>(numSegments);

	for (int segIndex = 0; segIndex < numSegments; segIndex++)
	{
		float startDegrees = static_cast<float>(segIndex) * degreesPerSegment;
		float endDegrees = startDegrees + degreesPerSegment;

		float startLeftCoef = (radius + (-halfWidth * CosDegrees(0.5f * startDegrees)));
		float startXLeft = startLeftCoef * CosDegrees(startDegrees);
		float startYLeft = startLeftCoef * SinDegrees(startDegrees);
		float startZLeft = -halfWidth * SinDegrees(0.5f * startDegrees);

		float startRightCoef = (radius + (halfWidth * CosDegrees(0.5f * startDegrees)));
		float startXRight = startRightCoef * CosDegrees(startDegrees);
		float startYRight = startRightCoef * SinDegrees(startDegrees);
		float startZRight = halfWidth * SinDegrees(0.5f * startDegrees);

		float endLeftCoef = (radius + (-halfWidth * CosDegrees(0.5f * endDegrees)));
		float endXLeft = endLeftCoef * CosDegrees(endDegrees);
		float endYLeft = endLeftCoef * SinDegrees(endDegrees);
		float endZLeft = -halfWidth * SinDegrees(0.5f * endDegrees);

		float endRightCoef = (radius + (halfWidth * CosDegrees(0.5f * endDegrees)));
		float endXRight = endRightCoef * CosDegrees(endDegrees);
		float endYRight = endRightCoef * SinDegrees(endDegrees);
		float endZRight = halfWidth * SinDegrees(0.5f * endDegrees);

		Vec3 startLeft = Vec3(startXLeft, startYLeft, startZLeft) + center;
		Vec3 startRight = Vec3(startXRight, startYRight, startZRight) + center;
		Vec3 endLeft = Vec3(endXLeft, endYLeft, endZLeft) + center;
		Vec3 endRight = Vec3(endXRight, endYRight, endZRight) + center;

		AddVertsForQuad3D(verts, startLeft, startRight, endLeft, endRight, color);
		AddVertsForQuad3D(verts, startRight, startLeft, endRight, endLeft, color);
	}
}


void AddVertsForMobiusStrip3D(std::vector<Vertex_PCUTBN>& verts, Vec3 const& center, float radius, float halfWidth, int numSegments, Rgba8 const& color)
{
	float degreesPerSegment = 360.0f / static_cast<float>(numSegments);

	for (int segIndex = 0; segIndex < numSegments; segIndex++)
	{
		float startDegrees = static_cast<float>(segIndex) * degreesPerSegment;
		float endDegrees = startDegrees + degreesPerSegment;

		float startLeftCoef = (radius + (-halfWidth * CosDegrees(0.5f * startDegrees)));
		float startXLeft = startLeftCoef * CosDegrees(startDegrees);
		float startYLeft = startLeftCoef * SinDegrees(startDegrees);
		float startZLeft = -halfWidth * SinDegrees(0.5f * startDegrees);

		float startRightCoef = (radius + (halfWidth * CosDegrees(0.5f * startDegrees)));
		float startXRight = startRightCoef * CosDegrees(startDegrees);
		float startYRight = startRightCoef * SinDegrees(startDegrees);
		float startZRight = halfWidth * SinDegrees(0.5f * startDegrees);

		float endLeftCoef = (radius + (-halfWidth * CosDegrees(0.5f * endDegrees)));
		float endXLeft = endLeftCoef * CosDegrees(endDegrees);
		float endYLeft = endLeftCoef * SinDegrees(endDegrees);
		float endZLeft = -halfWidth * SinDegrees(0.5f * endDegrees);

		float endRightCoef = (radius + (halfWidth * CosDegrees(0.5f * endDegrees)));
		float endXRight = endRightCoef * CosDegrees(endDegrees);
		float endYRight = endRightCoef * SinDegrees(endDegrees);
		float endZRight = halfWidth * SinDegrees(0.5f * endDegrees);

		Vec3 startLeft = Vec3(startXLeft, startYLeft, startZLeft) + center;
		Vec3 startRight = Vec3(startXRight, startYRight, startZRight) + center;
		Vec3 endLeft = Vec3(endXLeft, endYLeft, endZLeft) + center;
		Vec3 endRight = Vec3(endXRight, endYRight, endZRight) + center;

		AddVertsForQuad3D(verts, startLeft, startRight, endLeft, endRight, color);
		AddVertsForQuad3D(verts, startRight, startLeft, endRight, endLeft, color);
	}
}
