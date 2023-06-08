#pragma once


//forward declarations
class SpriteDefinition;
class SpriteSheet;


//enum class for types of animation playback
enum class SpriteAnimPlaybackType
{
	ONCE,
	LOOP,
	PINGPONG
};


class SpriteAnimDefinition
{
//public member functions
public:
	//constructor
	SpriteAnimDefinition(SpriteSheet const& spriteSheet, int startSpriteIndex, int endSpriteIndex, float framesPerSecond = 20.0f, SpriteAnimPlaybackType playbackType = SpriteAnimPlaybackType::LOOP);

	//accessors
	SpriteDefinition const& GetSpriteDefAtTime(float seconds) const;

//private member variables
private:
	SpriteSheet const&	   m_spriteSheet;
	int					   m_startSpriteIndex = -1;
	int					   m_endSpriteIndex = -1;
	float				   m_secondsPerFrame = 0.05f;
	SpriteAnimPlaybackType m_playbackType = SpriteAnimPlaybackType::LOOP;
};
