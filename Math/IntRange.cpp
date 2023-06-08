#include "Engine/Math/IntRange.hpp"


//
//static const objects
//
IntRange const IntRange::ZERO = IntRange(0, 0);
IntRange const IntRange::ONE = IntRange(1, 1);
IntRange const IntRange::ZERO_TO_ONE = IntRange(0, 1);


//
//explicit constructor
//
IntRange::IntRange(int min, int max)
	: m_min(min)
	, m_max(max)
{
}


//
//accessors
//
bool IntRange::IsOnRange(int value)
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


bool IntRange::IsOverlappingWith(IntRange range)
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
//const operators
//
bool IntRange::operator==(IntRange const& toCompare) const
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


bool IntRange::operator!=(IntRange const& toCompare) const
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
void IntRange::operator=(IntRange const& toCopy)
{
	m_min = toCopy.m_min;
	m_max = toCopy.m_max;
}
