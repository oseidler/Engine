#define WIN32_LEAN_AND_MEAN
#include "Engine/Input/XboxController.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <Windows.h>
#include <Xinput.h>
#pragma comment (lib, "xinput9_1_0")


//
//connection functions
//
bool XboxController::IsConnected() const
{
	return m_isConnected;
}


int XboxController::GetControllerID() const
{
	return m_id;
}


//
//trigger functions
//
float XboxController::GetLeftTrigger() const
{
	return m_leftTrigger;
}


float XboxController::GetRightTrigger() const
{
	return m_rightTrigger;
}


//
//button functions
//
KeyButtonState const& XboxController::GetButton(XboxButtonID button) const
{
	return m_buttons[button];
}


bool XboxController::IsButtonDown(XboxButtonID button) const
{
	return m_buttons[button].m_isPressed;
}


bool XboxController::WasButtonJustPressed(XboxButtonID button) const
{
	return (!m_buttons[button].m_wasPressedLastFrame && m_buttons[button].m_isPressed);
}


bool XboxController::WasButtonJustReleased(XboxButtonID button) const
{
	return (m_buttons[button].m_wasPressedLastFrame && !m_buttons[button].m_isPressed);
}


//
//joystick functions
//
AnalogJoystick const& XboxController::GetLeftStick() const
{
	return m_leftJoystick;
}


AnalogJoystick const& XboxController::GetRightStick() const
{
	return m_rightJoystick;
}


//
//game flow functions
//
void XboxController::Update()
{
	XINPUT_STATE xboxControllerState = {};
	DWORD errorStatus = XInputGetState(m_id, &xboxControllerState);
	if (errorStatus == ERROR_SUCCESS)
	{
		m_isConnected = true;

		//update joysticks
		UpdateJoystick(m_leftJoystick, xboxControllerState.Gamepad.sThumbLX, xboxControllerState.Gamepad.sThumbLY);
		UpdateJoystick(m_rightJoystick, xboxControllerState.Gamepad.sThumbRX, xboxControllerState.Gamepad.sThumbRY);

		//update triggers
		UpdateTrigger(m_leftTrigger, xboxControllerState.Gamepad.bLeftTrigger);
		UpdateTrigger(m_rightTrigger, xboxControllerState.Gamepad.bRightTrigger);

		//update buttons
		UpdateButton(XBOX_BUTTON_A, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_A);
		UpdateButton(XBOX_BUTTON_B, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_B);
		UpdateButton(XBOX_BUTTON_X, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_X);
		UpdateButton(XBOX_BUTTON_Y, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_Y);
		UpdateButton(XBOX_BUTTON_L, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER);
		UpdateButton(XBOX_BUTTON_R, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER);
		UpdateButton(XBOX_BUTTON_LSTICK, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_LEFT_THUMB);
		UpdateButton(XBOX_BUTTON_RSTICK, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_RIGHT_THUMB);
		UpdateButton(XBOX_BUTTON_START, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_START);
		UpdateButton(XBOX_BUTTON_SELECT, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_BACK);
		UpdateButton(XBOX_BUTTON_UP, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_UP);
		UpdateButton(XBOX_BUTTON_DOWN, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_DOWN);
		UpdateButton(XBOX_BUTTON_RIGHT, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_RIGHT);
		UpdateButton(XBOX_BUTTON_LEFT, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_LEFT);
	}
	else
	{
		Reset();	//reset controller if it is disconnected
		m_isConnected = false;
	}
}


void XboxController::Reset()
{
	//change everything back to zero
	m_leftJoystick.UpdatePosition(0.f, 0.f);
	m_rightJoystick.UpdatePosition(0.f, 0.f);
	m_leftTrigger = 0.f;
	m_rightTrigger = 0.f;
	for (int buttonIndex = 0; buttonIndex < NUM_XBOX_BUTTONS; buttonIndex++)
	{
		m_buttons[buttonIndex].m_isPressed = false;
	}
}


void XboxController::UpdateJoystick(AnalogJoystick& joystick, short rawX, short rawY)
{
	float rawNormalX = static_cast<float>(rawX) / 32767.f;
	float rawNormalY = static_cast<float>(rawY) / 32767.f;
	
	joystick.UpdatePosition(rawNormalX, rawNormalY);
}


void XboxController::UpdateTrigger(float& triggerValue, unsigned char rawValue)
{
	triggerValue = GetFractionWithinRange(static_cast<float>(rawValue), 0.f, 255.f);
}


void XboxController::UpdateButton(XboxButtonID button, unsigned short buttonFlags, unsigned short flagToCheck)
{
	m_buttons[static_cast<int>(button)].m_isPressed = (buttonFlags & flagToCheck) == flagToCheck;
}
