#define WIN32_LEAN_AND_MEAN
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <Windows.h>


//global constants
unsigned char const KEYCODE_F1 = VK_F1;
unsigned char const KEYCODE_F2 = VK_F2;
unsigned char const KEYCODE_F3 = VK_F3;
unsigned char const KEYCODE_F4 = VK_F4;
unsigned char const KEYCODE_F5 = VK_F5;
unsigned char const KEYCODE_F6 = VK_F6;
unsigned char const KEYCODE_F7 = VK_F7;
unsigned char const KEYCODE_F8 = VK_F8;
unsigned char const KEYCODE_F9 = VK_F9;
unsigned char const KEYCODE_F10 = VK_F10;
unsigned char const KEYCODE_F11 = VK_F11;
unsigned char const KEYCODE_ESC = VK_ESCAPE;
unsigned char const KEYCODE_SPACE = VK_SPACE;
unsigned char const KEYCODE_ENTER = VK_RETURN;
unsigned char const KEYCODE_UP = VK_UP;
unsigned char const KEYCODE_DOWN = VK_DOWN;
unsigned char const KEYCODE_LEFT = VK_LEFT;
unsigned char const KEYCODE_RIGHT = VK_RIGHT;
unsigned char const KEYCODE_LMB = VK_LBUTTON;
unsigned char const KEYCODE_RMB = VK_RBUTTON;
unsigned char const KEYCODE_TILDE = 0xC0;
unsigned char const KEYCODE_BACKSPACE = VK_BACK;
unsigned char const KEYCODE_INSERT = VK_INSERT;
unsigned char const KEYCODE_DELETE = VK_DELETE;
unsigned char const KEYCODE_HOME = VK_HOME;
unsigned char const KEYCODE_END = VK_END;
unsigned char const KEYCODE_SHIFT = VK_SHIFT;
unsigned char const KEYCODE_LEFTBRACKET = 0xDB;
unsigned char const KEYCODE_RIGHTBRACKET = 0xDD;
unsigned char const KEYCODE_COMMA = VK_OEM_COMMA;
unsigned char const KEYCODE_PERIOD = VK_OEM_PERIOD;
unsigned char const KEYCODE_SEMICOLON = VK_OEM_1;
unsigned char const KEYCODE_SINGLEQUOTE = VK_OEM_7;


//
//constructor
//
InputSystem::InputSystem(InputSystemConfig const& config)
	: m_config(config)
{
}

//external variable declaration
extern InputSystem* g_theInput;

//
//game flow functions
//
void InputSystem::Startup()
{
	for (int controllerIndex = 0; controllerIndex < NUM_XBOX_CONTROLLERS; controllerIndex++)
	{
		m_controllers[controllerIndex].m_id = controllerIndex;
		m_controllers[controllerIndex].m_leftJoystick.SetDeadZoneThresholds(INNER_DEAD_ZONE_THRESHOLD, OUTER_DEAD_ZONE_THRESHOLD);
	}

	SubscribeEventCallbackFunction("KeyPressed", Event_KeyPressed);
	SubscribeEventCallbackFunction("KeyReleased", Event_KeyReleased);
}


void InputSystem::Shutdown()
{
}


