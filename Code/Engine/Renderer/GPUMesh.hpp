#pragma once


//forward declarations
class VertexBuffer;
class IndexBuffer;


class GPUMesh
{
//public member functions
public:
	~GPUMesh();

//public member variables
public:
	VertexBuffer* m_vertexBuffer = nullptr;
	IndexBuffer*  m_indexBuffer = nullptr;
};
