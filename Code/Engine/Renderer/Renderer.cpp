#define WIN32_LEAN_AND_MEAN
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Renderer/DefaultShader.hpp"
#include "Engine/Renderer/ConstantBuffer.hpp"
#include "Engine/Renderer/IndexBuffer.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Image.hpp"
#include "ThirdParty/stb/stb_image.h"
#include <algorithm>
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")
#if defined(ENGINE_DEBUG_RENDER)
	#include <dxgidebug.h>
	#pragma comment(lib, "dxguid.lib")
#endif
#if defined(OPAQUE)
	#undef OPAQUE
#endif
//#include <gl/gl.h>
//#pragma comment(lib, "opengl32")


//
//constant buffer structs
//
struct LightConstants
{
	Vec3  SunDirection;
	float SunIntensity;
	float AmbientIntensity;
	Vec3  WorldEyePosition;
	int   NormalMode;
	int	  SpecularMode;
	float SpecularIntensity;
	float SpecularPower;
};
static const int k_lightConstantsSlot = 1;


struct CameraConstants
{
	Mat44 ProjectionMatrix;
	Mat44 ViewMatrix;
};
static const int k_cameraConstantsSlot = 2;


struct ModelConstants
{
	Mat44 ModelMatrix;
	Vec4  ModelColor;
};
static const int k_modelConstantsSlot = 3;


//
//constructor
//
Renderer::Renderer(RendererConfig const& config)
	:m_config(config)
{
}


