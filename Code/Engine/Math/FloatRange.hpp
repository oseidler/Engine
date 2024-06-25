#pragma once


struct FloatRange
{
//public member variables
public:
	float m_min = 0.0f;
	float m_max = 0.0f;

	//static variables
	static FloatRange const ZERO;
	static FloatRange const ONE;
	static FloatRange const ZERO_TO_ONE;

//public member functions
public:
	//constructors and destructor
	FloatRange() {}
	~FloatRange() {}
	explicit FloatRange(float min, float max);

	//accessors
	bool IsOnRange(float value);
	bool IsOverlappingWith(FloatRange range);

	//mutators
	void SetFromText(char const* text);

	//const operators
	bool operator==(FloatRange const& toCompare) const;
	bool operator!=(FloatRange const& toCompare) const;

	//non-const operators
	void operator=(FloatRange const& toCopy);
};
