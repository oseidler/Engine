#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <mutex>


//Apologies that this is so inefficient, I'll make it better if I have time later

//forward declarations
class DebugRenderWorldObject;
class DebugRenderScreenObject;
class DebugRenderMessage;

//constant declarations
constexpr float MESSAGE_SIZE = 16.0f;


//debug render system class
class DebugRenderSystem
{
//public member variables
public:
	DebugRenderConfig m_config;
	std::atomic<bool> m_isHidden = false;

	std::vector<DebugRenderWorldObject*> m_debugWorldObjects;
	std::mutex							 m_debugWorldObjectsMutex;

	std::vector<DebugRenderScreenObject*> m_debugScreenObjects;
	std::mutex							  m_debugScreenObjectsMutex;

	std::vector<DebugRenderMessage*> m_debugMessages;
	std::mutex					     m_debugMessagesMutex;

	BitmapFont* m_debugFont = nullptr;
	Camera const* m_worldCamera = nullptr;
	Camera const* m_screenCamera = nullptr;
};

DebugRenderSystem s_debugRender;


//abstract world object class
class DebugRenderWorldObject
{
//public member functions
public:
	virtual ~DebugRenderWorldObject() {}

	virtual void AddVertsForDebugObject(std::vector<Vertex_PCU>& verts) = 0;
	virtual void AddVertsForXRayObject(std::vector<Vertex_PCU>& verts) = 0;

//public member variables
public:
	float m_duration;
	float m_maxDuration;
	Rgba8 m_startColor;
	Rgba8 m_endColor;
	Rgba8 m_currentColor;
	DebugRenderMode m_mode;
	bool m_isWireframe;
	bool m_isBillboard = false;
	bool m_isText = false;
};


//world object subclasses
class DebugRenderSphere : public DebugRenderWorldObject
{
//public member functions
public:
	virtual void AddVertsForDebugObject(std::vector<Vertex_PCU>& verts) override;
	virtual void AddVertsForXRayObject(std::vector<Vertex_PCU>& verts) override;

//public member variables
public:
	Vec3 m_position;
	float m_radius;
};


void DebugRenderSphere::AddVertsForDebugObject(std::vector<Vertex_PCU>& verts)
{
	AddVertsForSphere3D(verts, m_position, m_radius, 16, 8, m_currentColor);
}


void DebugRenderSphere::AddVertsForXRayObject(std::vector<Vertex_PCU>& verts)
{
	unsigned char r = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.r + 120), 0, 255));
	unsigned char g = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.g + 120), 0, 255));
	unsigned char b = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.b + 120), 0, 255));
	unsigned char a = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.a - 80), 0, 255));

	AddVertsForSphere3D(verts, m_position, m_radius, 16, 8, Rgba8(r, g, b, a));
}


class DebugRenderArrow : public DebugRenderWorldObject
{
//public member functions
public:
	virtual void AddVertsForDebugObject(std::vector<Vertex_PCU>& verts) override;
	virtual void AddVertsForXRayObject(std::vector<Vertex_PCU>& verts) override;

//public member variables
public:
	Vec3 m_start;
	Vec3 m_end;
	float m_radius;
};


void DebugRenderArrow::AddVertsForDebugObject(std::vector<Vertex_PCU>& verts)
{
	Vec3 iBasis = (m_end - m_start).GetNormalized();

	AddVertsForCylinder3D(verts, m_start, m_end - iBasis * 0.25f, m_radius, 12, m_currentColor);
	
	AddVertsForCone3D(verts, m_end - iBasis * 0.25f, m_end, m_radius + 0.05f, 12, m_currentColor);
}


void DebugRenderArrow::AddVertsForXRayObject(std::vector<Vertex_PCU>& verts)
{
	unsigned char r = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.r + 120), 0, 255));
	unsigned char g = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.g + 120), 0, 255));
	unsigned char b = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.b + 120), 0, 255));
	unsigned char a = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.a - 80), 0, 255));

	Vec3 iBasis = (m_end - m_start).GetNormalized();

	AddVertsForCylinder3D(verts, m_start, m_end - iBasis * 0.25f, m_radius, 12, Rgba8(r, g, b, a));

	AddVertsForCone3D(verts, m_end - iBasis * 0.25f, m_end, m_radius + 0.05f, 12, Rgba8(r, g, b, a));
}


class DebugRenderCylinder : public DebugRenderWorldObject
{
//public member functions
public:
	virtual void AddVertsForDebugObject(std::vector<Vertex_PCU>& verts) override;
	virtual void AddVertsForXRayObject(std::vector<Vertex_PCU>& verts) override;

//public member variables
public:
	Vec3 m_base;
	Vec3 m_top;
	float m_radius;
};


