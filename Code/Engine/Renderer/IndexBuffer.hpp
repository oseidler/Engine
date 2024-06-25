#pragma once


//forward declarations
struct ID3D11Buffer;


class IndexBuffer
{
	friend class Renderer;

	//public member functions
public:
	//constructors and destructor
	IndexBuffer(size_t size);
	IndexBuffer(IndexBuffer const& copy) = delete;
	virtual ~IndexBuffer();

	//accessors
	unsigned int GetStride() const;

	//public member variables
public:
	ID3D11Buffer* m_buffer = nullptr;
	size_t m_size = 0;
};