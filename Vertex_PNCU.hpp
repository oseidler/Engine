#pragma once
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"


struct Vertex_PNCU
{
//public member variables
public:
	Vec3  m_position;
	Vec3  m_normal;
	Rgba8 m_color;
	Vec2  m_uvTexCoords;

//public member functions
public:
	//constructors and destructor
	Vertex_PNCU() {}
	~Vertex_PNCU() {}
	explicit Vertex_PNCU(Vec3 const& position, Vec3 const& normal, Rgba8 const& color, Vec2 const& uvTexCoords = Vec2(0.0f, 1.0f));
};
