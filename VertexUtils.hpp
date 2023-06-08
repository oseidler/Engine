#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/AABB2.hpp"


//forward declarations
struct Vec2;
struct Vec3;
struct AABB3;
struct Rgba8;
struct OBB2;
struct Vertex_PCU;
struct Vertex_PNCU;
struct Mat44;

//transform utilities
void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* vertArray, float scale, float rotationDegrees, Vec2 const& translation);
void TransformVertexArray3D(std::vector<Vertex_PCU>& verts, Mat44 const& transform);
void TransformVertexArray3D(std::vector<Vertex_PNCU>& verts, Mat44 const& transform);

//2D vertex adding functions
void AddVertsForAABB2(std::vector<Vertex_PCU>& verts, AABB2 const& aabb, Rgba8 const& color = Rgba8(), AABB2 const& uvs = AABB2(0.0f, 0.0f, 1.0f, 1.0f));	//#ToDo: rename to AABB2D without breaking other projects
void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& obb, Rgba8 const& color = Rgba8());
void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color = Rgba8(), int numEdges = 64);
void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, Vec2 const& startPos, Vec2 const& endPos, float thickness, Rgba8 const& color = Rgba8());
void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Vec2 const& boneStart, Vec2 const& boneEnd, float radius, Rgba8 const& color = Rgba8(), int numArcEdges = 32);
void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, Vec2 const& startPos, Vec2 const& endPos, float tipSize, float thickness, Rgba8 const& color = Rgba8());

//3D vertex adding functions
void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight, 
	Rgba8 const& color = Rgba8(255, 255, 255), AABB2 const& uvs = AABB2(0.0f, 0.0f, 1.0f, 1.0f));
void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight,
	Rgba8 const& color = Rgba8(255, 255, 255), AABB2 const& uvs = AABB2(0.0f, 0.0f, 1.0f, 1.0f));
void AddVertsForQuad3D(std::vector<Vertex_PNCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight,
	Rgba8 const& color = Rgba8(255, 255, 255), AABB2 const& uvs = AABB2(0.0f, 0.0f, 1.0f, 1.0f));
void AddVertsForQuad3D(std::vector<Vertex_PNCU>& verts, std::vector<unsigned int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight,
	Rgba8 const& color = Rgba8(255, 255, 255), AABB2 const& uvs = AABB2(0.0f, 0.0f, 1.0f, 1.0f));
void AddVertsForRoundedQuad3D(std::vector<Vertex_PNCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topLeft, Vec3 const& topRight,
	Rgba8 const& color = Rgba8(255, 255, 255), AABB2 const& uvs = AABB2(0.0f, 0.0f, 1.0f, 1.0f));
void AddVertsForCube3D(std::vector<Vertex_PCU>& verts, Vec3 const& forwardBottomLeft, Vec3 const& forwardBottomRight, Vec3 const& forwardTopLeft, Vec3 const& forwardTopRight,
	Vec3 const& backBottomLeft, Vec3 const& backBottomRight, Vec3 const& backTopLeft, Vec3 const& backTopRight, 
	Rgba8 const& colorForward, Rgba8 const& colorBack, Rgba8 const& colorLeft, Rgba8 const& colorRight, Rgba8 const& colorBottom, Rgba8 const& colorTop, 
	AABB2 const& uvs = AABB2(0.0f, 0.0f, 1.0f, 1.0f));
void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& bounds, Rgba8 const& color = Rgba8(255, 255, 255), AABB2 const& uvs = AABB2(0.0f, 0.0f, 1.0f, 1.0f));
void AddVertsForSphere3D(std::vector<Vertex_PCU>& verts, Vec3 const& center, float radius, int numSlices = 16, int numStacks = 8,
	Rgba8 const& color = Rgba8(255, 255, 255), AABB2 const& uvs = AABB2(0.0f, 0.0f, 1.0f, 1.0f));
void AddVertsForXYGrid3D(std::vector<Vertex_PCU>& verts, int numGridsX, int numGridsY, float gridSize, int sectionSize, 
	float gridLineHalfWidth, float sectionLineHalfWidth, float originLineHalfWidth, Rgba8 const& gridLineColor = Rgba8(192, 192, 192), Rgba8 const& sectionLineXColor = Rgba8(0, 192, 0), 
	Rgba8 const& sectionLineYColor = Rgba8(192, 0, 0), Rgba8 const& originLineXColor = Rgba8(0, 255, 0), Rgba8 const& originLineYColor = Rgba8(255, 0, 0));
void AddVertsForZCylinder3D(std::vector<Vertex_PCU>& verts, Vec3 const& baseCenter, float radius, float height, int numEdges = 16, Rgba8 const& color = Rgba8(), 
	AABB2 const& uvs = AABB2(0.0f, 0.0f, 1.0f, 1.0f));
void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, int numEdges = 16, Rgba8 const& color = Rgba8(255, 255, 255),
	AABB2 const& uvs = AABB2(0.0f, 0.0f, 1.0f, 1.0f));
void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, int numEdges = 16, Rgba8 const& color = Rgba8(255, 255, 255),
	AABB2 const& uvs = AABB2(0.0f, 0.0f, 1.0f, 1.0f));

//other vertex utilities
AABB2 GetVertexBounds2D(std::vector<Vertex_PCU> const& verts);
