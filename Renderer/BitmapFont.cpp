#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/Vec3.hpp"


//
//constructor
//
BitmapFont::BitmapFont(char const* fontFilePathNameNoExtension, Texture& fontTexture)
	: m_fontFilePathNameNoExtension(fontFilePathNameNoExtension)
	, m_fontGlyphsSpriteSheet(fontTexture, IntVec2(16, 16))
{
}


//
//vertex adding function
//
void BitmapFont::AddVertsForText2D(std::vector<Vertex_PCU>& verts, Vec2 const& textMins, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect)
{
	float cellWidth = cellHeight * cellAspect;
	
	for (int charIndex = 0; charIndex < text.length(); charIndex++)
	{
		float charMinX = textMins.x + (cellWidth * charIndex);
		float charMaxX = charMinX + cellWidth;
		float charMinY = textMins.y;
		float charMaxY = textMins.y + cellHeight;

		AABB2 bounds = AABB2(charMinX, charMinY, charMaxX, charMaxY);
		AABB2 uvs = m_fontGlyphsSpriteSheet.GetSpriteUVs(text[charIndex]);
		AddVertsForAABB2(verts, bounds, tint, uvs);
	}
}


void BitmapFont::AddVertsForTextInBox2D(std::vector<Vertex_PCU>& verts, AABB2 const& box, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect, Vec2 const& alignment, TextBoxMode mode, int maxGlyphsToDraw)
{
	float cellWidth = cellHeight * cellAspect;

	Strings splitText = SplitStringOnDelimiter(text, '\n');

	float textHeight = splitText.size() * cellHeight;
	
	//fine the longest line and make its width the text width
	float textWidth = 0.0f;
	int longestLine = -1;	//save longest line to use if needed for shrink to fit
	for (int lineIndex = 0; lineIndex < splitText.size(); lineIndex++)
	{
		float lineWidth = splitText[lineIndex].length() * cellWidth;
		if (lineWidth > textWidth)
		{
			textWidth = lineWidth;
			longestLine = lineIndex;
		}
	}

	//do all shrink to fit stuff here
	if (mode == TextBoxMode::SHRINK_TO_FIT)
	{
		if (textHeight > box.GetDimensions().y)
		{
			textHeight = box.GetDimensions().y;
			cellHeight = textHeight / static_cast<float>(splitText.size());
		}
		if (textWidth > box.GetDimensions().x)
		{
			textWidth = box.GetDimensions().x;
			cellWidth = textWidth / static_cast<float>(splitText[longestLine].length());
		}
	}

	//find out how much space is needed above the text and to the left of the text (since text is written from top to bottom and left to right)
	float boxTopBuffer = (box.GetDimensions().y - textHeight) * (1.0f - alignment.y);
	float boxLeftBuffer = (box.GetDimensions().x - textWidth) * alignment.x;

	//strt counter for total number of characters
	int numTotalChars = 0;

	for (int lineIndex = 0; lineIndex < splitText.size(); lineIndex++)
	{
		for (int charIndex = 0; charIndex < splitText[lineIndex].length(); charIndex++)
		{
			float lineWidth = splitText[lineIndex].length() * cellWidth;
			
			float lineLeftBuffer = boxLeftBuffer + ((textWidth - lineWidth) * alignment.x);
			
			float charMinX = box.m_mins.x + lineLeftBuffer + (cellWidth * charIndex);
			float charMaxX = charMinX + cellWidth;

			//starting from max first since we're working from the top down
			float charMaxY = box.m_maxs.y - boxTopBuffer - (lineIndex * cellHeight);
			float charMinY = charMaxY - cellHeight;

			AABB2 charBounds = AABB2(charMinX, charMinY, charMaxX, charMaxY);
			AABB2 uvs = m_fontGlyphsSpriteSheet.GetSpriteUVs(splitText[lineIndex][charIndex]);
			AddVertsForAABB2(verts, charBounds, tint, uvs);

			//if the number of characters equals the max, return (or greater just in case some bug happens)
			numTotalChars++;
			if (numTotalChars >= maxGlyphsToDraw)
			{
				return;
			}
		}
	}
}


void BitmapFont::AddVertsForText3D(std::vector<Vertex_PCU>& verts, Vec2 const& textMins, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect, Vec2 const& alignment, int maxGlyphsToDraw)
{
	//INCOMPLETE

	AddVertsForTextInBox2D(verts, AABB2(textMins - Vec2(1.0f, 1.0f), textMins + Vec2(1.0f, 1.0f)), cellHeight, text, tint, cellAspect, alignment, TextBoxMode::OVERRUN, maxGlyphsToDraw);

	Mat44 textOrientMatrix = Mat44::CreateXRotationDegrees(90.0f);
	textOrientMatrix.AppendYRotation(90.0f);

	TransformVertexArray3D(verts, textOrientMatrix);
}


//
//public accessors
//
Texture& BitmapFont::GetTexture()
{
	return m_fontGlyphsSpriteSheet.GetTexture();
}


float BitmapFont::GetTextWidth(float cellHeight, std::string const& text, float cellAspect)
{
	float cellWidth = cellHeight * cellAspect;
	
	return cellWidth * text.length();
}


//
//protected accessors
//
float BitmapFont::GetGlyphAspect(int glyphUnicode) const
{
	UNUSED(glyphUnicode);

	//always returns 1.0f for now
	return 1.0f;
}
