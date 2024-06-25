#include "Engine/Math/IntVec3.hpp"


//
//constructors
//
IntVec3::IntVec3(IntVec3 const& copyFrom)
	: x(copyFrom.x)
	, y(copyFrom.y)
	, z(copyFrom.z)
{
}


IntVec3::IntVec3(int initialX, int initialY, int initialZ)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
{
}