//
//game flow functions
//
void Renderer::Startup()
{
	#if defined(ENGINE_DEBUG_RENDER)
		m_dxgiDebugModule = (void*) ::LoadLibraryA("dxgidebug.dll");
		if (m_dxgiDebugModule == nullptr)
		{
			ERROR_AND_DIE("could not load dxgidebug.dll");
		}

		typedef HRESULT(WINAPI* GetDebugModuleCB)(REFIID, void**);
		((GetDebugModuleCB) ::GetProcAddress((HMODULE)m_dxgiDebugModule, "DXGIGetDebugInterface")) (__uuidof(IDXGIDebug), &m_dxgiDebug);

		if (m_dxgiDebug == nullptr)
		{
			ERROR_AND_DIE("Could not load debug module");
		}
	#endif
	
	//create device and swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferDesc.Width = m_config.m_window->GetClientDimensions().x;
	swapChainDesc.BufferDesc.Height = m_config.m_window->GetClientDimensions().y;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = static_cast<HWND>(m_config.m_window->GetHwnd());
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	UINT deviceFlags = 0;
	#if defined(ENGINE_DEBUG_RENDER)
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		deviceFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_swapChain,
		&m_device,
		nullptr,
		&m_deviceContext
	);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create D3D 11 device and swap chain.");
	}

	//get back buffer texture
	ID3D11Texture2D* backBuffer;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not get swap chain buffer.");
	}

	//set render target view
	hr = m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create render target view for swap chain buffer.");
	}

	backBuffer->Release();

	//create and bind shaders
	m_defaultShader = CreateShader("Default", g_defaultShaderSource);
	BindShader(nullptr);

	//set primitive topology to triangles
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//create immediate vertex buffer
	m_immediateVBO_PCU = CreateVertexBuffer(sizeof(Vertex_PCU), sizeof(Vertex_PCU));
	m_immediateVBO_PCUTBN = CreateVertexBuffer(sizeof(Vertex_PCUTBN), sizeof(Vertex_PCUTBN));

	//create rasterizer states
	D3D11_RASTERIZER_DESC rasterDescSolidBack = {};
	rasterDescSolidBack.FillMode = D3D11_FILL_SOLID;
	rasterDescSolidBack.CullMode = D3D11_CULL_BACK;
	rasterDescSolidBack.FrontCounterClockwise = true;
	rasterDescSolidBack.DepthClipEnable = true;
	rasterDescSolidBack.AntialiasedLineEnable = true;

	hr = m_device->CreateRasterizerState(&rasterDescSolidBack, &m_rasterizerStates[(int)RasterizerMode::SOLID_CULL_BACK]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create rasterizer state.");
	}

	D3D11_RASTERIZER_DESC rasterDescSolidNone = {};
	rasterDescSolidNone.FillMode = D3D11_FILL_SOLID;
	rasterDescSolidNone.CullMode = D3D11_CULL_NONE;
	rasterDescSolidNone.FrontCounterClockwise = true;
	rasterDescSolidNone.DepthClipEnable = true;
	rasterDescSolidNone.AntialiasedLineEnable = true;

	hr = m_device->CreateRasterizerState(&rasterDescSolidNone, &m_rasterizerStates[(int)RasterizerMode::SOLID_CULL_NONE]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create rasterizer state.");
	}

	D3D11_RASTERIZER_DESC rasterDescWireframeBack = {};
	rasterDescWireframeBack.FillMode = D3D11_FILL_WIREFRAME;
	rasterDescWireframeBack.CullMode = D3D11_CULL_BACK;
	rasterDescWireframeBack.FrontCounterClockwise = true;
	rasterDescWireframeBack.DepthClipEnable = true;
	rasterDescWireframeBack.AntialiasedLineEnable = true;

	hr = m_device->CreateRasterizerState(&rasterDescWireframeBack, &m_rasterizerStates[(int)RasterizerMode::WIREFRAME_CULL_BACK]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create rasterizer state.");
	}

	D3D11_RASTERIZER_DESC rasterDescWireframeNone = {};
	rasterDescWireframeNone.FillMode = D3D11_FILL_WIREFRAME;
	rasterDescWireframeNone.CullMode = D3D11_CULL_NONE;
	rasterDescWireframeNone.FrontCounterClockwise = true;
	rasterDescWireframeNone.DepthClipEnable = true;
	rasterDescWireframeNone.AntialiasedLineEnable = true;

	hr = m_device->CreateRasterizerState(&rasterDescWireframeNone, &m_rasterizerStates[(int)RasterizerMode::WIREFRAME_CULL_NONE]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create rasterizer state.");
	}

	//set initial rasterizer state
	m_rasterizerState = m_rasterizerStates[(int)m_desiredRasterizerMode];
	m_deviceContext->RSSetState(m_rasterizerState);

	//create constant buffers
	m_cameraCBO = CreateConstantBuffer(sizeof(CameraConstants));
	m_modelCBO = CreateConstantBuffer(sizeof(ModelConstants));
	m_lightCBO = CreateConstantBuffer(sizeof(LightConstants));

	//initialize and bind default texture
	Image defaultTextureImage = Image(IntVec2(2, 2), Rgba8(255, 255, 255));
	m_defaultTexture = CreateTextureFromImage(defaultTextureImage);
	BindTexture(nullptr);

	//create sampler states
	D3D11_SAMPLER_DESC samplerStatePointClamp = {};
	samplerStatePointClamp.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerStatePointClamp.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerStatePointClamp.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerStatePointClamp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerStatePointClamp.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerStatePointClamp.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_device->CreateSamplerState(&samplerStatePointClamp, &m_samplerStates[(int)SamplerMode::POINT_CLAMP]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create sampler state.");
	}

	D3D11_SAMPLER_DESC samplerStatePointWrap = {};
	samplerStatePointWrap.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerStatePointWrap.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStatePointWrap.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStatePointWrap.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStatePointWrap.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerStatePointWrap.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_device->CreateSamplerState(&samplerStatePointWrap, &m_samplerStates[(int)SamplerMode::POINT_WRAP]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create sampler state.");
	}

	D3D11_SAMPLER_DESC samplerStateBilinearWrap = {};
	samplerStateBilinearWrap.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStateBilinearWrap.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateBilinearWrap.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateBilinearWrap.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateBilinearWrap.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerStateBilinearWrap.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_device->CreateSamplerState(&samplerStateBilinearWrap, &m_samplerStates[(int)SamplerMode::BILINEAR_WRAP]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create sampler state.");
	}

	D3D11_SAMPLER_DESC samplerStateBilinearClamp = {};
	samplerStateBilinearClamp.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStateBilinearClamp.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerStateBilinearClamp.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerStateBilinearClamp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerStateBilinearClamp.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerStateBilinearClamp.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_device->CreateSamplerState(&samplerStateBilinearClamp, &m_samplerStates[(int)SamplerMode::BILINEAR_CLAMP]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create sampler state.");
	}

	//set initial sampler state
	m_samplerState = m_samplerStates[(int)m_desiredSamplerMode];
	m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);

	//create blend states
	D3D11_BLEND_DESC blendStateOpaque = { 0 };
	blendStateOpaque.RenderTarget[0].BlendEnable = true;
	blendStateOpaque.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateOpaque.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendStateOpaque.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateOpaque.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateOpaque.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendStateOpaque.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateOpaque.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = m_device->CreateBlendState(&blendStateOpaque, &m_blendStates[(int)BlendMode::OPAQUE]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create blend state.");
	}

	D3D11_BLEND_DESC blendStateAlpha = { 0 };
	blendStateAlpha.RenderTarget[0].BlendEnable = true;
	blendStateAlpha.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateAlpha.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateAlpha.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateAlpha.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateAlpha.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendStateAlpha.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateAlpha.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = m_device->CreateBlendState(&blendStateAlpha, &m_blendStates[(int)BlendMode::ALPHA]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create blend state.");
	}

	D3D11_BLEND_DESC blendStateAdditive = { 0 };
	blendStateAdditive.RenderTarget[0].BlendEnable = true;
	blendStateAdditive.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateAdditive.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateAdditive.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateAdditive.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateAdditive.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendStateAdditive.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateAdditive.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = m_device->CreateBlendState(&blendStateAdditive, &m_blendStates[(int)BlendMode::ADDITIVE]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create blend state.");
	}

	//set initial blend state
	m_blendState = m_blendStates[(int)m_desiredBlendMode];
	float zeroArray[4] = { 0 };
	m_deviceContext->OMSetBlendState(m_blendState, zeroArray, 0xffffffff);

	//create depth texture
	D3D11_TEXTURE2D_DESC depthTextureDesc = {};
	depthTextureDesc.Width = m_config.m_window->GetClientDimensions().x;
	depthTextureDesc.Height = m_config.m_window->GetClientDimensions().y;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.SampleDesc.Count = 1;

	hr = m_device->CreateTexture2D(&depthTextureDesc, nullptr, &m_depthStencilTexture);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create depth texture.");
	}

	hr = m_device->CreateDepthStencilView(m_depthStencilTexture, nullptr, &m_depthStencilView);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create depth stencil view.");
	}

	//create depth stencil states
	D3D11_DEPTH_STENCIL_DESC depthStencilEnabled = {};
	depthStencilEnabled.DepthEnable = true;
	depthStencilEnabled.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilEnabled.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = m_device->CreateDepthStencilState(&depthStencilEnabled, &m_depthStencilStates[(int)DepthMode::ENABLED]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create depth stencil state.");
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDisabled = {};
	depthStencilDisabled.DepthEnable = true;
	depthStencilDisabled.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDisabled.DepthFunc = D3D11_COMPARISON_ALWAYS;

	hr = m_device->CreateDepthStencilState(&depthStencilDisabled, &m_depthStencilStates[(int)DepthMode::DISABLED]);
	
	//set initial depth stencil state
	m_depthStencilState = m_depthStencilStates[(int)m_desiredDepthMode];
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);


	//old OpenGL code (preserved just in case)
	
	//CreateRenderContext();
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
}


