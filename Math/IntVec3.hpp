#pragma once


struct IntVec3
{
//public member variables
public:
	int x = 0;
	int y = 0;
	int z = 0;

//public member functions
public:
	//constructors and destructor
	IntVec3() {}
	~IntVec3() {}
	IntVec3(IntVec3 const& copyFrom);
	explicit IntVec3(int initialX, int initialY, int initialZ);
};
