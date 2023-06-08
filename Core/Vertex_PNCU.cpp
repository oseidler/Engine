#include "Engine/Core/Vertex_PNCU.hpp"


//
//constructors
//
Vertex_PNCU::Vertex_PNCU(Vec3 const& position, Vec3 const& normal, Rgba8 const& color, Vec2 const& uvTexCoords)
	: m_position(position)
	, m_normal(normal)
	, m_color(color)
	, m_uvTexCoords(uvTexCoords)
{
}
