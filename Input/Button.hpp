#pragma once
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/BitmapFont.hpp"


//forward declaration
class Renderer;
class InputSystem;


class Button
{
//public member functions
public:
	//constructor
	Button(Renderer* renderer, InputSystem* input, AABB2 const& bounds, std::string  const&text = "", Rgba8 const& color = Rgba8(), Rgba8 const& textColor = Rgba8());

	//game flow functions
	void Update();
	void Render() const;

	//button utilities


//public member variables
public:
	AABB2		m_bounds = AABB2();
	Rgba8		m_color = Rgba8();
	std::string m_text = "";
	Rgba8		m_textColor = Rgba8();

	Renderer*	 m_renderer = nullptr;
	InputSystem* m_input = nullptr;
	BitmapFont*	 m_font = nullptr;
};
