#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/AABB2.hpp"


class SpriteSheet;
class Texture;


class SpriteDefinition
{
//public member functions
public:
	//constructor
	SpriteDefinition() = default;
	explicit SpriteDefinition(SpriteSheet const& spriteSheet, int spriteIndex, Vec2 const& uvAtMins, Vec2 const& uvAtMaxs);

	//accessors
	void			   GetUVs(Vec2& out_uvAtMins,Vec2& out_uvAtMaxs ) const;
	AABB2			   GetUVs() const;
	SpriteSheet const& GetSpriteSheet() const;
	Texture&		   GetTexture() const;
	float			   GetAspect() const;


//protected member variables
protected:
	SpriteSheet const& m_spriteSheet;
	int				   m_spriteIndex = -1;
	Vec2			   m_uvAtMins = Vec2(0.0f, 0.0f);
	Vec2			   m_uvAtMaxs = Vec2(1.0f, 1.0f);
};
