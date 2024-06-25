#pragma once
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/BitmapFont.hpp"


//forward declaration
class Renderer;
class InputSystem;
class Texture;


class Button
{
//public member functions
public:
	//constructor
	Button() {}
	Button(Renderer* renderer, InputSystem* input, AABB2 const& bounds, Vec2 const& screenSize, Texture* buttonTexture = nullptr, std::string const&text = "", 
		AABB2 const& relativeTextBounds = AABB2(), Rgba8 const& color = Rgba8(), Rgba8 const& textColor = Rgba8(), Rgba8 const& selectedColor = Rgba8(), 
		Rgba8 const& selectedTextColor = Rgba8(), Vec2 const& alignment = Vec2(0.5f, 0.5f));

	//game flow functions
	void Update();
	void Render() const;

	//button utilities
	void SubscribeToEvent(std::string const& eventName, EventCallbackFunction eventFuncPointer);
	void AddEvent(std::string const& eventName, EventArgs args);
	void AddFollowupEvent(std::string const& eventName, EventArgs args);
	bool IsMouseInsideBounds() const;
	void FireButtonEvent();
	void FireFollowupEvent();

//public member variables
public:
	AABB2		m_bounds = AABB2();
	Rgba8		m_color = Rgba8();
	Rgba8		m_selectedColor = Rgba8();
	std::string m_text = "";
	AABB2		m_textBoundsRelative = AABB2();
	Rgba8		m_textColor = Rgba8();
	Rgba8		m_selectedTextColor = Rgba8();
	Texture*	m_texture = nullptr;
	Vec2		m_alignment = Vec2(0.5f, 0.5f);

	std::string m_eventName = "";
	std::string m_followupEventName = "";
	EventArgs m_args;
	EventArgs m_followupArgs;

	Renderer*	 m_renderer = nullptr;
	InputSystem* m_input = nullptr;
	BitmapFont*	 m_font = nullptr;

	Vec2 m_screenSize = Vec2();

	bool m_isSelected = false;
	bool m_overrideMouseSelection = false;
	bool m_justFired = false;
};
