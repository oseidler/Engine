#pragma once


struct Vec2
{
//public member variables
public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	float x = 0.f;
	float y = 0.f;

//public member functions
public:
	//constructors and destructor
	~Vec2() {}											// destructor (do nothing)
	Vec2() {}											// default constructor (do nothing)
	Vec2(Vec2 const& copyFrom);							// copy constructor (from another vec2)
	explicit Vec2(float initialX, float initialY);		// explicit constructor (from x, y)

	//creation functions
	static Vec2 const MakeFromPolarDegrees(float orientationDegrees, float length = 1.f);
	static Vec2 const MakeFromPolarRadians(float orientationRadians, float length = 1.f);

	//accessors
	float	   GetLength() const;
	float	   GetLengthSquared() const;
	float	   GetOrientationDegrees() const;
	float	   GetOrientationRadians() const;
	Vec2 const GetRotated90Degrees() const;
	Vec2 const GetRotatedMinus90Degrees() const;
	Vec2 const GetRotatedDegrees(float degrees) const;
	Vec2 const GetRotatedRadians(float radians) const;
	Vec2 const GetClamped(float maxLength) const;
	Vec2 const GetNormalized() const;
	Vec2 const GetReflected(Vec2 const& surfaceNormal) const;

	//mutators
	void  SetOrientationDegrees(float newOrientationDegrees);
	void  SetOrientationRadians(float newOrientationRadians);
	void  SetPolarDegrees(float newOrienationDegrees, float newLength);
	void  SetPolarRadians(float newOrientationRadians, float newLength);
	void  Rotate90Degrees();
	void  RotateMinus90Degrees();
	void  RotateDegrees(float deltaDegrees);
	void  RotateRadians(float deltaRadians);
	void  SetLength(float newLength);
	void  ClampLength(float maxLength);
	void  Normalize();
	float NormalizeAndGetPreviousLength();
	void  Reflect(Vec2 const& surfaceNormal);
	void  SetFromText(char const* text);

	//const operators
	bool	   operator==( Vec2 const& compare ) const;			// vec2 == vec2
	bool	   operator!=( Vec2 const& compare ) const;			// vec2 != vec2
	Vec2 const operator+( Vec2 const& vecToAdd ) const;			// vec2 + vec2
	Vec2 const operator-( Vec2 const& vecToSubtract ) const;	// vec2 - vec2
	Vec2 const operator-() const;								// -vec2, i.e. "unary negation"
	Vec2 const operator*( float uniformScale ) const;			// vec2 * float
	Vec2 const operator*( Vec2 const& vecToMultiply ) const;	// vec2 * vec2
	Vec2 const operator/( float inverseScale ) const;			// vec2 / float

	//non-const operators
	void operator+=( Vec2 const& vecToAdd );				// vec2 += vec2
	void operator-=( Vec2 const& vecToSubtract );		// vec2 -= vec2
	void operator*=( float const uniformScale );			// vec2 *= float
	void operator/=( float const uniformDivisor );		// vec2 /= float
	void operator=( Vec2 const& copyFrom );				// vec2 = vec2

	//standalone "friend" functions that are conceptually, but not actually, part of Vec2::
	friend Vec2 const operator*( float uniformScale, Vec2 const& vecToScale );	// float * vec2
};