void DebugRenderCylinder::AddVertsForDebugObject(std::vector<Vertex_PCU>& verts)
{
	AddVertsForCylinder3D(verts, m_base, m_top, m_radius, 12, m_currentColor);
}


void DebugRenderCylinder::AddVertsForXRayObject(std::vector<Vertex_PCU>& verts)
{
	unsigned char r = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.r + 120), 0, 255));
	unsigned char g = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.g + 120), 0, 255));
	unsigned char b = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.b + 120), 0, 255));
	unsigned char a = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.a - 80), 0, 255));

	AddVertsForCylinder3D(verts, m_base, m_top, m_radius, 12, Rgba8(r, g, b, a));
}


class DebugRenderWorldText : public DebugRenderWorldObject
{
//public member functions
public:
	virtual void AddVertsForDebugObject(std::vector<Vertex_PCU>& verts) override;
	virtual void AddVertsForXRayObject(std::vector<Vertex_PCU>& verts) override;

//public member variables
public:
	std::string m_text;
	Mat44 m_transform;
	float m_textHeight;
	Vec2 m_alignment;
};


void DebugRenderWorldText::AddVertsForDebugObject(std::vector<Vertex_PCU>& verts)
{
	std::vector<Vertex_PCU> textVerts;
	
	s_debugRender.m_debugFont->AddVertsForText3D(textVerts, Vec2(0.0f, 0.0f), m_textHeight, m_text, m_currentColor, 1.0f, m_alignment);

	TransformVertexArray3D(textVerts, m_transform);

	verts.insert(verts.begin(), textVerts.begin(), textVerts.end());
}


void DebugRenderWorldText::AddVertsForXRayObject(std::vector<Vertex_PCU>& verts)
{
	unsigned char r = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.r + 120), 0, 255));
	unsigned char g = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.g + 120), 0, 255));
	unsigned char b = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.b + 120), 0, 255));
	unsigned char a = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.a - 80), 0, 255));

	std::vector<Vertex_PCU> textVerts;

	s_debugRender.m_debugFont->AddVertsForText3D(textVerts, Vec2(0.0f, 0.0f), m_textHeight, m_text, Rgba8(r, g, b, a), 1.0f, m_alignment);

	TransformVertexArray3D(textVerts, m_transform);

	verts.insert(verts.begin(), textVerts.begin(), textVerts.end());
}


class DebugRenderBillboardText : public DebugRenderWorldObject
{
//public member functions
public:
	virtual void AddVertsForDebugObject(std::vector<Vertex_PCU>& verts) override;
	virtual void AddVertsForXRayObject(std::vector<Vertex_PCU>& verts) override;

//public member variables
public:
	std::string m_text;
	Vec3 m_origin;
	float m_textHeight;
	Vec2 m_alignment;
};


void DebugRenderBillboardText::AddVertsForDebugObject(std::vector<Vertex_PCU>& verts)
{
	std::vector<Vertex_PCU> textVerts;

	s_debugRender.m_debugFont->AddVertsForText3D(textVerts, Vec2(0.0f, 0.0f), m_textHeight, m_text, m_currentColor, 1.0f, m_alignment);

	Mat44 cameraMatrix = s_debugRender.m_worldCamera->GetViewMatrix().GetOrthonormalInverse();

	Mat44 billboardMatrix = GetBillboardMatrix(BillboardType::FULL_CAMERA_OPPOSING, cameraMatrix, Vec3());

	TransformVertexArray3D(textVerts, billboardMatrix);

	Mat44 translation = Mat44::CreateTranslation3D(m_origin);

	TransformVertexArray3D(textVerts, translation);

	verts.insert(verts.begin(), textVerts.begin(), textVerts.end());
}


void DebugRenderBillboardText::AddVertsForXRayObject(std::vector<Vertex_PCU>& verts)
{
	unsigned char r = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.r + 120), 0, 255));
	unsigned char g = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.g + 120), 0, 255));
	unsigned char b = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.b + 120), 0, 255));
	unsigned char a = static_cast<unsigned char>(GetClamped(static_cast<int>(m_currentColor.a - 80), 0, 255));

	std::vector<Vertex_PCU> textVerts;

	s_debugRender.m_debugFont->AddVertsForText3D(textVerts, Vec2(0.0f, 0.0f), m_textHeight, m_text, Rgba8(r, g, b, a), 1.0f, m_alignment);

	Mat44 cameraMatrix = s_debugRender.m_worldCamera->GetViewMatrix().GetOrthonormalInverse();

	Mat44 billboardMatrix = GetBillboardMatrix(BillboardType::FULL_CAMERA_OPPOSING, cameraMatrix, Vec3());

	TransformVertexArray3D(textVerts, billboardMatrix);

	Mat44 translation = Mat44::CreateTranslation3D(m_origin);

	TransformVertexArray3D(textVerts, translation);

	verts.insert(verts.begin(), textVerts.begin(), textVerts.end());
}


