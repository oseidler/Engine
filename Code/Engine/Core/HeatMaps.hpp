#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/IntVec2.hpp"


class TileHeatMap
{
//public member functions
public:
	//constructor
	TileHeatMap(IntVec2 const& dimensions);

	//accessors
	float GetValueAtTileID(int tileID) const;
	float GetValueAtTileCoords(IntVec2 tileCoords) const;

	//mutators
	void SetAllValues(float valueToSet);
	void SetValueAtTileID(float valueToSet, int tileID);
	void SetValueAtTileCoords(float valueToSet, IntVec2 const& tileCoords);
	void AddValueAtTileID(float valueToAdd, int tileID);
	void AddValueAtTileCoords(float valueToAdd, IntVec2 const& tileCoords);

//public member variables
public:
	std::vector<float> m_values;
	IntVec2			   m_dimensions;
};
