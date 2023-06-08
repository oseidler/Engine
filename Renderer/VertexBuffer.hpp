#pragma once


//forward declarations
struct ID3D11Buffer;


class VertexBuffer
{
	friend class Renderer;

//public member functions
public:
	//constructors and destructor
	VertexBuffer(size_t size);
	VertexBuffer(size_t size, unsigned int stride);
	VertexBuffer(VertexBuffer const& copy) = delete;
	virtual ~VertexBuffer();

	//accessors
	unsigned int GetStride() const;

//public member variables
public:
	ID3D11Buffer* m_buffer = nullptr;
	size_t m_size = 0;
	unsigned int m_stride = 0;
};