//abstract screen object class
class DebugRenderScreenObject
{
//public member functions
public:
	virtual ~DebugRenderScreenObject() {}

	virtual void AddVertsForDebugObject(std::vector<Vertex_PCU>& verts) = 0;

//public member variables
public:
	float m_duration;
	float m_maxDuration;
	Rgba8 m_startColor;
	Rgba8 m_endColor;
	Rgba8 m_currentColor;
};


//screen object subclasses
class DebugRenderScreenText : public DebugRenderScreenObject
{
//public member functions
public:
	virtual void AddVertsForDebugObject(std::vector<Vertex_PCU>& verts) override;

//public member variables
public:
	std::string m_text;
	Vec2 m_position;
	float m_size;
	Vec2 m_alignment;
};


void DebugRenderScreenText::AddVertsForDebugObject(std::vector<Vertex_PCU>& verts)
{
	std::vector<Vertex_PCU> textVerts;

	s_debugRender.m_debugFont->AddVertsForTextInBox2D(textVerts, AABB2(), m_size, m_text, m_currentColor, 1.0f, m_alignment, TextBoxMode::OVERRUN);

	Mat44 textTranslation = Mat44::CreateTranslation2D(m_position);

	TransformVertexArray3D(textVerts, textTranslation);

	verts.insert(verts.begin(), textVerts.begin(), textVerts.end());
}


class DebugRenderMessage : public DebugRenderScreenObject
{
//public member functions
public:
	virtual void AddVertsForDebugObject(std::vector<Vertex_PCU>& verts) override;

//public member variables
public:
	std::string m_text;
	int m_line;
};


void DebugRenderMessage::AddVertsForDebugObject(std::vector<Vertex_PCU>& verts)
{
	std::vector<Vertex_PCU> textVerts;

	s_debugRender.m_debugFont->AddVertsForTextInBox2D(textVerts, AABB2(), MESSAGE_SIZE, m_text, m_currentColor, 1.0f, Vec2(0.0f, 1.0f), TextBoxMode::OVERRUN);

	Vec2 linePosition = Vec2(0.0f, s_debugRender.m_screenCamera->GetOrthoTopRight().y - MESSAGE_SIZE * m_line);

	Mat44 textTranslation = Mat44::CreateTranslation2D(linePosition);

	TransformVertexArray3D(textVerts, textTranslation);

	verts.insert(verts.begin(), textVerts.begin(), textVerts.end());
}


//
//setup functions
//
void DebugRenderSystemStartup(DebugRenderConfig const& config)
{
	s_debugRender.m_config = config;

	SubscribeEventCallbackFunction("debugrenderclear", Command_DebugRenderClear);
	SubscribeEventCallbackFunction("debugrendertoggle", Command_DebugRenderToggle);

	s_debugRender.m_debugFont = s_debugRender.m_config.m_renderer->CreateOrGetBitmapFont("Data/Fonts/SquirrelFixedFont");
}


void DebugRenderSystemShutdown()
{
	s_debugRender.m_debugWorldObjectsMutex.lock();
	for (int objectIndex = 0; objectIndex < s_debugRender.m_debugWorldObjects.size(); objectIndex++)
	{
		DebugRenderWorldObject*& object = s_debugRender.m_debugWorldObjects[objectIndex];

		if (object != nullptr)
		{
			delete object;
			object = nullptr;
		}
	}
	s_debugRender.m_debugWorldObjectsMutex.unlock();

	s_debugRender.m_debugScreenObjectsMutex.lock();
	for (int objectIndex = 0; objectIndex < s_debugRender.m_debugScreenObjects.size(); objectIndex++)
	{
		DebugRenderScreenObject*& object = s_debugRender.m_debugScreenObjects[objectIndex];

		if (object != nullptr)
		{
			delete object;
			object = nullptr;
		}
	}
	s_debugRender.m_debugScreenObjectsMutex.unlock();

	s_debugRender.m_debugMessagesMutex.lock();
	for (int objectIndex = 0; objectIndex < s_debugRender.m_debugMessages.size(); objectIndex++)
	{
		DebugRenderMessage*& object = s_debugRender.m_debugMessages[objectIndex];

		if (object != nullptr)
		{
			delete object;
			object = nullptr;
		}
	}
	s_debugRender.m_debugMessagesMutex.unlock();
}


