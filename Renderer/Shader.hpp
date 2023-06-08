#pragma once
#include "Engine/Core/EngineCommon.hpp"


//forward declarations
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;


struct ShaderConfig
{
	std::string m_name;
	std::string m_vertexEntryPoint = "VertexMain";
	std::string m_pixelEntryPoint = "PixelMain";
};


class Shader
{
	friend class Renderer;

//public member functions
public:
	//constructors and destructor
	Shader(ShaderConfig const& config);
	Shader(Shader const& copy) = delete;
	~Shader();

	//accessors
	std::string const& GetName() const;

//public member variables
public:
	ShaderConfig m_config;
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;
};
