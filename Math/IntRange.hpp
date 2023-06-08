#pragma once


struct IntRange
{
public:
	int m_min = 0;
	int m_max = 0;

	//static variables
	static IntRange const ZERO;
	static IntRange const ONE;
	static IntRange const ZERO_TO_ONE;

	//public member functions
public:
	//constructors and destructor
	IntRange() {}
	~IntRange() {}
	explicit IntRange(int min, int max);

	//accessors
	bool IsOnRange(int value);
	bool IsOverlappingWith(IntRange range);

	//const operators
	bool operator==(IntRange const& toCompare) const;
	bool operator!=(IntRange const& toCompare) const;

	//non-const operators
	void operator=(IntRange const& toCopy);
};