//
//options functions
//
void DebugRenderSetVisible()
{
	s_debugRender.m_isHidden = false;
}


void DebugRenderSetHidden()
{
	s_debugRender.m_isHidden = true;
}


void DebugRenderClear()
{
	s_debugRender.m_debugWorldObjectsMutex.lock();
	s_debugRender.m_debugWorldObjects.clear();
	s_debugRender.m_debugWorldObjectsMutex.unlock();

	s_debugRender.m_debugScreenObjectsMutex.lock();
	s_debugRender.m_debugScreenObjects.clear();
	s_debugRender.m_debugScreenObjectsMutex.unlock();

	s_debugRender.m_debugMessagesMutex.lock();
	s_debugRender.m_debugMessages.clear();
	s_debugRender.m_debugMessagesMutex.unlock();
}


//
//frame flow functions
//
void DebugRenderBeginFrame()
{
}


void DebugRenderWorld(Camera const& camera)
{
	if (s_debugRender.m_isHidden)
	{
		return;
	}
	
	Renderer*& renderer = s_debugRender.m_config.m_renderer;
	
	renderer->BeginCamera(camera);

	s_debugRender.m_worldCamera = &camera;

	//assign object to verts list based on rendering mode
	std::vector<Vertex_PCU> solidDepthVerts;
	std::vector<Vertex_PCU> wireframeDepthVerts;
	std::vector<Vertex_PCU> solidAlwaysVerts;
	std::vector<Vertex_PCU> wireframeAlwaysVerts;
	std::vector<Vertex_PCU> solidXrayVerts;
	std::vector<Vertex_PCU> wireframeXrayVerts;
	std::vector<Vertex_PCU> textDepthVerts;
	std::vector<Vertex_PCU> textAlwaysVerts;
	std::vector<Vertex_PCU> textXrayVerts;

	s_debugRender.m_debugWorldObjectsMutex.lock();
	for (int objectIndex = 0; objectIndex < s_debugRender.m_debugWorldObjects.size(); objectIndex++)
	{
		DebugRenderWorldObject*& object = s_debugRender.m_debugWorldObjects[objectIndex];

		if (object != nullptr)
		{
			if (object->m_isWireframe)
			{
				if (object->m_mode == DebugRenderMode::ALWAYS)
				{
					object->AddVertsForDebugObject(wireframeAlwaysVerts);
				}
				else if (object->m_mode == DebugRenderMode::X_RAY)
				{
					object->AddVertsForXRayObject(wireframeXrayVerts);
					object->AddVertsForDebugObject(wireframeDepthVerts);
				}
				else
				{
					object->AddVertsForDebugObject(wireframeDepthVerts);
				}
			}
			else
			{
				if (object->m_isText)
				{
					if (object->m_mode == DebugRenderMode::ALWAYS)
					{
						object->AddVertsForDebugObject(textAlwaysVerts);
					}
					else if (object->m_mode == DebugRenderMode::X_RAY)
					{
						object->AddVertsForXRayObject(textXrayVerts);
						object->AddVertsForDebugObject(textDepthVerts);
					}
					else
					{
						object->AddVertsForDebugObject(textDepthVerts);
					}
				}
				else
				{
					if (object->m_mode == DebugRenderMode::ALWAYS)
					{
						object->AddVertsForDebugObject(solidAlwaysVerts);
					}
					else if (object->m_mode == DebugRenderMode::X_RAY)
					{
						object->AddVertsForXRayObject(solidXrayVerts);
						object->AddVertsForDebugObject(solidDepthVerts);
					}
					else
					{
						object->AddVertsForDebugObject(solidDepthVerts);
					}
				}
			}
		}
	}
	s_debugRender.m_debugWorldObjectsMutex.unlock();

	renderer->SetModelConstants();
	renderer->BindShader(nullptr);

	//do draw calls for xray first
	renderer->SetDepthMode(DepthMode::DISABLED);

	renderer->BindTexture(nullptr);
	renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_BACK);
	renderer->SetBlendMode(BlendMode::ALPHA);
	renderer->DrawVertexArray(static_cast<int>(solidXrayVerts.size()), solidXrayVerts.data());

	renderer->BindTexture(nullptr);
	renderer->SetRasterizerMode(RasterizerMode::WIREFRAME_CULL_BACK);
	renderer->DrawVertexArray(static_cast<int>(wireframeXrayVerts.size()), wireframeXrayVerts.data());

	renderer->BindTexture(&s_debugRender.m_debugFont->GetTexture());
	renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	renderer->DrawVertexArray(static_cast<int>(textXrayVerts.size()), textXrayVerts.data());

	//do then a draw call for all depth verts
	renderer->SetDepthMode(DepthMode::ENABLED);
	renderer->BindTexture(nullptr);
	renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_BACK);
	renderer->SetBlendMode(BlendMode::OPAQUE);
	renderer->DrawVertexArray(static_cast<int>(solidDepthVerts.size()), solidDepthVerts.data());

	renderer->BindTexture(nullptr);
	renderer->SetRasterizerMode(RasterizerMode::WIREFRAME_CULL_BACK);
	renderer->DrawVertexArray(static_cast<int>(wireframeDepthVerts.size()), wireframeDepthVerts.data());

	renderer->BindTexture(&s_debugRender.m_debugFont->GetTexture());
	renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	renderer->SetBlendMode(BlendMode::ALPHA);
	renderer->DrawVertexArray(static_cast<int>(textDepthVerts.size()), textDepthVerts.data());

	//then all always verts
	renderer->SetDepthMode(DepthMode::DISABLED);

	renderer->BindTexture(nullptr);
	renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_BACK);
	renderer->DrawVertexArray(static_cast<int>(solidAlwaysVerts.size()), solidAlwaysVerts.data());

	renderer->BindTexture(nullptr);
	renderer->SetRasterizerMode(RasterizerMode::WIREFRAME_CULL_BACK);
	renderer->DrawVertexArray(static_cast<int>(wireframeAlwaysVerts.size()), wireframeAlwaysVerts.data());

	renderer->BindTexture(&s_debugRender.m_debugFont->GetTexture());
	renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	renderer->DrawVertexArray(static_cast<int>(textAlwaysVerts.size()), textAlwaysVerts.data());

	renderer->SetDepthMode(DepthMode::ENABLED);

	renderer->EndCamera(camera);
}