void Renderer::BeginFrame() const
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}


void Renderer::EndFrame() const
{
	HRESULT hr = m_swapChain->Present(0, 0);

	//error handling for present
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		ERROR_AND_DIE("Device was lost, program will exit.");
	}
	

	//old OpenGL code (preserved just in case)
	
	//SwapBuffers(displayDeviceContext); // Note: call this only once at the very end of each frame
}


void Renderer::Shutdown()
{
	//release dx11 objects
	if (m_immediateVBO_PCU != nullptr)
	{
		delete m_immediateVBO_PCU;
		m_immediateVBO_PCU = nullptr;
	}

	if (m_immediateVBO_PCUTBN != nullptr)
	{
		delete m_immediateVBO_PCUTBN;
		m_immediateVBO_PCUTBN = nullptr;
	}

	if (m_lightCBO != nullptr)
	{
		delete m_lightCBO;
		m_lightCBO = nullptr;
	}

	if (m_cameraCBO != nullptr)
	{
		delete m_cameraCBO;
		m_cameraCBO = nullptr;
	}

	if (m_modelCBO != nullptr)
	{
		delete m_modelCBO;
		m_modelCBO = nullptr;
	}

	for (int shaderIndex = 0; shaderIndex < m_loadedShaders.size(); shaderIndex++)
	{
		if (m_loadedShaders[shaderIndex] != nullptr)
		{
			delete m_loadedShaders[shaderIndex];
			m_loadedShaders[shaderIndex] = nullptr;
		}
	}

	for (int textureIndex = 0; textureIndex < m_loadedTextures.size(); textureIndex++)
	{
		if (m_loadedTextures[textureIndex] != nullptr)
		{
			delete m_loadedTextures[textureIndex];
			m_loadedTextures[textureIndex] = nullptr;
		}
	}

	for (int stateIndex = 0; stateIndex < (int)BlendMode::COUNT; stateIndex++)
	{
		DX_SAFE_RELEASE(m_blendStates[stateIndex]);
	}

	for (int stateIndex = 0; stateIndex < (int)SamplerMode::COUNT; stateIndex++)
	{
		DX_SAFE_RELEASE(m_samplerStates[stateIndex]);
	}

	for (int stateIndex = 0; stateIndex < (int)RasterizerMode::COUNT; stateIndex++)
	{
		DX_SAFE_RELEASE(m_rasterizerStates[stateIndex]);
	}

	for (int stateIndex = 0; stateIndex < (int)DepthMode::COUNT; stateIndex++)
	{
		DX_SAFE_RELEASE(m_depthStencilStates[stateIndex]);
	}
	
	DX_SAFE_RELEASE(m_depthStencilTexture);
	DX_SAFE_RELEASE(m_depthStencilView);
	//DX_SAFE_RELEASE(m_depthStencilState);
	//DX_SAFE_RELEASE(m_samplerState);
	//DX_SAFE_RELEASE(m_blendState);
	//DX_SAFE_RELEASE(m_rasterizerState);
	DX_SAFE_RELEASE(m_renderTargetView);
	DX_SAFE_RELEASE(m_swapChain);
	DX_SAFE_RELEASE(m_deviceContext);
	DX_SAFE_RELEASE(m_device);

	#if defined(ENGINE_DEBUG_RENDER)
		((IDXGIDebug*)m_dxgiDebug)->ReportLiveObjects(DXGI_DEBUG_ALL, (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));

		((IDXGIDebug*)m_dxgiDebug)->Release();
		m_dxgiDebug = nullptr;

		::FreeLibrary((HMODULE)m_dxgiDebugModule);
		m_dxgiDebugModule = nullptr;
	#endif
}


//
//renderer functions
//
void Renderer::ClearScreen(Rgba8 const& clearColor) const
{
	//get background color as array of floats
	float clearColorAsFloats[4];
	clearColor.GetAsFloats(clearColorAsFloats);

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, clearColorAsFloats);

	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	

	//old OpenGL code (preserved just in case)

	//float red = ((float)clearColor.r) / 255.f;		//convert the unsigned chars into floats with range [0,1] so glClearColor can read them
	//float green = ((float)clearColor.g) / 255.f;
	//float blue = ((float)clearColor.b) / 255.f;
	//float alpha = ((float)clearColor.a) / 255.f;
	//glClearColor(red, green, blue, alpha);		//set background color
	//glClear(GL_COLOR_BUFFER_BIT);					//clear the screen
}


void Renderer::BeginCamera(Camera const& camera)
{
	//set camera constants
	CameraConstants cameraConstants;
	cameraConstants.ProjectionMatrix = camera.GetProjectionMatrix();
	cameraConstants.ViewMatrix = camera.GetViewMatrix();

	//copy constants to constant buffer
	CopyCPUToGPU(&cameraConstants, sizeof(cameraConstants), m_cameraCBO);
	BindConstantBuffer(k_cameraConstantsSlot, m_cameraCBO);
	
	//set viewport
	D3D11_VIEWPORT viewport = { 0 };
	Vec2 cameraViewportOrigin = camera.GetViewportOrigin();
	Vec2 cameraViewportWidthHeight = camera.GetViewportWidthHeight();
	//negative ones used to represent that whole window should be used as viewport
	if (cameraViewportOrigin == Vec2(-1.0f, -1.0f) || cameraViewportWidthHeight == Vec2(-1.0f, -1.0f))
	{
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(m_config.m_window->GetClientDimensions().x);
		viewport.Height = static_cast<float>(m_config.m_window->GetClientDimensions().y);
	}
	else
	{
		viewport.TopLeftX = cameraViewportOrigin.x;
		viewport.TopLeftY = cameraViewportOrigin.y;
		viewport.Width = cameraViewportWidthHeight.x;
		viewport.Height = cameraViewportWidthHeight.y;
	}
	
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	m_deviceContext->RSSetViewports(1, &viewport);
	

	//old OpenGL code (preserved just in case)
	
	//glLoadIdentity();
	////get camera bounds
	//Vec2 const bottomLeft = camera.GetOrthoBottomLeft();
	//Vec2 const topRight = camera.GetOrthoTopRight();

	//glOrtho(bottomLeft.x, topRight.x, bottomLeft.y, topRight.y, 0.f, 1.f); //set bounds to bounds of the camera that is passed in
}


