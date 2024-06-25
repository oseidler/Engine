#include "Engine/Renderer/SpriteAnimDefinition.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Math/MathUtils.hpp"


//
//constructor
//
SpriteAnimDefinition::SpriteAnimDefinition(SpriteSheet const& spriteSheet, int startSpriteIndex, int endSpriteIndex, float framesPerSecond, SpriteAnimPlaybackType playbackType)
	: m_spriteSheet(spriteSheet)
	, m_startSpriteIndex(startSpriteIndex)
	, m_endSpriteIndex(endSpriteIndex)
	, m_secondsPerFrame(1.0f / framesPerSecond)
	, m_playbackType(playbackType)
{
}


//
//accessors
//
SpriteDefinition const& SpriteAnimDefinition::GetSpriteDefAtTime(float seconds) const
{
	if (m_playbackType == SpriteAnimPlaybackType::ONCE)
	{
		int animFrame = RoundDownToInt(seconds / m_secondsPerFrame);
		int spriteIndex = GetClamped(m_startSpriteIndex + animFrame, m_startSpriteIndex, m_endSpriteIndex);
		return m_spriteSheet.GetSpriteDef(spriteIndex);
	}
	else if (m_playbackType == SpriteAnimPlaybackType::LOOP)
	{
		int animFrame = RoundDownToInt(seconds / m_secondsPerFrame);
		animFrame %= m_endSpriteIndex - m_startSpriteIndex + 1;
		int spriteIndex = GetClamped(m_startSpriteIndex + animFrame, m_startSpriteIndex, m_endSpriteIndex);
		return m_spriteSheet.GetSpriteDef(spriteIndex);
	}
	else if (m_playbackType == SpriteAnimPlaybackType::PINGPONG)
	{
		//Inaccurate, hangs on final frame too long
		int animFrame = RoundDownToInt(seconds / m_secondsPerFrame);
		animFrame %= (m_endSpriteIndex - m_startSpriteIndex + 1) * 2;
		if (animFrame > m_endSpriteIndex - m_startSpriteIndex - 1)
		{
			animFrame = (m_endSpriteIndex - m_startSpriteIndex + 1) * 2 - animFrame;
		}
		int spriteIndex = GetClamped(m_startSpriteIndex + animFrame, m_startSpriteIndex, m_endSpriteIndex);
		return m_spriteSheet.GetSpriteDef(spriteIndex);
	}

	//shouldn't ever get here, just return the start index if you somehow do
	return m_spriteSheet.GetSpriteDef(m_startSpriteIndex);
}