void DebugRenderScreen(Camera const& camera)
{
	if (s_debugRender.m_isHidden)
	{
		return;
	}
	
	Renderer*& renderer = s_debugRender.m_config.m_renderer;

	renderer->BeginCamera(camera);

	s_debugRender.m_screenCamera = &camera;

	std::vector<Vertex_PCU> verts;

	s_debugRender.m_debugScreenObjectsMutex.lock();
	for (int objectIndex = 0; objectIndex < s_debugRender.m_debugScreenObjects.size(); objectIndex++)
	{
		DebugRenderScreenObject*& object = s_debugRender.m_debugScreenObjects[objectIndex];

		if (object != nullptr)
		{
			object->AddVertsForDebugObject(verts);
		}
	}
	s_debugRender.m_debugScreenObjectsMutex.unlock();

	//do infinite and zero duration ones first, then others underneath
	int lineNumber = 0;

	s_debugRender.m_debugMessagesMutex.lock();
	for (int messageIndex = 0; messageIndex < s_debugRender.m_debugMessages.size(); messageIndex++)
	{
		DebugRenderMessage*& message = s_debugRender.m_debugMessages[messageIndex];

		if (message != nullptr && message->m_maxDuration <= 0.0f)
		{
			message->m_line = lineNumber;
			message->AddVertsForDebugObject(verts);
			lineNumber++;
		}
	}
	for (int messageIndex = 0; messageIndex < s_debugRender.m_debugMessages.size(); messageIndex++)
	{
		DebugRenderMessage*& message = s_debugRender.m_debugMessages[messageIndex];

		if (message != nullptr && message->m_maxDuration > 0.0f)
		{
			message->m_line = lineNumber;
			message->AddVertsForDebugObject(verts);
			lineNumber++;
		}
	}
	s_debugRender.m_debugMessagesMutex.unlock();

	renderer->BindShader(nullptr);
	renderer->BindTexture(&s_debugRender.m_debugFont->GetTexture());
	renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_BACK);
	renderer->SetBlendMode(BlendMode::ALPHA);
	renderer->DrawVertexArray(static_cast<int>(verts.size()), verts.data());

	renderer->EndCamera(camera);
}