void Renderer::EndCamera(Camera const& camera) const
{
	UNUSED(camera);
}


void Renderer::DrawVertexArray(int numVertexes, Vertex_PCU const* vertexes)
{
	CopyCPUToGPU(vertexes, numVertexes * sizeof(Vertex_PCU), m_immediateVBO_PCU);

	DrawVertexBuffer(m_immediateVBO_PCU, numVertexes);
	

	//old OpenGL code (preserved just in case)
	
	//glBegin(GL_TRIANGLES);
	//{
	//	for (int vertexIndex = 0; vertexIndex < numVertexes; vertexIndex++)	//draw each point in the vertex array in order
	//	{
	//		Vertex_PCU const& vert = vertexes[vertexIndex];
	//		glColor4ub(vert.m_color.r, vert.m_color.g, vert.m_color.b, vert.m_color.a);
	//		glTexCoord2f(vert.m_uvTexCoords.x, vert.m_uvTexCoords.y);
	//		glVertex3f(vert.m_position.x, vert.m_position.y, vert.m_position.z);
	//	}
	//}
	//glEnd();
}


void Renderer::DrawVertexArray(int numVertexes, Vertex_PCUTBN const* vertexes)
{
	CopyCPUToGPU(vertexes, numVertexes * sizeof(Vertex_PCUTBN), m_immediateVBO_PCUTBN);

	DrawVertexBuffer(m_immediateVBO_PCUTBN, numVertexes);
}


void Renderer::DrawVertexArray(std::vector<Vertex_PCU> const& verts)
{
	DrawVertexArray(static_cast<int>(verts.size()), verts.data());
}


void Renderer::DrawVertexArray(std::vector<Vertex_PCUTBN> const& verts)
{
	DrawVertexArray(static_cast<int>(verts.size()), verts.data());
}


//old OpenGL code (preserved just in case)

//void Renderer::CreateRenderContext()
//{
//	// Creates an OpenGL rendering context (RC) and binds it to the current window's device context (DC)
//	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
//	memset(&pixelFormatDescriptor, 0, sizeof(pixelFormatDescriptor));
//	pixelFormatDescriptor.nSize = sizeof(pixelFormatDescriptor);
//	pixelFormatDescriptor.nVersion = 1;
//	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
//	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
//	pixelFormatDescriptor.cColorBits = 24;
//	pixelFormatDescriptor.cDepthBits = 24;
//	pixelFormatDescriptor.cAccumBits = 0;
//	pixelFormatDescriptor.cStencilBits = 8;
//
//	HWND windowHandle = ::GetActiveWindow();
//	displayDeviceContext = ::GetDC(windowHandle);
//	GUARANTEE_OR_DIE(displayDeviceContext != nullptr, "Cannot create a render context without a display context!");
//
//	int pixelFormatCode = ChoosePixelFormat(displayDeviceContext, &pixelFormatDescriptor);
//	SetPixelFormat(displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor);
//	HGLRC rc = wglCreateContext(displayDeviceContext);
//	wglMakeCurrent(displayDeviceContext, rc);
//}


//
//public state change functions
//
void Renderer::SetStatesIfChanged()
{
	//check if blend mode changed
	auto blendItr = std::find(m_blendStates, m_blendStates + (int)BlendMode::COUNT, m_blendState);

	if (blendItr == std::end(m_blendStates))
	{
		ERROR_AND_DIE("Couldn't find current blend state in list of states.");
	}

	int blendModeIndex = (int)std::distance(m_blendStates, blendItr);

	if ((int)m_desiredBlendMode != blendModeIndex)
	{
		m_blendState = m_blendStates[(int)m_desiredBlendMode];
		float zeroArray[4] = { 0 };
		m_deviceContext->OMSetBlendState(m_blendState, zeroArray, 0xffffffff);
	}

	//check if sampler mode changed
	auto samplerItr = std::find(m_samplerStates, m_samplerStates + (int)SamplerMode::COUNT, m_samplerState);

	if (samplerItr == std::end(m_samplerStates))
	{
		ERROR_AND_DIE("Couldn't find current sampler state in list of states.");
	}

	int samplerModeIndex = (int)std::distance(m_samplerStates, samplerItr);

	if ((int)m_desiredSamplerMode != samplerModeIndex)
	{
		m_samplerState = m_samplerStates[(int)m_desiredSamplerMode];
		m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	}

	//check if rasterizer mode changed
	auto rasterItr = std::find(m_rasterizerStates, m_rasterizerStates + (int)RasterizerMode::COUNT, m_rasterizerState);

	if (rasterItr == std::end(m_rasterizerStates))
	{
		ERROR_AND_DIE("Couldn't find current rasterizer state in list of states.");
	}

	int rasterModeIndex = (int)std::distance(m_rasterizerStates, rasterItr);

	if ((int)m_desiredRasterizerMode != rasterModeIndex)
	{
		m_rasterizerState = m_rasterizerStates[(int)m_desiredRasterizerMode];
		m_deviceContext->RSSetState(m_rasterizerState);
	}

	//check if depth mode changed
	auto depthItr = std::find(m_depthStencilStates, m_depthStencilStates + (int)DepthMode::COUNT, m_depthStencilState);

	if (depthItr == std::end(m_depthStencilStates))
	{
		ERROR_AND_DIE("Couldn't find current depth state in list of states.");
	}

	int depthModeIndex = (int)std::distance(m_depthStencilStates, depthItr);

	if ((int)m_desiredDepthMode != depthModeIndex)
	{
		m_depthStencilState = m_depthStencilStates[(int)m_desiredDepthMode];
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);
	}
}


