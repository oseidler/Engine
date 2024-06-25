#pragma once
#include "Engine/Math/Vec2.hpp"


struct AABB2
{
//public member variables
public:
	Vec2 m_mins;
	Vec2 m_maxs;

//public member functions
public:
	//constructors
	AABB2() {}
	~AABB2() {}
	AABB2(AABB2 const& copyFrom);
	explicit AABB2(float minX, float minY, float maxX, float maxY);
	explicit AABB2(Vec2 const& initialMins, Vec2 const& initialMaxs);

	//accessors
	bool	    IsPointInside(Vec2 const& point) const;
	Vec2 const  GetCenter() const;
	Vec2 const  GetDimensions() const;
	Vec2 const  GetNearestPoint(Vec2 const& point) const;
	Vec2 const  GetPointAtUV(Vec2 const& uv) const;
	Vec2 const  GetUVForPoint(Vec2 const& point) const;
	AABB2 const GetAABB2Within(Vec2 const& mins, Vec2 const& maxs) const;

	//mutators
	void Translate(Vec2 const& translation);
	void SetCenter(Vec2 const& newCenter);
	void SetDimensions(Vec2 const& newDimensions);
	void StretchToIncludePoint(Vec2 const& point);
	void SetFromText(char const* text);
	void operator=(AABB2 const& copyFrom);
};
