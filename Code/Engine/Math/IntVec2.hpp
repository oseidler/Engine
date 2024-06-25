#pragma once


struct IntVec2
{
//public member variables
public:
	int x = 0;
	int y = 0;

//public member functions
public:
	//constructors and destructor
	IntVec2() {}
	~IntVec2() {}
	IntVec2(IntVec2 const& copyFrom);
	explicit IntVec2(int initialX, int initialY);

	//accessors
	float		  GetLength() const;
	int			  GetLengthSquared() const;
	int			  GetTaxicabLength() const;
	float		  GetOrientationDegrees() const;
	float		  GetOrientationRadians() const;
	IntVec2 const GetRotated90Degrees() const;
	IntVec2 const GetRotatedMinus90Degrees() const;
	
	//mutators
	void Rotate90Degrees();
	void RotateMinus90Degrees();
	void SetFromText(char const* text);

	//const operators
	bool		  operator==(IntVec2 const& intVecToCompare) const;
	bool		  operator!=(IntVec2 const& intVecToCompare) const;
	bool		  operator<(IntVec2 const& intVecToCompare) const;
	IntVec2 const operator+(IntVec2 const& intVecToAdd) const;
	IntVec2 const operator-(IntVec2 const& intVecToSubtract) const;
	//#ToDo - finish writing operators for intvec2

	//non-const operators
	

};
