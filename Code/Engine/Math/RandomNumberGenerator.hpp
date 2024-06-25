#pragma once

class RandomNumberGenerator
{
//public member functions
public:
	//random integer functions
	int RollRandomIntLessThan(int maxNotInclusive);
	int RollRandomIntInRange(int minInclusive, int maxInclusive);

	//random float functions
	float RollRandomFloatZeroToOne();
	float RollRandomFloatInRange(float minInclusive, float maxInclusive);

	//seeding functions
	void SeedRNGWithTime();
	void SeedRNG(unsigned int seed);

//public member variables
public:
	unsigned int m_seed = 0;
	int m_position = 0;
};