void Renderer::SetBlendMode(BlendMode blendMode)
{
	m_desiredBlendMode = blendMode;

	//old OpenGL code (preserved just in case)
	
	/*if (blendMode == BlendMode::ALPHA)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else if (blendMode == BlendMode::ADDITIVE)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	else
	{
		ERROR_AND_DIE(Stringf("Unknown / unsupported blend mode #%i", blendMode));
	}*/
}


void Renderer::SetSamplerMode(SamplerMode samplerMode)
{
	m_desiredSamplerMode = samplerMode;
}


void Renderer::SetRasterizerMode(RasterizerMode rasterizerMode)
{
	m_desiredRasterizerMode = rasterizerMode;
}


void Renderer::SetDepthMode(DepthMode depthMode)
{
	m_desiredDepthMode = depthMode;
}


void Renderer::SetPrimitiveTopology(PrimitiveTopology topology)
{
	if (topology == PrimitiveTopology::TRIANGLES)
	{
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	else if (topology == PrimitiveTopology::LINES)
	{
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	}
}


//
//texture functions
//
Texture* Renderer::CreateOrGetTextureFromFile(char const* imageFilePath)
{
	// See if we already have this texture previously loaded
	Texture* existingTexture = GetTextureForFileName(imageFilePath);
	if (existingTexture)
	{
		return existingTexture;
	}

	// Never seen this texture before!  Let's load it.
	Texture* newTexture = CreateTextureFromFile(imageFilePath);
	return newTexture;
}


Texture* Renderer::GetTextureForFileName(char const* imageFilePath)
{
	for (int textureIndex = 0; textureIndex < m_loadedTextures.size(); textureIndex++)
	{
		if (m_loadedTextures[textureIndex] != nullptr)
		{
			std::string fileName = m_loadedTextures[textureIndex]->GetImageFilePath();

			if (fileName == imageFilePath)
			{
				return m_loadedTextures[textureIndex];
			}
		}
	}

	return nullptr;
}


Texture* Renderer::CreateTextureFromFile(char const* imageFilePath)
{
	Image imageFile = Image(imageFilePath);
	
	Texture* newTexture = CreateTextureFromImage(imageFile);

	return newTexture;

	
	//old code
	//IntVec2 dimensions = IntVec2(0, 0);		// This will be filled in for us to indicate image width & height
	//int bytesPerTexel = 0; // This will be filled in for us to indicate how many color components the image had (e.g. 3=RGB=24bit, 4=RGBA=32bit)
	//int numComponentsRequested = 0; // don't care; we support 3 (24-bit RGB) or 4 (32-bit RGBA)

	//// Load (and decompress) the image RGB(A) bytes from a file on disk into a memory buffer (array of bytes)
	//stbi_set_flip_vertically_on_load(1); // We prefer uvTexCoords has origin (0,0) at BOTTOM LEFT
	//unsigned char* texelData = stbi_load(imageFilePath, &dimensions.x, &dimensions.y, &bytesPerTexel, numComponentsRequested);

	//// Check if the load was successful
	//GUARANTEE_OR_DIE(texelData, Stringf("Failed to load image \"%s\"", imageFilePath));

	//Texture* newTexture = CreateTextureFromData(imageFilePath, dimensions, bytesPerTexel, texelData);

	// Free the raw image texel data now that we've sent a copy of it down to the GPU to be stored in video memory
	//stbi_image_free(texelData);

	//return newTexture;
}


Texture* Renderer::CreateTextureFromImage(Image const& image)
{
	Texture* newTexture = new Texture();
	newTexture->m_name = image.GetImageFilePath(); // NOTE: m_name must be a std::string, otherwise it may point to temporary data!
	newTexture->m_dimensions = image.GetImageDimensions();

	//create texture description
	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = image.GetImageDimensions().x;
	textureDesc.Height = image.GetImageDimensions().y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.SampleDesc.Count = 1;

	//create resource
	D3D11_SUBRESOURCE_DATA resource = { 0 };
	resource.pSysMem = image.GetRawData();
	resource.SysMemPitch = image.GetImageDimensions().x * sizeof(Rgba8);

	//create texture
	HRESULT hr = m_device->CreateTexture2D(&textureDesc, &resource, &newTexture->m_texture);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create texture");
	}

	//create shader resource view
	if (newTexture->m_texture == nullptr)
	{
		ERROR_AND_DIE("Error: Texture pointer was null when creating shader resource view");
	}
	hr = m_device->CreateShaderResourceView(newTexture->m_texture, NULL, &newTexture->m_shaderResourceView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create shader resource view for texture");
	}

	m_loadedTextures.push_back(newTexture);
	return newTexture;
}