void InputSystem::BeginFrame()
{
	//poll for controller inputs
	for (int controllerIndex = 0; controllerIndex < NUM_XBOX_CONTROLLERS; controllerIndex++)
	{
		XboxController& controller = m_controllers[controllerIndex];
		for (int buttonIndex = 0; buttonIndex < NUM_XBOX_BUTTONS; buttonIndex++)
		{
			KeyButtonState& button = controller.m_buttons[buttonIndex];
			button.m_wasPressedLastFrame = button.m_isPressed;
		}

		controller.Update();
	}

	//set mouse stuff
	if (m_mouseState.m_currentHidden != m_mouseState.m_desiredHidden)
	{
		m_mouseState.m_currentHidden = m_mouseState.m_desiredHidden;

		//show/hide cursor
		if (m_mouseState.m_currentHidden)
		{
			while (ShowCursor(false) >= 0);
		}
		else
		{
			while (ShowCursor(true) < 0);
		}
	}

	//get actual cursor position
	POINT cursorPos;
	::GetCursorPos(&cursorPos);

	//convert to client window coordinates
	HWND activeWindow = ::GetActiveWindow();
	::ScreenToClient(activeWindow, &cursorPos);

	IntVec2 newCursorPos = IntVec2(static_cast<int>(cursorPos.x), static_cast<int>(cursorPos.y));

	if (m_mouseState.m_currentRelative != m_mouseState.m_desiredRelative)
	{
		m_mouseState.m_currentRelative = m_mouseState.m_desiredRelative;

		//zero cursor client position and cursor client delta
		m_mouseState.m_cursorClientDelta = IntVec2();
		m_mouseState.m_cursorClientPosition = newCursorPos;
	}

	if (m_mouseState.m_currentRelative)
	{
		//store cursor client delta and reset cursor position to center of window
		m_mouseState.m_cursorClientDelta = newCursorPos - m_mouseState.m_cursorClientPosition;
		
		//convert to screen window coordinates
		RECT clientRect;
		::GetClientRect(activeWindow, &clientRect);

		POINT windowCenter;
		windowCenter.x = (clientRect.right - clientRect.left) / 2;
		windowCenter.y = (clientRect.bottom - clientRect.top) / 2;
		::ClientToScreen(activeWindow, &windowCenter);

		SetCursorPos(windowCenter.x, windowCenter.y);

		GetCursorPos(&cursorPos);

		//convert to client window coordinates
		::ScreenToClient(activeWindow, &cursorPos);

		newCursorPos = IntVec2(static_cast<int>(cursorPos.x), static_cast<int>(cursorPos.y));
	}

	m_mouseState.m_cursorClientPosition = newCursorPos;
}


void InputSystem::EndFrame()
{
	//copy inputs pressed this frame into array of inputs that were pressed the previous frame
	for (int index = 0; index < NUM_KEYCODES; index++)
	{
		m_keyStates[index].m_wasPressedLastFrame = m_keyStates[index].m_isPressed;
	}
}


//
//key state functions
//
bool InputSystem::HandleKeyPressed(unsigned char keyCode)
{
	m_keyStates[keyCode].m_isPressed = true;

	return true;
}


bool InputSystem::HandleKeyReleased(unsigned char keyCode)
{
	m_keyStates[keyCode].m_isPressed = false;

	return true;
}


bool InputSystem::IsKeyDown(unsigned char keyCode)
{
	return m_keyStates[keyCode].m_isPressed;
}


bool InputSystem::WasKeyJustPressed(unsigned char keyCode)
{
	return (m_keyStates[keyCode].m_isPressed && !m_keyStates[keyCode].m_wasPressedLastFrame);
}


bool InputSystem::WasKeyJustReleased(unsigned char keyCode)
{
	return (!m_keyStates[keyCode].m_isPressed && m_keyStates[keyCode].m_wasPressedLastFrame);
}


//
//static key state functions
//
bool InputSystem::Event_KeyPressed(EventArgs& args)
{
	if (g_theInput == nullptr)
	{
		return false;
	}

	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	g_theInput->HandleKeyPressed(keyCode);
	return true;
}


bool InputSystem::Event_KeyReleased(EventArgs& args)
{
	if (g_theInput == nullptr)
	{
		return false;
	}

	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	g_theInput->HandleKeyReleased(keyCode);
	return true;
}


//
//controller functions
//
XboxController const& InputSystem::GetController(int controllerID)
{
	if (controllerID < 0 || controllerID >= NUM_XBOX_CONTROLLERS)
	{
		//handle this somehow, just returns the first one for now
		return m_controllers[0];
	}
	else
	{
		return m_controllers[controllerID];
	}
}

//
//public mouse functions
//
void InputSystem::SetCursorMode(bool hidden, bool relative)
{
	m_mouseState.m_desiredHidden = hidden;
	m_mouseState.m_desiredRelative = relative;
}


IntVec2 InputSystem::GetCursorClientDelta() const
{
	return m_mouseState.m_cursorClientDelta;
}


IntVec2 InputSystem::GetCursorClientPosition() const
{
	return m_mouseState.m_cursorClientPosition;
}


Vec2 InputSystem::GetCursorNormalizedPosition() const
{
	//get actual cursor position
	POINT cursorPos;
	::GetCursorPos(&cursorPos);

	//convert to normalized client window coordinates
	HWND activeWindow = ::GetActiveWindow();
	::ScreenToClient(activeWindow, &cursorPos);

	RECT clientRect;
	::GetClientRect(activeWindow, &clientRect);

	float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
	float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

	float cursorNormalizedU = static_cast<float>(cursorPos.x) / clientWidth;
	float cursorNormalizedV = static_cast<float>(cursorPos.y) / clientHeight;

	return Vec2(cursorNormalizedU, 1.0f - cursorNormalizedV);
}
