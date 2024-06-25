#pragma once
#include "Engine/Input/KeyButtonState.hpp"
#include "Engine/Input/XboxController.hpp"
#include "Engine/Core/EngineCommon.hpp"


//external constants
extern unsigned char const KEYCODE_F1;
extern unsigned char const KEYCODE_F2;
extern unsigned char const KEYCODE_F3;
extern unsigned char const KEYCODE_F4;
extern unsigned char const KEYCODE_F5;
extern unsigned char const KEYCODE_F6;
extern unsigned char const KEYCODE_F7;
extern unsigned char const KEYCODE_F8;
extern unsigned char const KEYCODE_F9;
extern unsigned char const KEYCODE_F10;
extern unsigned char const KEYCODE_F11;
extern unsigned char const KEYCODE_ESC;
extern unsigned char const KEYCODE_SPACE;
extern unsigned char const KEYCODE_ENTER;
extern unsigned char const KEYCODE_UP;
extern unsigned char const KEYCODE_DOWN;
extern unsigned char const KEYCODE_LEFT;
extern unsigned char const KEYCODE_RIGHT;
extern unsigned char const KEYCODE_LMB;
extern unsigned char const KEYCODE_RMB;
extern unsigned char const KEYCODE_TILDE;
extern unsigned char const KEYCODE_BACKSPACE;
extern unsigned char const KEYCODE_INSERT;
extern unsigned char const KEYCODE_DELETE;
extern unsigned char const KEYCODE_HOME;
extern unsigned char const KEYCODE_END;
extern unsigned char const KEYCODE_SHIFT;
extern unsigned char const KEYCODE_LEFTBRACKET;
extern unsigned char const KEYCODE_RIGHTBRACKET;
extern unsigned char const KEYCODE_COMMA;
extern unsigned char const KEYCODE_PERIOD;
extern unsigned char const KEYCODE_SEMICOLON;
extern unsigned char const KEYCODE_SINGLEQUOTE;


//internal constants
constexpr int	NUM_KEYCODES = 256;
constexpr int	NUM_XBOX_CONTROLLERS = 4;
constexpr float OUTER_DEAD_ZONE_THRESHOLD = 0.95f;
constexpr float INNER_DEAD_ZONE_THRESHOLD = 0.3f;


//mouse state struct
struct MouseState
{
	IntVec2 m_cursorClientPosition;
	IntVec2 m_cursorClientDelta;

	bool m_currentHidden = false;
	bool m_desiredHidden = false;

	bool m_currentRelative = false;
	bool m_desiredRelative = false;
};

//config struct
struct InputSystemConfig
{
};


class InputSystem
{
//public member functions
public:
	//constructors and destructor
	InputSystem(InputSystemConfig const& config);
	~InputSystem() {}

	//game flow functions
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	//key state functions
	bool HandleKeyPressed(unsigned char keyCode);
	bool HandleKeyReleased(unsigned char keyCode);
	bool IsKeyDown(unsigned char keyCode);
	bool WasKeyJustPressed(unsigned char keyCode);
	bool WasKeyJustReleased(unsigned char keyCode);

	//static key state functions
	static bool Event_KeyPressed(EventArgs& args);
	static bool Event_KeyReleased(EventArgs& args);

	//xbox controller functions
	XboxController const& GetController(int controllerID);

	//mouse functions
	void SetCursorMode(bool hidden, bool relative);
	IntVec2 GetCursorClientDelta() const;
	IntVec2 GetCursorClientPosition() const;
	Vec2 GetCursorNormalizedPosition() const;

	//accessors
	InputSystemConfig const& GetConfig() const	{ return m_config; }

//protected member variables
protected:
	InputSystemConfig m_config;
	KeyButtonState	  m_keyStates[NUM_KEYCODES];
	XboxController	  m_controllers[NUM_XBOX_CONTROLLERS];
	MouseState		  m_mouseState;
};
