#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"


//forward declarations
class Texture;
struct Vertex_PCU;


//enum for different text box modes
enum class TextBoxMode
{
	SHRINK_TO_FIT,
	OVERRUN
};


class BitmapFont
{
	friend class Renderer;

//private member functions
private:
	//private constructor
	BitmapFont(char const* fontFilePathNameNoExtension, Texture& fontTexture);

//public member functions
public:
	//vertex adding functions
	void AddVertsForText2D(std::vector<Vertex_PCU>& verts, Vec2 const& textMins, float cellHeight, std::string const& text, Rgba8 const& tint = Rgba8(255, 255, 255), 
		float cellAspect = 1.0f);
	void AddVertsForTextInBox2D(std::vector<Vertex_PCU>& verts, AABB2 const& box, float cellHeight, std::string const& text, Rgba8 const& tint = Rgba8(255, 255, 255), 
		float cellAspect = 1.0f, Vec2 const& alignment = Vec2(0.5f, 0.5f), TextBoxMode mode = TextBoxMode::SHRINK_TO_FIT, int maxGlyphsToDraw = INT_MAX);
	void AddVertsForText3D(std::vector<Vertex_PCU>& verts, Vec2 const& textMins, float cellHeight, std::string const& text, Rgba8 const& tint = Rgba8(), 
		float cellAspect = 1.0f, Vec2 const& alignment = Vec2(0.5f, 0.5f), int maxGlyphsToDraw = INT_MAX);
	
	//accessors
	Texture& GetTexture();
	float	 GetTextWidth(float cellHeight, std::string const& text, float cellAspect = 1.0f);

//protected member functions
protected:
	float GetGlyphAspect(int glyphUnicode) const;

//protected member variables
protected:
	std::string m_fontFilePathNameNoExtension;
	SpriteSheet m_fontGlyphsSpriteSheet;
};