Texture* Renderer::CreateTextureFromData(char const* name, IntVec2 const& dimensions, int bytesPerTexel, unsigned char* texelData)
{
	// Check if the load was successful
	GUARANTEE_OR_DIE(texelData, Stringf("CreateTextureFromData failed for \"%s\" - texelData was null!", name));
	GUARANTEE_OR_DIE(bytesPerTexel >= 3 && bytesPerTexel <= 4, Stringf("CreateTextureFromData failed for \"%s\" - unsupported BPP=%i (must be 3 or 4)", name, bytesPerTexel));
	GUARANTEE_OR_DIE(dimensions.x > 0 && dimensions.y > 0, Stringf("CreateTextureFromData failed for \"%s\" - illegal texture dimensions (%i x %i)", name, dimensions.x, dimensions.y));

	Texture* newTexture = new Texture();
	newTexture->m_name = name; // NOTE: m_name must be a std::string, otherwise it may point to temporary data!
	newTexture->m_dimensions = dimensions;


	//old OpenGL code (preserved just in case)

	//// Enable OpenGL texturing
	//glEnable(GL_TEXTURE_2D);

	//// Tell OpenGL that our pixel data is single-byte aligned
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//// Ask OpenGL for an unused texName (ID number) to use for this texture
	//glGenTextures(1, (GLuint*)&newTexture->m_openglTextureID);

	//// Tell OpenGL to bind (set) this as the currently active texture
	//glBindTexture(GL_TEXTURE_2D, newTexture->m_openglTextureID);

	//// Set texture clamp vs. wrap (repeat) default settings
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_CLAMP or GL_REPEAT
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_CLAMP or GL_REPEAT

	//// Set magnification (texel > pixel) and minification (texel < pixel) filters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // one of: GL_NEAREST, GL_LINEAR
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR

	//// Pick the appropriate OpenGL format (RGB or RGBA) for this texel data
	//GLenum bufferFormat = GL_RGBA; // the format our source pixel data is in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	//if (bytesPerTexel == 3)
	//{
	//	bufferFormat = GL_RGB;
	//}
	//GLenum internalFormat = bufferFormat; // the format we want the texture to be on the card; technically allows us to translate into a different texture format as we upload to OpenGL

	//// Upload the image texel data (raw pixels bytes) to OpenGL under this textureID
	//glTexImage2D(			// Upload this pixel data to our new OpenGL texture
	//	GL_TEXTURE_2D,		// Creating this as a 2d texture
	//	0,					// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
	//	internalFormat,		// Type of texel format we want OpenGL to use for this texture internally on the video card
	//	dimensions.x,		// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,11], and B is the border thickness [0,1]
	//	dimensions.y,		// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,11], and B is the border thickness [0,1]
	//	0,					// Border size, in texels (must be 0 or 1, recommend 0)
	//	bufferFormat,		// Pixel format describing the composition of the pixel data in buffer
	//	GL_UNSIGNED_BYTE,	// Pixel color components are unsigned bytes (one byte per color channel/component)
	//	texelData);		// Address of the actual pixel data bytes/buffer in system memory

	m_loadedTextures.push_back(newTexture);
	return newTexture;
}


void Renderer::BindTexture(Texture* texture)
{
	//bind default texture if nullptr is passed
	if (texture == nullptr)
	{
		m_deviceContext->PSSetShaderResources(0, 1, &m_defaultTexture->m_shaderResourceView);
	}
	//otherwise bind passed texture
	else
	{
		m_deviceContext->PSSetShaderResources(0, 1, &texture->m_shaderResourceView);
	}


	//old OpenGL code (preserved just in case)

	/*if (texture)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture->m_openglTextureID);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}*/
}


void Renderer::BindTextures(Texture const* texture0, Texture const* texture1, Texture const* texture2)
{
	//bind default texture if nullptr is passed
	if (texture0 == nullptr)
	{
		m_deviceContext->PSSetShaderResources(0, 1, &m_defaultTexture->m_shaderResourceView);
	}
	//otherwise bind passed texture
	else
	{
		m_deviceContext->PSSetShaderResources(0, 1, &texture0->m_shaderResourceView);
	}

	if (texture1 == nullptr)
	{
		m_deviceContext->PSSetShaderResources(1, 1, &m_defaultTexture->m_shaderResourceView);
	}
	else
	{
		m_deviceContext->PSSetShaderResources(1, 1, &texture1->m_shaderResourceView);
	}

	if (texture2 == nullptr)
	{
		m_deviceContext->PSSetShaderResources(2, 1, &m_defaultTexture->m_shaderResourceView);
	}
	else
	{
		m_deviceContext->PSSetShaderResources(2, 1, &texture2->m_shaderResourceView);
	}
}


//
//font functions
//
BitmapFont* Renderer::CreateOrGetBitmapFont(char const* bitmapFontFilePathNoExtension)
{
	// See if we already have this texture previously loaded
	BitmapFont* existingFont = GetBitmapFontForFileName(bitmapFontFilePathNoExtension);
	if (existingFont)
	{
		return existingFont;
	}

	// Never seen this texture before!  Let's load it.
	BitmapFont* newFont = CreateBitmapFontFromFile(bitmapFontFilePathNoExtension);
	return newFont;
}


BitmapFont* Renderer::GetBitmapFontForFileName(char const* bitmapFontFilePathNoExtension)
{
	for (int fontIndex = 0; fontIndex < m_loadedFonts.size(); fontIndex++)
	{
		if (m_loadedFonts[fontIndex] != nullptr)
		{
			std::string fileName = m_loadedFonts[fontIndex]->m_fontFilePathNameNoExtension;

			if (fileName == bitmapFontFilePathNoExtension)
			{
				return m_loadedFonts[fontIndex];
			}
		}
	}

	return nullptr;
}


BitmapFont* Renderer::CreateBitmapFontFromFile(char const* bitmapFontFilePathNoExtension)
{
	//in future versions, this will not just assume that the file is a .png
	std::string extension = ".png";
	std::string filePathWithExtension = bitmapFontFilePathNoExtension + extension;
	Texture* fontTexture = CreateOrGetTextureFromFile(filePathWithExtension.c_str());
	
	return new BitmapFont(bitmapFontFilePathNoExtension, *fontTexture);
}


//
//shader functions
//
void Renderer::BindShader(Shader* shader)
{
	if (shader == nullptr)
	{
		if (m_defaultShader == nullptr)
		{
			ERROR_AND_DIE("Default shader was null!");
		}
		shader = m_defaultShader;
	}
	
	m_deviceContext->VSSetShader(shader->m_vertexShader, NULL, 0);

	m_deviceContext->PSSetShader(shader->m_pixelShader, NULL, 0);

	m_deviceContext->IASetInputLayout(shader->m_inputLayout);

	m_currentShader = shader;
}


