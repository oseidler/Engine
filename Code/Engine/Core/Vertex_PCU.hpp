#pragma once
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"


struct Vertex_PCU
{
//public member variables
public:
	Vec3  m_position;
	Rgba8 m_color;
	Vec2  m_uvTexCoords;

//public member functions
public:
	//constructors and destructor
	Vertex_PCU() {}
	~Vertex_PCU() {}
	explicit Vertex_PCU(Vec3 const& position, Rgba8 const& tint, Vec2 const& uvTexCoords);
	explicit Vertex_PCU(Vec2 const& position, Rgba8 const& tint, Vec2 const& uvTexCoords);
	explicit Vertex_PCU(Vec2 const& position, Rgba8 const& tint);
};
