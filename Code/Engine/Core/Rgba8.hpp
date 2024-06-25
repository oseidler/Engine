#pragma once


struct Rgba8
{
//public member variables
public:
	unsigned char r = 255;
	unsigned char g = 255;
	unsigned char b = 255;
	unsigned char a = 255;

//public member functions
public:
	//constructors and destructor
	Rgba8() {}
	~Rgba8() {}
	explicit Rgba8(unsigned char initialR, unsigned char initialG, unsigned char initialB, unsigned char initialA = 255);

	//accessors
	void GetAsFloats(float* colorAsFloats) const;

	//mutators
	void SetFromText(char const* text);
	void SetFromFloats(float* colorAsFloats);

	//const operators
	bool  operator==(Rgba8 const& compare) const;
	Rgba8 operator+(Rgba8 const& toAdd) const;
	Rgba8 operator-(Rgba8 const& toSubtract) const;
};
