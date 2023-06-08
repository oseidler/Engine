#include "Engine/Renderer/Shader.hpp"
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
Shader::Shader(ShaderConfig const& config)
	: m_config(config)
{
}


Shader::~Shader()
{
	//release all dx11 objects
	DX_SAFE_RELEASE(m_vertexShader);
	DX_SAFE_RELEASE(m_pixelShader);
	DX_SAFE_RELEASE(m_inputLayout);
}


//
//accessors
//
std::string const& Shader::GetName() const
{
	return m_config.m_name;
}