void DebugRenderEndFrame()
{
	float deltaSeconds = Clock::GetSystemClock().GetDeltaSeconds();
	
	s_debugRender.m_debugWorldObjectsMutex.lock();
	for (int objectIndex = 0; objectIndex < s_debugRender.m_debugWorldObjects.size(); objectIndex++)
	{
		DebugRenderWorldObject*& object = s_debugRender.m_debugWorldObjects[objectIndex];

		if (object != nullptr)
		{
			//change color
			object->m_currentColor.r = static_cast<unsigned char>(RangeMap(object->m_duration, 0.0f, object->m_maxDuration, static_cast<float>(object->m_endColor.r), static_cast<float>(object->m_startColor.r)));
			object->m_currentColor.g = static_cast<unsigned char>(RangeMap(object->m_duration, 0.0f, object->m_maxDuration, static_cast<float>(object->m_endColor.g), static_cast<float>(object->m_startColor.g)));
			object->m_currentColor.b = static_cast<unsigned char>(RangeMap(object->m_duration, 0.0f, object->m_maxDuration, static_cast<float>(object->m_endColor.b), static_cast<float>(object->m_startColor.b)));
			object->m_currentColor.a = static_cast<unsigned char>(RangeMap(object->m_duration, 0.0f, object->m_maxDuration, static_cast<float>(object->m_endColor.a), static_cast<float>(object->m_startColor.a)));

			if (object->m_duration >= 0.0f)
			{
				object->m_duration -= deltaSeconds;

				if (object->m_duration <= 0.0f)
				{
					delete object;
					object = nullptr;
				}
			}
		}
	}
	s_debugRender.m_debugWorldObjectsMutex.unlock();

	s_debugRender.m_debugScreenObjectsMutex.lock();
	for (int objectIndex = 0; objectIndex < s_debugRender.m_debugScreenObjects.size(); objectIndex++)
	{
		DebugRenderScreenObject*& object = s_debugRender.m_debugScreenObjects[objectIndex];

		if (object != nullptr)
		{
			//change color
			object->m_currentColor.r = static_cast<unsigned char>(RangeMap(object->m_duration, 0.0f, object->m_maxDuration, static_cast<float>(object->m_endColor.r), static_cast<float>(object->m_startColor.r)));
			object->m_currentColor.g = static_cast<unsigned char>(RangeMap(object->m_duration, 0.0f, object->m_maxDuration, static_cast<float>(object->m_endColor.g), static_cast<float>(object->m_startColor.g)));
			object->m_currentColor.b = static_cast<unsigned char>(RangeMap(object->m_duration, 0.0f, object->m_maxDuration, static_cast<float>(object->m_endColor.b), static_cast<float>(object->m_startColor.b)));
			object->m_currentColor.a = static_cast<unsigned char>(RangeMap(object->m_duration, 0.0f, object->m_maxDuration, static_cast<float>(object->m_endColor.a), static_cast<float>(object->m_startColor.a)));

			if (object->m_duration >= 0.0f)
			{
				object->m_duration -= deltaSeconds;

				if (object->m_duration <= 0.0f)
				{
					delete object;
					object = nullptr;
				}
			}
		}
	}
	s_debugRender.m_debugScreenObjectsMutex.unlock();

	s_debugRender.m_debugMessagesMutex.lock();
	for (int objectIndex = 0; objectIndex < s_debugRender.m_debugMessages.size(); objectIndex++)
	{
		DebugRenderMessage*& object = s_debugRender.m_debugMessages[objectIndex];

		if (object != nullptr)
		{
			//change color
			object->m_currentColor.r = static_cast<unsigned char>(RangeMap(object->m_duration, 0.0f, object->m_maxDuration, static_cast<float>(object->m_endColor.r), static_cast<float>(object->m_startColor.r)));
			object->m_currentColor.g = static_cast<unsigned char>(RangeMap(object->m_duration, 0.0f, object->m_maxDuration, static_cast<float>(object->m_endColor.g), static_cast<float>(object->m_startColor.g)));
			object->m_currentColor.b = static_cast<unsigned char>(RangeMap(object->m_duration, 0.0f, object->m_maxDuration, static_cast<float>(object->m_endColor.b), static_cast<float>(object->m_startColor.b)));
			object->m_currentColor.a = static_cast<unsigned char>(RangeMap(object->m_duration, 0.0f, object->m_maxDuration, static_cast<float>(object->m_endColor.a), static_cast<float>(object->m_startColor.a)));

			if (object->m_duration >= 0.0f)
			{
				object->m_duration -= deltaSeconds;

				if (object->m_duration <= 0.0f)
				{
					delete object;
					object = nullptr;
				}
			}
		}
	}
	s_debugRender.m_debugMessagesMutex.unlock();
}


