#pragma once
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"


struct Vertex_PCUTBN
{
//public member variables
public:
	Vec3  m_position;
	Rgba8 m_color;
	Vec2  m_uvTexCoords;
	Vec3  m_tangent;
	Vec3  m_bitangent;
	Vec3  m_normal;

//public member functions
public:
	//constructors and destructor
	Vertex_PCUTBN() {}
	~Vertex_PCUTBN() {}
	explicit Vertex_PCUTBN(Vec3 const& position, Vec3 const& normal, Rgba8 const& color, Vec2 const& uvTexCoords = Vec2(0.0f, 1.0f), Vec3 const& tangent = Vec3(), Vec3 const& bitangent = Vec3());
	explicit Vertex_PCUTBN(Vec3 const& position, Rgba8 const& color, Vec2 const& uvTexCoords = Vec2(0.0f, 1.0f), Vec3 const& tangent = Vec3(), Vec3 const& bitangent = Vec3(), Vec3 const& normal = Vec3());
};
