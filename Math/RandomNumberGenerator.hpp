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
};
