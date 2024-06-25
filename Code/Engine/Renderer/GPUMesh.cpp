#include "Engine/Renderer/GPUMesh.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Renderer/IndexBuffer.hpp"



GPUMesh::~GPUMesh()
{
	if (m_vertexBuffer != nullptr)
	{
		delete m_vertexBuffer;
	}
	if (m_indexBuffer != nullptr)
	{
		delete m_indexBuffer;
	}
}
