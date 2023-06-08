#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include <math.h>


//
//constructors
//
IntVec2::IntVec2(IntVec2 const& copyFrom)
	: x(copyFrom.x)
	, y(copyFrom.y)
{
}


IntVec2::IntVec2(int initialX, int initialY)
	: x(initialX)
	, y(initialY)
{
}


//
//accessors
//
float IntVec2::GetLength() const
{
	float fX = static_cast<float>(x);
	float fY = static_cast<float>(y);
	
	return sqrtf(fX * fX + fY * fY);
}


int IntVec2::GetLengthSquared() const
{
	return (x * x) + (y * y);
}


int IntVec2::GetTaxicabLength() const
{
	return abs(x) + abs(y);
}


float IntVec2::GetOrientationDegrees() const
{
	float fX = static_cast<float>(x);
	float fY = static_cast<float>(y);
	
	return Atan2Degrees(fY, fX);
}


float IntVec2::GetOrientationRadians() const
{
	float fX = static_cast<float>(x);
	float fY = static_cast<float>(y);
	
	return atan2f(fY, fX);
}


IntVec2 const IntVec2::GetRotated90Degrees() const
{
	return IntVec2(-y, x);
}


IntVec2 const IntVec2::GetRotatedMinus90Degrees() const
{
	return IntVec2(y, -x);
}


//
//mutators
//
void IntVec2::Rotate90Degrees()
{
	int temp = x;
	x = -y;
	y = temp;
}


void IntVec2::RotateMinus90Degrees()
{
	int temp = y;
	y = -x;
	x = temp;
}


void IntVec2::SetFromText(char const* text)
{
	Strings splitText = SplitStringOnDelimiter(text, ',');

	//return without changing the vector if there isn't a valid number of entries
	if (splitText.size() != 2)
	{
		return;
	}

	//set x and y to the first and second string, respectively
	x = atoi(splitText[0].c_str());
	y = atoi(splitText[1].c_str());
}


//
//const operators
//
bool IntVec2::operator==(IntVec2 const& intVecToCompare) const
{
	if (x == intVecToCompare.x && y == intVecToCompare.y) return true;
	else return false;
}


bool IntVec2::operator!=(IntVec2 const& intVecToCompare) const
{
	if (x != intVecToCompare.x || y != intVecToCompare.y) return true;
	else return false;
}


bool IntVec2::operator<(IntVec2 const& intVecToCompare) const
{
	if (y < intVecToCompare.y)
	{
		return true;
	}
	else if (y > intVecToCompare.y)
	{
		return false;
	}
	else
	{
		return x < intVecToCompare.x;
	}
}


IntVec2 const IntVec2::operator+(IntVec2 const& intVecToAdd) const
{
	return IntVec2(x + intVecToAdd.x, y + intVecToAdd.y);
}


IntVec2 const IntVec2::operator-(IntVec2 const& intVecToSubtract) const
{
	return IntVec2(x - intVecToSubtract.x, y - intVecToSubtract.y);
}