Shader* Renderer::CreateShader(char const* shaderName, char const* source, VertexType vertexType)
{
	Shader* existingShader = GetShaderForFileName(shaderName);
	if (existingShader != nullptr)
	{
		return existingShader;
	}
	
	ShaderConfig config;
	config.m_name = shaderName;

	Shader* shader = new Shader(config);

	std::vector<uint8_t> m_vertexShaderByteCode;
	std::vector<uint8_t> m_pixelShaderByteCode;

	//compile vertex shader
	CompileShaderToByteCode(m_vertexShaderByteCode, shaderName, source, shader->m_config.m_vertexEntryPoint.c_str(), "vs_5_0");

	//create vertex shader
	HRESULT hr = m_device->CreateVertexShader(
		m_vertexShaderByteCode.data(),
		m_vertexShaderByteCode.size(),
		nullptr,
		&shader->m_vertexShader
	);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create vertex shader.");
	}

	//compile pixel shader
	CompileShaderToByteCode(m_pixelShaderByteCode, shaderName, source, shader->m_config.m_pixelEntryPoint.c_str(), "ps_5_0");

	//create pixel shader
	hr = m_device->CreatePixelShader(
		m_pixelShaderByteCode.data(),
		m_pixelShaderByteCode.size(),
		nullptr,
		&shader->m_pixelShader
	);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create pixel shader.");
	}

	//create input layout
	if (vertexType == VertexType::VertexPCU)
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		hr = m_device->CreateInputLayout(
			inputElementDesc,
			3,
			m_vertexShaderByteCode.data(),
			m_vertexShaderByteCode.size(),
			&shader->m_inputLayout
		);

		if (!SUCCEEDED(hr))
		{
			ERROR_AND_DIE("Could not create input layout");
		}
	}
	else if (vertexType == VertexType::VertexPCUTBN)
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		hr = m_device->CreateInputLayout(
			inputElementDesc,
			6,
			m_vertexShaderByteCode.data(),
			m_vertexShaderByteCode.size(),
			&shader->m_inputLayout
		);

		if (!SUCCEEDED(hr))
		{
			ERROR_AND_DIE("Could not create input layout");
		}
	}
	else
	{
		std::string sourceAsString = source;
		if (sourceAsString.find("NORMAL") != std::string::npos)
		{
			D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			hr = m_device->CreateInputLayout(
				inputElementDesc,
				6,
				m_vertexShaderByteCode.data(),
				m_vertexShaderByteCode.size(),
				&shader->m_inputLayout
			);

			if (!SUCCEEDED(hr))
			{
				ERROR_AND_DIE("Could not create input layout");
			}
		}
		else
		{
			D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			hr = m_device->CreateInputLayout(
				inputElementDesc,
				3,
				m_vertexShaderByteCode.data(),
				m_vertexShaderByteCode.size(),
				&shader->m_inputLayout
			);

			if (!SUCCEEDED(hr))
			{
				ERROR_AND_DIE("Could not create input layout");
			}
		}
	}

	m_loadedShaders.push_back(shader);
	
	return shader;
}


Shader* Renderer::CreateShader(char const* shaderName, VertexType vertexType)
{
	Shader* existingShader = GetShaderForFileName(shaderName);
	if (existingShader != nullptr)
	{
		return existingShader;
	}
	
	std::string extension = ".hlsl";
	std::string shaderFileName = shaderName + extension;
	
	//file io
	std::string shaderSourceFromFile;
	FileReadToString(shaderSourceFromFile, shaderFileName);

	return CreateShader(shaderName, shaderSourceFromFile.c_str(), vertexType);
}


bool Renderer::CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target)
{
	//shader debugging
	DWORD flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
	#if defined(ENGINE_DEBUG_RENDER)
		flags = D3DCOMPILE_DEBUG;
		flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
		flags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
	#endif

	//compile vertex shader
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompile(
		source,
		strlen(source),
		name,
		NULL,
		NULL,
		entryPoint,
		target,
		flags,
		0,
		&shaderBlob,
		&errorBlob
	);

	if (!SUCCEEDED(hr))
	{
		//check error blob
		if (errorBlob != nullptr)
		{
			DebuggerPrintf((char const*)errorBlob->GetBufferPointer());
		}
		else
		{
			DebuggerPrintf("No error blob contents to print.");
		}
		
		ERROR_AND_DIE("Could not compile shader.");
	}

	//memcopy blob to out byte code
	outByteCode.resize(shaderBlob->GetBufferSize());
	memcpy(outByteCode.data(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());

	DX_SAFE_RELEASE(shaderBlob);
	DX_SAFE_RELEASE(errorBlob);
	
	return true;
}


Shader* Renderer::GetShaderForFileName(char const* shaderName)
{
	for (int shaderIndex = 0; shaderIndex < m_loadedShaders.size(); shaderIndex++)
	{
		if (m_loadedShaders[shaderIndex] != nullptr)
		{
			std::string fileName = m_loadedShaders[shaderIndex]->GetName();

			if (fileName == shaderName)
			{
				return m_loadedShaders[shaderIndex];
			}
		}
	}

	return nullptr;
}


//
//vertex buffer functions
//
VertexBuffer* Renderer::CreateVertexBuffer(size_t const size, unsigned int stride)
{
	VertexBuffer* tempBuffer = new VertexBuffer(size, stride);
	
	//create vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = static_cast<UINT>(size);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&vertexBufferDesc, NULL, &tempBuffer->m_buffer);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create vertex buffer.");
	}
	
	return tempBuffer;
}


