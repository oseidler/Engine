#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"


//
//constructor
//
SpriteSheet::SpriteSheet(Texture& texture, IntVec2 const& simpleGridLayout)
	: m_texture(texture)
{
	//get sprite size in u and v
	float spriteWidthU = 1.0f / static_cast<float>(simpleGridLayout.x);
	float spriteHeightV = 1.0f / static_cast<float>(simpleGridLayout.y);
	int spriteIndex = 0;

	//get amount to nudge for sample correction
	IntVec2 texDims = texture.GetDimensions();
	float sampleCorrectX = 1.0f / static_cast<float>(texDims.x * 128);
	float sampleCorrectY = 1.0f / static_cast<float>(texDims.y * 128);

	for (int yIndex = 0; yIndex < simpleGridLayout.y; yIndex++)
	{
		for (int xIndex = 0; xIndex < simpleGridLayout.x; xIndex++)
		{
			float uMin = spriteWidthU * static_cast<float>(xIndex);
			float uMax = uMin + spriteWidthU;
			float vMax = 1.0f - (spriteHeightV * static_cast<float>(yIndex));
			float vMin = vMax - spriteHeightV;

			m_spriteDefs.push_back(SpriteDefinition(*this, spriteIndex, Vec2(uMin + sampleCorrectX, vMin + sampleCorrectY), Vec2(uMax - sampleCorrectX, vMax - sampleCorrectY)));
			spriteIndex++;
		}
	}
}


//
//accessors
//
Texture& SpriteSheet::GetTexture() const
{
	return m_texture;
}


int SpriteSheet::GetNumSprites() const
{
	return static_cast<int>(m_spriteDefs.size());
}


SpriteDefinition const& SpriteSheet::GetSpriteDef(int spriteIndex) const
{
	//to-do: check if index exists?
	return m_spriteDefs[spriteIndex];
}


void SpriteSheet::GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const
{
	m_spriteDefs[spriteIndex].GetUVs(out_uvAtMins, out_uvAtMaxs);
}


AABB2 SpriteSheet::GetSpriteUVs(int spriteIndex) const
{
	return m_spriteDefs[spriteIndex].GetUVs();
}
