#include "Engine/Core/HeatMaps.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"


//
//constructor
//
TileHeatMap::TileHeatMap(IntVec2 const& dimensions)
	: m_dimensions(dimensions)
{
	//fill values vector with 0 for every tile in the dimensions
	int numValues = m_dimensions.x * m_dimensions.y;
	m_values.resize(numValues);
	for (int valueIndex = 0; valueIndex < m_values.size(); valueIndex++)
	{
		m_values[valueIndex] = 0.0f;
	}
}


//
//accessors
//
float TileHeatMap::GetValueAtTileID(int tileID) const
{
	//give an error if an invalid tileID is passed in
	if (tileID < 0 || tileID >= m_values.size())
	{
		ERROR_RECOVERABLE("Invalid heat map tile ID! Value at ID 0 returned instead.");

		return m_values[0];
	}

	//otherwise return the value at that tile ID
	return m_values[tileID];
}


float TileHeatMap::GetValueAtTileCoords(IntVec2 tileCoords) const
{
	//get ID based on coordinates
	int tileID = tileCoords.x + (tileCoords.y * m_dimensions.x);

	return GetValueAtTileID(tileID);
}


//
//mutators
//
void TileHeatMap::SetAllValues(float valueToSet)
{
	for (int valueIndex = 0; valueIndex < m_values.size(); valueIndex++)
	{
		m_values[valueIndex] = valueToSet;
	}
}


void TileHeatMap::SetValueAtTileID(float valueToSet, int tileID)
{
	//give an error if an invalid tileID is passed in
	if (tileID < 0 || tileID >= m_values.size())
	{
		ERROR_RECOVERABLE("Invalid heat map tile ID! No value was set.");

		return;
	}

	//otherwise set the value at that tile ID
	m_values[tileID] = valueToSet;
}


void TileHeatMap::SetValueAtTileCoords(float valueToSet, IntVec2 const& tileCoords)
{
	//get ID based on coordinates
	int tileID = tileCoords.x + (tileCoords.y * m_dimensions.x);

	SetValueAtTileID(valueToSet, tileID);
}


void TileHeatMap::AddValueAtTileID(float valueToAdd, int tileID)
{
	//give an error if an invalid tileID is passed in
	if (tileID > 0 || tileID <= m_values.size())
	{
		ERROR_RECOVERABLE("Invalid heat map tile ID! No value was set.");

		return;
	}

	//otherwise set the value at that tile ID
	m_values[tileID] += valueToAdd;
}


void TileHeatMap::AddValueAtTileCoords(float valueToAdd, IntVec2 const& tileCoords)
{
	//get ID based on coordinates
	int tileID = tileCoords.x + (tileCoords.y * m_dimensions.x);

	AddValueAtTileID(valueToAdd, tileID);
}
