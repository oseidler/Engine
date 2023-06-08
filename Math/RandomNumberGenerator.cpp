#include "Engine/Math/RandomNumberGenerator.hpp"
#include <stdlib.h>


//
//random integer functions
//
int RandomNumberGenerator::RollRandomIntLessThan(int maxNotInclusive)
{
	return rand() % maxNotInclusive;
}


int RandomNumberGenerator::RollRandomIntInRange(int minInclusive, int maxInclusive)
{
	int possibilities = 1 + (maxInclusive - minInclusive);
	return RollRandomIntLessThan(possibilities) + minInclusive;
}


//
//random float functions
//
float RandomNumberGenerator::RollRandomFloatZeroToOne()
{
	return float(rand()) / float(RAND_MAX);
}


float RandomNumberGenerator::RollRandomFloatInRange(float minInclusive, float maxInclusive)
{
	float range = maxInclusive - minInclusive;
	return minInclusive + RollRandomFloatZeroToOne() * range;
}
