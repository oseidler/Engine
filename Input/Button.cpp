#include "Engine/Input/Button.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"


//
//constructor
//
Button::Button(Renderer* renderer, InputSystem* input, AABB2 const& bounds, std::string const& text, Rgba8 const& color, Rgba8 const& textColor)
	: m_renderer(renderer)
	, m_input(input)
	, m_bounds(bounds)
	, m_text(text)
	, m_color(color)
	, m_textColor(textColor)
{
	m_font = m_renderer->CreateOrGetBitmapFont("Data/Fonts/SquirrelFixedFont");
}


//
//public game flow functions
//
void Button::Update()
{
	/*if (m_input->WasKeyJustPressed(KEYCODE_LMB) &&)
	{

	}*/
}


void Button::Render() const
{
	std::vector<Vertex_PCU> buttonVerts;

	AddVertsForAABB2(buttonVerts, m_bounds, m_color);

	m_renderer->BindTexture(nullptr);
	m_renderer->BindShader(nullptr);
	m_renderer->DrawVertexArray(buttonVerts);

	std::vector<Vertex_PCU> textVerts;

	float textHeight = (m_bounds.m_maxs.y - m_bounds.m_mins.y) * 0.75f;

	m_font->AddVertsForTextInBox2D(textVerts, m_bounds, textHeight, m_text, m_textColor);

	m_renderer->BindTexture(&m_font->GetTexture());
	m_renderer->DrawVertexArray(textVerts);
}
