#pragma once
#include "Engine/Input/AnalogJoystick.hpp"
#include "Engine/Input/KeyButtonState.hpp"


//enum for xbox button id
//TO-DO: enum class?
enum XboxButtonID
{
	XBOX_BUTTON_INVALID = -1,

	XBOX_BUTTON_A,
	XBOX_BUTTON_B,
	XBOX_BUTTON_X,
	XBOX_BUTTON_Y,
	XBOX_BUTTON_START,
	XBOX_BUTTON_SELECT,
	XBOX_BUTTON_UP,
	XBOX_BUTTON_DOWN,
	XBOX_BUTTON_RIGHT,
	XBOX_BUTTON_LEFT,
	XBOX_BUTTON_LSTICK,
	XBOX_BUTTON_RSTICK,
	XBOX_BUTTON_L,
	XBOX_BUTTON_R,

	NUM_XBOX_BUTTONS
};


class XboxController
{
	friend class InputSystem;	//let it access InputSystem's protected variables

//public member functions
public:
	//constructor and destructor
	XboxController() {}
	~XboxController() {}

	//connection and id
	bool IsConnected()	   const;
	int	 GetControllerID() const;

	//trigger functions
	float GetLeftTrigger() const;
	float GetRightTrigger() const;

	//button functions
	KeyButtonState const& GetButton(XboxButtonID button) const;
	bool				  IsButtonDown(XboxButtonID button) const;
	bool				  WasButtonJustPressed(XboxButtonID button) const;
	bool				  WasButtonJustReleased(XboxButtonID button) const;

	//joystick functions
	AnalogJoystick const& GetLeftStick() const;
	AnalogJoystick const& GetRightStick() const;

//private member functions
private:
	//game flow functions
	void Update();
	void Reset();
	void UpdateJoystick(AnalogJoystick& joystick, short rawX, short rawY);
	void UpdateTrigger(float& triggerValue, unsigned char rawValue);
	void UpdateButton(XboxButtonID button, unsigned short buttonFlags, unsigned short flagToCheck);

//private member variables
private:
	//controller info
	int	 m_id = -1;
	bool m_isConnected = false;

	//input info
	float		   m_leftTrigger = 0.f;
	float		   m_rightTrigger = 0.f;
	KeyButtonState m_buttons[static_cast<int>(NUM_XBOX_BUTTONS)];
	AnalogJoystick m_leftJoystick;
	AnalogJoystick m_rightJoystick;
};
