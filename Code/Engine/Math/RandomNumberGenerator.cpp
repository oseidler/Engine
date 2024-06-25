#include "Engine/Math/RandomNumberGenerator.hpp"
#include "ThirdParty/Squirrel/RawNoise.hpp"
#include <time.h>


//
//random integer functions
//
int RandomNumberGenerator::RollRandomIntLessThan(int maxNotInclusive)
{
	//return rand() % maxNotInclusive;

	unsigned int randomInt = Get1dNoiseUint(m_position, m_seed);
	m_position++;
	return randomInt % maxNotInclusive;
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
	//return float(rand()) / float(RAND_MAX);
	
	float randomFloat = Get1dNoiseZeroToOne(m_position, m_seed);
	m_position++;
	
	return randomFloat;
}


float RandomNumberGenerator::RollRandomFloatInRange(float minInclusive, float maxInclusive)
{
	float range = maxInclusive - minInclusive;
	return minInclusive + RollRandomFloatZeroToOne() * range;
}


//
//seeding functions
//
void RandomNumberGenerator::SeedRNGWithTime()
{
	/*int currentTime = static_cast<int>(time(NULL));

	srand(currentTime);*/

	m_seed = static_cast<int>(time(NULL));
}


void RandomNumberGenerator::SeedRNG(unsigned int seed)
{
	//srand(seed);

	m_seed = seed;
}