void Renderer::CopyCPUToGPU(void const* data, size_t size, VertexBuffer*& vbo)
{
	//resize buffer
	if (vbo->m_size < size)
	{
		VertexBuffer* temp = vbo;
		vbo = CreateVertexBuffer(size, vbo->m_stride);
		delete temp;
	}
	
	//copy vertex buffer data from CPU to GPU
	D3D11_MAPPED_SUBRESOURCE resource = { 0 };

	m_deviceContext->Map(
		(ID3D11Resource*)vbo->m_buffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&resource
	);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap((ID3D11Resource*)vbo->m_buffer, 0);
}


void Renderer::BindVertexBuffer(VertexBuffer* vbo)
{
	UINT const zeroPointer = 0;

	m_deviceContext->IASetVertexBuffers(
		0,
		1,
		&vbo->m_buffer,
		&vbo->m_stride,
		&zeroPointer
	);
}


void Renderer::DrawVertexBuffer(VertexBuffer* vbo, int vertexCount, int vertexOffset)
{
	BindVertexBuffer(vbo);

	SetStatesIfChanged();

	m_deviceContext->Draw(vertexCount, vertexOffset);
}


//
//index buffer functions
//
IndexBuffer* Renderer::CreateIndexBuffer(size_t const size)
{
	IndexBuffer* tempBuffer = new IndexBuffer(size);

	//create vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = static_cast<UINT>(size);
	vertexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&vertexBufferDesc, NULL, &tempBuffer->m_buffer);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create index buffer.");
	}

	return tempBuffer;
}


void Renderer::CopyCPUToGPU(void const* data, size_t size, IndexBuffer*& ibo)
{
	//resize buffer
	if (ibo->m_size < size)
	{
		IndexBuffer* temp = ibo;
		ibo = CreateIndexBuffer(size);
		delete temp;
	}

	//copy vertex buffer data from CPU to GPU
	D3D11_MAPPED_SUBRESOURCE resource = { 0 };

	m_deviceContext->Map(
		(ID3D11Resource*)ibo->m_buffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&resource
	);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap((ID3D11Resource*)ibo->m_buffer, 0);
}


void Renderer::BindIndexBuffer(IndexBuffer* ibo)
{
	m_deviceContext->IASetIndexBuffer(ibo->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}


void Renderer::DrawVertexBufferIndexed(VertexBuffer* vbo, IndexBuffer* ibo, int indexCount)
{
	BindVertexBuffer(vbo);
	BindIndexBuffer(ibo);

	SetStatesIfChanged();

	m_deviceContext->DrawIndexed(indexCount, 0, 0);
}


//
//constant buffer functions
//
ConstantBuffer* Renderer::CreateConstantBuffer(size_t const size)
{
	ConstantBuffer* tempBuffer = new ConstantBuffer(size);

	//create vertex buffer
	D3D11_BUFFER_DESC constantBufferDesc = { 0 };
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = static_cast<UINT>(size);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&constantBufferDesc, NULL, &tempBuffer->m_buffer);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create constant buffer.");
	}

	return tempBuffer;
}


void Renderer::CopyCPUToGPU(void const* data, size_t size, ConstantBuffer* cbo)
{
	//copy vertex buffer data from CPU to GPU
	D3D11_MAPPED_SUBRESOURCE resource = { 0 };

	m_deviceContext->Map(
		(ID3D11Resource*)cbo->m_buffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&resource
	);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap((ID3D11Resource*)cbo->m_buffer, 0);
}


void Renderer::BindConstantBuffer(int slot, ConstantBuffer* cbo)
{
	m_deviceContext->VSSetConstantBuffers(slot, 1, &cbo->m_buffer);

	m_deviceContext->PSSetConstantBuffers(slot, 1, &cbo->m_buffer);
}


void Renderer::SetModelConstants(Mat44 const& modelMatrix, Rgba8 const& modelColor)
{
	//set model constants
	ModelConstants modelConstants;
	modelConstants.ModelMatrix = modelMatrix;
	modelColor.GetAsFloats(&modelConstants.ModelColor.x);

	//copy constants to constant buffer
	CopyCPUToGPU(&modelConstants, sizeof(modelConstants), m_modelCBO);
	BindConstantBuffer(k_modelConstantsSlot, m_modelCBO);
}


void Renderer::SetLightConstants(Vec3 const& sunDirection, float sunIntensity, float ambientIntensity)
{
	LightConstants lightConstants;
	lightConstants.SunDirection = sunDirection.GetNormalized();
	lightConstants.SunIntensity = GetClamped(sunIntensity, 0.0f, 1.0f);
	lightConstants.AmbientIntensity = GetClamped(ambientIntensity, 0.0f, 1.0f);

	CopyCPUToGPU(&lightConstants, sizeof(lightConstants), m_lightCBO);
	BindConstantBuffer(k_lightConstantsSlot, m_lightCBO);
}


void Renderer::SetLightConstants(Vec3 const& sunDirection, float sunIntensity, float ambientIntensity, Vec3 worldEyePosition, int normalMode, int specularMode, float specularIntensity, float specularPower)
{
	LightConstants lightConstants;
	lightConstants.SunDirection = sunDirection.GetNormalized();
	lightConstants.SunIntensity = GetClamped(sunIntensity, 0.0f, 1.0f);
	lightConstants.AmbientIntensity = GetClamped(ambientIntensity, 0.0f, 1.0f);
	lightConstants.WorldEyePosition = worldEyePosition;
	lightConstants.NormalMode = normalMode;
	lightConstants.SpecularMode = specularMode;
	lightConstants.SpecularIntensity = specularIntensity;
	lightConstants.SpecularPower = specularPower;

	CopyCPUToGPU(&lightConstants, sizeof(lightConstants), m_lightCBO);
	BindConstantBuffer(k_lightConstantsSlot, m_lightCBO);
}
