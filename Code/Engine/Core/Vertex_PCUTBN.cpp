#include "Engine/Core/Vertex_PCUTBN.hpp"


//
//constructors
//
Vertex_PCUTBN::Vertex_PCUTBN(Vec3 const& position, Vec3 const& normal, Rgba8 const& color, Vec2 const& uvTexCoords, Vec3 const& tangent, Vec3 const& bitangent)
	: m_position(position)
	, m_normal(normal)
	, m_color(color)
	, m_uvTexCoords(uvTexCoords)
	, m_tangent(tangent)
	, m_bitangent(bitangent)
{
}


Vertex_PCUTBN::Vertex_PCUTBN(Vec3 const& position, Rgba8 const& color, Vec2 const& uvTexCoords, Vec3 const& tangent, Vec3 const& bitangent, Vec3 const& normal)
	: m_position(position)
	, m_color(color)
	, m_uvTexCoords(uvTexCoords)
	, m_tangent(tangent)
	, m_bitangent(bitangent)
	, m_normal(normal)
{

}
