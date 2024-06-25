#pragma once


//forward declarations
struct ID3D11Buffer;


class ConstantBuffer
{
	friend class Renderer;

//public member functions
public:
	//constructors and destructor
	ConstantBuffer(size_t size);
	ConstantBuffer(ConstantBuffer const& copy) = delete;
	virtual ~ConstantBuffer();

//public member variables
public:
	ID3D11Buffer* m_buffer = nullptr;
	size_t m_size = 0;
};
