#include "Engine/Core/Vertex_PCU.hpp"


//
//constructors
//
Vertex_PCU::Vertex_PCU(Vec3 const& position, Rgba8 const& tint, Vec2 const& uvTexCoords)
	: m_position(position)
	, m_color(tint)
	, m_uvTexCoords(uvTexCoords)
{
}


Vertex_PCU::Vertex_PCU(Vec2 const& position, Rgba8 const& tint, Vec2 const& uvTexCoords)
	: m_position(Vec3(position.x, position.y, 0.f))
	, m_color(tint)
	, m_uvTexCoords(uvTexCoords)
{
}


Vertex_PCU::Vertex_PCU(Vec2 const& position, Rgba8 const& tint)
	: m_position(Vec3(position.x, position.y, 0.f))
	, m_color(tint)
	, m_uvTexCoords(Vec2(0.f, 0.f))
{
}