//
//geometry adding functions
//
void DebugAddWorldPoint(Vec3 const& point, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderSphere* debugRenderPoint = new DebugRenderSphere();

	debugRenderPoint->m_position = point;
	debugRenderPoint->m_radius = radius;
	debugRenderPoint->m_duration = duration;
	debugRenderPoint->m_maxDuration = duration;
	debugRenderPoint->m_startColor = startColor;
	debugRenderPoint->m_currentColor = startColor;
	debugRenderPoint->m_endColor = endColor;
	debugRenderPoint->m_mode = mode;
	debugRenderPoint->m_isWireframe = false;

	s_debugRender.m_debugWorldObjectsMutex.lock();
	s_debugRender.m_debugWorldObjects.push_back(debugRenderPoint);
	s_debugRender.m_debugWorldObjectsMutex.unlock();
}

void DebugAddWorldLine(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderCylinder* debugRenderLine = new DebugRenderCylinder();

	debugRenderLine->m_base = start;
	debugRenderLine->m_top = end;
	debugRenderLine->m_radius = radius;
	debugRenderLine->m_duration = duration;
	debugRenderLine->m_maxDuration = duration;
	debugRenderLine->m_startColor = startColor;
	debugRenderLine->m_currentColor = startColor;
	debugRenderLine->m_endColor = endColor;
	debugRenderLine->m_mode = mode;
	debugRenderLine->m_isWireframe = false;

	s_debugRender.m_debugWorldObjectsMutex.lock();
	s_debugRender.m_debugWorldObjects.push_back(debugRenderLine);
	s_debugRender.m_debugWorldObjectsMutex.unlock();
}


void DebugAddWorldWireCylinder(Vec3 const& base, Vec3 const& top, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderCylinder* debugRenderCylinder = new DebugRenderCylinder();

	debugRenderCylinder->m_base = base;
	debugRenderCylinder->m_top = top;
	debugRenderCylinder->m_radius = radius;
	debugRenderCylinder->m_duration = duration;
	debugRenderCylinder->m_maxDuration = duration;
	debugRenderCylinder->m_startColor = startColor;
	debugRenderCylinder->m_currentColor = startColor;
	debugRenderCylinder->m_endColor = endColor;
	debugRenderCylinder->m_mode = mode;
	debugRenderCylinder->m_isWireframe = true;

	s_debugRender.m_debugWorldObjectsMutex.lock();
	s_debugRender.m_debugWorldObjects.push_back(debugRenderCylinder);
	s_debugRender.m_debugWorldObjectsMutex.unlock();
}


void DebugAddWorldWireSphere(Vec3 const& center, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderSphere* debugRenderSphere = new DebugRenderSphere();

	debugRenderSphere->m_position = center;
	debugRenderSphere->m_radius = radius;
	debugRenderSphere->m_duration = duration;
	debugRenderSphere->m_maxDuration = duration;
	debugRenderSphere->m_startColor = startColor;
	debugRenderSphere->m_currentColor = startColor;
	debugRenderSphere->m_endColor = endColor;
	debugRenderSphere->m_mode = mode;
	debugRenderSphere->m_isWireframe = true;

	s_debugRender.m_debugWorldObjectsMutex.lock();
	s_debugRender.m_debugWorldObjects.push_back(debugRenderSphere);
	s_debugRender.m_debugWorldObjectsMutex.unlock();
}


void DebugAddWorldSphere(Vec3 const& center, float radius, float duration, Rgba8 const& startColor /*= Rgba8(255, 255, 255)*/, Rgba8 const& endColor /*= Rgba8(255, 255, 255)*/, DebugRenderMode mode /*= DebugRenderMode::USE_DEPTH*/)
{
	DebugRenderSphere* debugRenderSphere = new DebugRenderSphere();

	debugRenderSphere->m_position = center;
	debugRenderSphere->m_radius = radius;
	debugRenderSphere->m_duration = duration;
	debugRenderSphere->m_maxDuration = duration;
	debugRenderSphere->m_startColor = startColor;
	debugRenderSphere->m_currentColor = startColor;
	debugRenderSphere->m_endColor = endColor;
	debugRenderSphere->m_mode = mode;
	debugRenderSphere->m_isWireframe = false;

	s_debugRender.m_debugWorldObjectsMutex.lock();
	s_debugRender.m_debugWorldObjects.push_back(debugRenderSphere);
	s_debugRender.m_debugWorldObjectsMutex.unlock();
}


