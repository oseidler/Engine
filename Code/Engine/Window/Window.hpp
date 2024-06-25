#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/IntVec2.hpp"

//forward declarations
class InputSystem;

//config struct
struct WindowConfig
{
	InputSystem* m_inputSystem = nullptr;
	std::string	 m_windowTitle = "Untitled App";
	float		 m_clientAspect = 2.f;
	float		 m_clientFraction = 0.9f;
	IntVec2		 m_windowSize = IntVec2();
	IntVec2		 m_windowPosition = IntVec2();
	bool		 m_isFullscreen = false;
};


class Window
{
//public member functions
public:
	//constructor and destructor
	Window(WindowConfig const& config);
	~Window() {}

	//game flow functions
	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	//accessors
	WindowConfig const& GetConfig() const	{ return m_config; }
	Vec2				GetCursorNormalizedPos() const;
	void*				GetHwnd() const { return m_hWnd; }
	IntVec2				GetClientDimensions() const { return m_clientDimensions; }
	bool			    HasFocus() const;
	std::string			GetXMLFileNameFromDialog(unsigned char keyRelease);

	//static functions
	static Window* GetWindowContext()	{ return s_mainWindow; }

//protected member functions
protected:
	//window utilities
	void CreateOSWindow();
	void RunMessagePump();

//protected member variables
protected:
	WindowConfig   m_config;
	IntVec2		   m_clientDimensions;
	void*		   m_hWnd;
	static Window* s_mainWindow;
};
