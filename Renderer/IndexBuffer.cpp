#include "Engine/Renderer/IndexBuffer.hpp"
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
IndexBuffer::IndexBuffer(size_t size)
	: m_size(size)
{
}


IndexBuffer::~IndexBuffer()
{
	DX_SAFE_RELEASE(m_buffer);
}


//
//accessors
//
unsigned int IndexBuffer::GetStride() const
{
	return sizeof(Vertex_PCU);
}