void DebugAddWorldArrow(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderArrow* debugRenderArrow = new DebugRenderArrow();

	debugRenderArrow->m_start = start;
	debugRenderArrow->m_end = end;
	debugRenderArrow->m_radius = radius;
	debugRenderArrow->m_duration = duration;
	debugRenderArrow->m_maxDuration = duration;
	debugRenderArrow->m_startColor = startColor;
	debugRenderArrow->m_currentColor = startColor;
	debugRenderArrow->m_endColor = endColor;
	debugRenderArrow->m_mode = mode;
	debugRenderArrow->m_isWireframe = false;

	s_debugRender.m_debugWorldObjectsMutex.lock();
	s_debugRender.m_debugWorldObjects.push_back(debugRenderArrow);
	s_debugRender.m_debugWorldObjectsMutex.unlock();
}


void DebugAddWorldText(std::string const& text, Mat44 const& transform, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderWorldText* debugRenderText = new DebugRenderWorldText();

	debugRenderText->m_text = text;
	debugRenderText->m_transform = transform;
	debugRenderText->m_textHeight = textHeight;
	debugRenderText->m_alignment = alignment;
	debugRenderText->m_duration = duration;
	debugRenderText->m_maxDuration = duration;
	debugRenderText->m_startColor = startColor;
	debugRenderText->m_currentColor = startColor;
	debugRenderText->m_endColor = endColor;
	debugRenderText->m_mode = mode;
	debugRenderText->m_isWireframe = false;
	debugRenderText->m_isText = true;

	s_debugRender.m_debugWorldObjectsMutex.lock();
	s_debugRender.m_debugWorldObjects.push_back(debugRenderText);
	s_debugRender.m_debugWorldObjectsMutex.unlock();
}


void DebugAddWorldBillboardText(std::string const& text, Vec3 const& origin, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderBillboardText* debugRenderBillboard = new DebugRenderBillboardText();

	debugRenderBillboard->m_text = text;
	debugRenderBillboard->m_origin = origin;
	debugRenderBillboard->m_textHeight = textHeight;
	debugRenderBillboard->m_alignment = alignment;
	debugRenderBillboard->m_duration = duration;
	debugRenderBillboard->m_maxDuration = duration;
	debugRenderBillboard->m_startColor = startColor;
	debugRenderBillboard->m_currentColor = startColor;
	debugRenderBillboard->m_endColor = endColor;
	debugRenderBillboard->m_mode = mode;
	debugRenderBillboard->m_isWireframe = false;
	debugRenderBillboard->m_isBillboard = true;
	debugRenderBillboard->m_isText = true;

	s_debugRender.m_debugWorldObjectsMutex.lock();
	s_debugRender.m_debugWorldObjects.push_back(debugRenderBillboard);
	s_debugRender.m_debugWorldObjectsMutex.unlock();
}


void DebugAddScreenText(std::string const& text, Vec2 const& position, float size, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor)
{
	DebugRenderScreenText* debugRenderText = new DebugRenderScreenText();

	debugRenderText->m_text = text;
	debugRenderText->m_position = position;
	debugRenderText->m_size = size;
	debugRenderText->m_alignment = alignment;
	debugRenderText->m_duration = duration;
	debugRenderText->m_maxDuration = duration;
	debugRenderText->m_startColor = startColor;
	debugRenderText->m_currentColor = startColor;
	debugRenderText->m_endColor = endColor;

	s_debugRender.m_debugScreenObjectsMutex.lock();
	s_debugRender.m_debugScreenObjects.push_back(debugRenderText);
	s_debugRender.m_debugScreenObjectsMutex.unlock();
}


void DebugAddMessage(std::string const& text, float duration, Rgba8 const& startColor, Rgba8 const& endColor)
{
	DebugRenderMessage* debugRenderMessage = new DebugRenderMessage();

	debugRenderMessage->m_text = text;
	debugRenderMessage->m_duration = duration;
	debugRenderMessage->m_maxDuration = duration;
	debugRenderMessage->m_startColor = startColor;
	debugRenderMessage->m_currentColor = startColor;
	debugRenderMessage->m_endColor = endColor;

	s_debugRender.m_debugMessagesMutex.lock();
	s_debugRender.m_debugMessages.push_back(debugRenderMessage);
	s_debugRender.m_debugMessagesMutex.unlock();
}


//
//console commands
//
bool Command_DebugRenderClear(EventArgs& args)
{
	UNUSED(args);

	DebugRenderClear();

	return true;
}


bool Command_DebugRenderToggle(EventArgs& args)
{
	UNUSED(args);

	s_debugRender.m_isHidden = !s_debugRender.m_isHidden;

	return true;
}
