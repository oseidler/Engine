#include "Engine/Math/FloatRange.hpp"
#include "Engine/Core/EngineCommon.hpp"


//
//static const objects
//
FloatRange const FloatRange::ZERO = FloatRange(0.0f, 0.0f);
FloatRange const FloatRange::ONE = FloatRange(1.0f, 1.0f);
FloatRange const FloatRange::ZERO_TO_ONE = FloatRange(0.0f, 1.0f);


//
//explicit constructor
//
FloatRange::FloatRange(float min, float max)
	: m_min(min)
	, m_max(max)
{
}


//
//accessors
//
bool FloatRange::IsOnRange(float value)
{
	if (value <= m_max && value >= m_min)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool FloatRange::IsOverlappingWith(FloatRange range)
{
	//ranges are overlapping if one of them has a min or max inside the other
	if (IsOnRange(range.m_min) || IsOnRange(range.m_max) || range.IsOnRange(m_min) || range.IsOnRange(m_max))
	{
		return true;
	}
	else
	{
		return false;
	}
}


//
//mutators
//
void FloatRange::SetFromText(char const* text)
{
	Strings splitText = SplitStringOnDelimiter(text, '~');

	//return without changing the vector if there isn't a valid number of entries
	if (splitText.size() != 2)
	{
		return;
	}

	//set x and y to the first and second string, respectively
	m_min = static_cast<float>(atof(splitText[0].c_str()));
	m_max = static_cast<float>(atof(splitText[1].c_str()));
}


//
//const operators
//
bool FloatRange::operator==(FloatRange const& toCompare) const
{
	if (m_min == toCompare.m_min && m_max == toCompare.m_max)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool FloatRange::operator!=(FloatRange const& toCompare) const
{
	if (m_min != toCompare.m_min || m_max != toCompare.m_max)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//
//non-const operators
//
void FloatRange::operator=(FloatRange const& toCopy)
{
	m_min = toCopy.m_min;
	m_max = toCopy.m_max;
}
