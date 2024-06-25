#pragma once
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/EngineCommon.hpp"


//forward declarations
class Renderer;
class Camera;
struct Vec2;
struct Vec3;
struct Mat44;


enum class DebugRenderMode
{
	ALWAYS,
	USE_DEPTH,
	X_RAY,
	COUNT
};


struct DebugRenderConfig
{
	Renderer* m_renderer = nullptr;
};


//setup functions
void DebugRenderSystemStartup(DebugRenderConfig const& config);
void DebugRenderSystemShutdown();

//options functions
void DebugRenderSetVisible();
void DebugRenderSetHidden();
void DebugRenderClear();

//frame flow functions
void DebugRenderBeginFrame();
void DebugRenderWorld(Camera const& camera);
void DebugRenderScreen(Camera const& camera);
void DebugRenderEndFrame();

//geometry adding functions
void DebugAddWorldPoint(Vec3 const& point, float radius, float duration, Rgba8 const& startColor = Rgba8(255, 255, 255), Rgba8 const& endColor = Rgba8(255, 255, 255), 
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldLine(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor = Rgba8(255, 255, 255), Rgba8 const& endColor = Rgba8(255, 255, 255),
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireCylinder(Vec3 const& base, Vec3 const& top, float radius, float duration, Rgba8 const& startColor = Rgba8(255, 255, 255), Rgba8 const& endColor = Rgba8(255, 255, 255),
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireSphere(Vec3 const& center, float radius, float duration, Rgba8 const& startColor = Rgba8(255, 255, 255), Rgba8 const& endColor = Rgba8(255, 255, 255),
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldSphere(Vec3 const& center, float radius, float duration, Rgba8 const& startColor = Rgba8(255, 255, 255), Rgba8 const& endColor = Rgba8(255, 255, 255),
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldArrow(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor = Rgba8(255, 255, 255), Rgba8 const& endColor = Rgba8(255, 255, 255),
	DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldText(std::string const& text, Mat44 const& transform, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor = Rgba8(255, 255, 255), 
	Rgba8 const& endColor = Rgba8(255, 255, 255), DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldBillboardText(std::string const& text, Vec3 const& origin, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor = Rgba8(255, 255, 255),
	Rgba8 const& endColor = Rgba8(255, 255, 255), DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddScreenText(std::string const& text, Vec2 const& position, float size, Vec2 const& alignment, float duration, Rgba8 const& startColor = Rgba8(255, 255, 255),
	Rgba8 const& endColor = Rgba8(255, 255, 255));
void DebugAddMessage(std::string const& text, float duration, Rgba8 const& startColor = Rgba8(255, 255, 255), Rgba8 const& endColor = Rgba8(255, 255, 255));

//console commands
bool Command_DebugRenderClear(EventArgs& args);
bool Command_DebugRenderToggle(EventArgs& args);
