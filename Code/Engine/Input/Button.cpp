#include "Engine/Input/Button.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"


//
//constructor
//
Button::Button(Renderer* renderer, InputSystem* input, AABB2 const& bounds, Vec2 const& screenSize, Texture* buttonTexture, std::string const& text, AABB2 const& relativeTextBounds, 
	Rgba8 const& color, Rgba8 const& textColor, Rgba8 const& selectedColor, Rgba8 const& selectedTextColor, Vec2 const& alignment)
	: m_renderer(renderer)
	, m_input(input)
	, m_bounds(bounds)
	, m_screenSize(screenSize)
	, m_texture(buttonTexture)
	, m_text(text)
	, m_textBoundsRelative(relativeTextBounds)
	, m_color(color)
	, m_textColor(textColor)
	, m_selectedColor(selectedColor)
	, m_selectedTextColor(selectedTextColor)
	, m_alignment(alignment)
{
	m_font = m_renderer->CreateOrGetBitmapFont("Data/Fonts/SquirrelFixedFont");
}


//
//public game flow functions
//
void Button::Update()
{
	if (m_justFired)
	{
		m_justFired = false;

		FireFollowupEvent();
	}
	
	Vec2 mousePosition = m_input->GetCursorNormalizedPosition();
	Vec2 gameMousePosition = Vec2(mousePosition.x * m_screenSize.x, mousePosition.y * m_screenSize.y);
	
	if (!m_overrideMouseSelection)
	{
		if (IsPointInsideAABB2D(gameMousePosition, m_bounds))
		{
			m_isSelected = true;
		}
		else
		{
			m_isSelected = false;
		}
	}
	
	if (m_isSelected && m_input->WasKeyJustPressed(KEYCODE_LMB))
	{
		FireButtonEvent();
	}
}


void Button::Render() const
{
	std::vector<Vertex_PCU> buttonVerts;

	if (m_isSelected)
	{
		AddVertsForAABB2(buttonVerts, m_bounds, m_selectedColor);
	}
	else
	{
		AddVertsForAABB2(buttonVerts, m_bounds, m_color);
	}

	m_renderer->BindTexture(m_texture);
	m_renderer->BindShader(nullptr);
	m_renderer->SetModelConstants();
	m_renderer->DrawVertexArray(buttonVerts);

	std::vector<Vertex_PCU> textVerts;

	float textHeight = (m_bounds.m_maxs.y - m_bounds.m_mins.y) * 0.75f;
	AABB2 textBoundsAbsolute = m_bounds.GetAABB2Within(m_textBoundsRelative.m_mins, m_textBoundsRelative.m_maxs);

	if (m_isSelected)
	{
		m_font->AddVertsForTextInBox2D(textVerts, textBoundsAbsolute, textHeight, m_text, m_selectedTextColor, 1.0f, m_alignment);
	}
	else
	{
		m_font->AddVertsForTextInBox2D(textVerts, textBoundsAbsolute, textHeight, m_text, m_textColor, 1.0f, m_alignment);
	}

	m_renderer->BindTexture(&m_font->GetTexture());
	m_renderer->DrawVertexArray(textVerts);
}


//
//public button utilities
//
void Button::SubscribeToEvent(std::string const& eventName, EventCallbackFunction eventFuncPointer)
{
	SubscribeEventCallbackFunction(eventName, eventFuncPointer);
	m_eventName = eventName;
}


void Button::AddEvent(std::string const& eventName, EventArgs args)
{
	m_eventName = eventName;
	m_args = args;
}


void Button::AddFollowupEvent(std::string const& eventName, EventArgs args)
{
	m_followupEventName = eventName;
	m_followupArgs = args;
}


bool Button::IsMouseInsideBounds() const
{
	Vec2 mousePosition = m_input->GetCursorNormalizedPosition();
	Vec2 gameMousePosition = Vec2(mousePosition.x * m_screenSize.x, mousePosition.y * m_screenSize.y);

	return IsPointInsideAABB2D(gameMousePosition, m_bounds);
}


void Button::FireButtonEvent()
{
	if (m_args.m_keyValuePairs.size() > 0)
	{
		FireEvent(m_eventName, m_args);
	}
	else
	{
		FireEvent(m_eventName);
	}

	m_justFired = true;
}


void Button::FireFollowupEvent()
{
	if (m_followupArgs.m_keyValuePairs.size() > 0)
	{
		FireEvent(m_followupEventName, m_followupArgs);
	}
	else
	{
		FireEvent(m_followupEventName);
	}
}
