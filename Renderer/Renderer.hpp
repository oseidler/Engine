#pragma once
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/Vertex_PNCU.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Game/EngineBuildPreferences.hpp"

#define DX_SAFE_RELEASE(dxObject)	\
{									\
	if ((dxObject) != nullptr)		\
	{								\
		(dxObject)->Release();		\
		(dxObject) = nullptr;		\
	}								\
}	


//forward declarations
class Window;
class Texture;
class BitmapFont;
class Shader;
class VertexBuffer;
class ConstantBuffer;
class IndexBuffer;
class Image;

struct ID3D11RasterizerState;
struct ID3D11RenderTargetView;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11BlendState;
struct ID3D11SamplerState;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;


//render mode enums
enum class BlendMode
{
	ALPHA,
	ADDITIVE,
	OPAQUE,
	COUNT
};

enum class SamplerMode
{
	POINT_CLAMP,
	BILINEAR_WRAP,
	COUNT
};

enum class RasterizerMode
{
	SOLID_CULL_NONE,
	SOLID_CULL_BACK,
	WIREFRAME_CULL_NONE,
	WIREFRAME_CULL_BACK,
	COUNT
};

enum class DepthMode
{
	DISABLED,
	ENABLED,
	COUNT
};


//config struct
struct RendererConfig
{
	Window* m_window = nullptr;
};


class Renderer
{
//public member functions
public:
	Renderer(RendererConfig const& config);
	~Renderer() {}

	//game flow functions
	void Startup();
	void BeginFrame() const;
	void EndFrame() const;
	void Shutdown();

	//renderering functions
	void ClearScreen(Rgba8 const& clearColor) const;
	void BeginCamera(Camera const& camera);
	void EndCamera(Camera const& camera) const;
	void DrawVertexArray(int numVertexes, Vertex_PCU const* vertexes);
	void DrawVertexArray(std::vector<Vertex_PCU> const& verts);
	void DrawVertexArray(int numVertexes, Vertex_PNCU const* vertexes);
	//void CreateRenderContext();

	//state change functions
	void SetStatesIfChanged();
	void SetBlendMode(BlendMode blendMode);
	void SetSamplerMode(SamplerMode samplerMode);
	void SetRasterizerMode(RasterizerMode rasterizerMode);
	void SetDepthMode(DepthMode depthMode);

	//accessors
	RendererConfig const& GetConfig() const { return m_config; }

	//texture functions
	Texture* CreateOrGetTextureFromFile(char const* imageFilePath);
	void	 BindTexture(Texture* texture);

	//font functions
	BitmapFont* CreateOrGetBitmapFont(char const* bitmapFontFilePathNoExtension);

	//shader functions
	void	BindShader(Shader* shader);
	Shader* CreateShader(char const* shaderName, char const* source);
	Shader* CreateShader(char const* shaderName);
	bool	CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target);
	Shader* GetShaderForFileName(char const* shaderName);

	//vertex buffer functions
	VertexBuffer* CreateVertexBuffer(size_t const size, unsigned int stride = sizeof(Vertex_PCU));
	void		  CopyCPUToGPU(void const* data, size_t size, VertexBuffer*& vbo);
	void		  BindVertexBuffer(VertexBuffer* vbo);
	void		  DrawVertexBuffer(VertexBuffer* vbo, int vertexCount, int vertexOffset = 0);

	//index buffer functions
	IndexBuffer* CreateIndexBuffer(size_t const size);
	void		 CopyCPUToGPU(void const* data, size_t size, IndexBuffer*& vbo);
	void		 BindIndexBuffer(IndexBuffer* ibo);
	void		 DrawVertexBufferIndexed(VertexBuffer* vbo, IndexBuffer* ibo, int indexCount);

	//constant buffer functions
	ConstantBuffer* CreateConstantBuffer(size_t const size);
	void			CopyCPUToGPU(void const* data, size_t size, ConstantBuffer* cbo);
	void			BindConstantBuffer(int slot, ConstantBuffer* cbo);
	void			SetModelConstants(Mat44 const& modelMatrix = Mat44(), Rgba8 const& modelColor = Rgba8(255, 255, 255));
	void			SetLightConstants(Vec3 const& sunDirection, float sunIntensity, float ambientIntensity);

//private member functions
private:
	//texture functions
	Texture* GetTextureForFileName(char const* imageFilePath);
	Texture* CreateTextureFromFile(char const* imageFilePath);
	Texture* CreateTextureFromImage(Image const& image);
	Texture* CreateTextureFromData(char const* name, IntVec2 const& dimensions, int bytesPerTexel, unsigned char* texelData);

	//font functions
	BitmapFont* GetBitmapFontForFileName(char const* bitmapFontFilePathNoExtension);
	BitmapFont* CreateBitmapFontFromFile(char const* bitmapFontFilePathNoExtension);

//protected member variables
protected:
	RendererConfig m_config;

	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	ID3D11RasterizerState* m_rasterizerState = nullptr;
	ID3D11BlendState* m_blendState = nullptr;
	ID3D11SamplerState* m_samplerState = nullptr;

	void* m_dxgiDebug = nullptr;
	void* m_dxgiDebugModule = nullptr;

	std::vector<Texture*> m_loadedTextures;
	Texture const* m_defaultTexture = nullptr;

	std::vector<BitmapFont*> m_loadedFonts;

	std::vector<Shader*> m_loadedShaders;
	Shader const* m_currentShader = nullptr;
	Shader* m_defaultShader = nullptr;

	VertexBuffer* m_immediateVBO_PCU = nullptr;
	VertexBuffer* m_immediateVBO_PNCU = nullptr;

	ConstantBuffer* m_lightCBO = nullptr;
	ConstantBuffer* m_cameraCBO = nullptr;
	ConstantBuffer* m_modelCBO = nullptr;

	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11Texture2D* m_depthStencilTexture = nullptr;

	BlendMode m_desiredBlendMode = BlendMode::ALPHA;
	SamplerMode m_desiredSamplerMode = SamplerMode::POINT_CLAMP;
	RasterizerMode m_desiredRasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	DepthMode m_desiredDepthMode = DepthMode::ENABLED;

	ID3D11BlendState* m_blendStates[(int)BlendMode::COUNT] = {};
	ID3D11SamplerState* m_samplerStates[(int)SamplerMode::COUNT] = {};
	ID3D11RasterizerState* m_rasterizerStates[(int)RasterizerMode::COUNT] = {};
	ID3D11DepthStencilState* m_depthStencilStates[(int)DepthMode::COUNT] = {};
};
