#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")


//
//constructor and destructor
//
VertexBuffer::VertexBuffer(size_t size)
	: m_size(size)
{
	m_stride = sizeof(Vertex_PCU);
}


VertexBuffer::VertexBuffer(size_t size, unsigned int stride)
	: m_size(size)
	, m_stride(stride)
{
}


VertexBuffer::~VertexBuffer()
{
	DX_SAFE_RELEASE(m_buffer);
}


//
//accessors
//
unsigned int VertexBuffer::GetStride() const
{
	return m_stride;
}
