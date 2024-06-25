#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/MathUtils.hpp"


//
//constructors
//
Rgba8::Rgba8(unsigned char initialR, unsigned char initialG, unsigned char initialB, unsigned char initialA)
	: r(initialR)
	, g(initialG)
	, b(initialB)
	, a(initialA)
{
}


//
//accessors
//
void Rgba8::GetAsFloats(float* colorAsFloats) const
{
	colorAsFloats[0] = RangeMap(static_cast<float>(r), 0.0f, 255.0f, 0.0f, 1.0f);
	colorAsFloats[1] = RangeMap(static_cast<float>(g), 0.0f, 255.0f, 0.0f, 1.0f);
	colorAsFloats[2] = RangeMap(static_cast<float>(b), 0.0f, 255.0f, 0.0f, 1.0f);
	colorAsFloats[3] = RangeMap(static_cast<float>(a), 0.0f, 255.0f, 0.0f, 1.0f);
}


//
//mutators
//
void Rgba8::SetFromText(char const* text)
{
	Strings splitText = SplitStringOnDelimiter(text, ',');

	if (splitText.size() == 3)		//assume alpha to be 255 if there's three entries
	{
		r = static_cast<unsigned char>(atoi(splitText[0].c_str()));
		g = static_cast<unsigned char>(atoi(splitText[1].c_str()));
		b = static_cast<unsigned char>(atoi(splitText[2].c_str()));
		a = 255;
	}
	else if (splitText.size() == 4)	//take alpha if there's four entries
	{
		r = static_cast<unsigned char>(atoi(splitText[0].c_str()));
		g = static_cast<unsigned char>(atoi(splitText[1].c_str()));
		b = static_cast<unsigned char>(atoi(splitText[2].c_str()));
		a = static_cast<unsigned char>(atoi(splitText[3].c_str()));
	}
	else							//invalid if there's not either 3 or 4 entries, just don't change it in this case
	{
		return;
	}
}


void Rgba8::SetFromFloats(float* colorAsFloats)
{
	r = static_cast<unsigned char>(colorAsFloats[0] * 255.0f);
	g = static_cast<unsigned char>(colorAsFloats[1] * 255.0f);
	b = static_cast<unsigned char>(colorAsFloats[2] * 255.0f);
	a = static_cast<unsigned char>(colorAsFloats[3] * 255.0f);
}


//
//operators
//
bool Rgba8::operator==(Rgba8 const& compare) const
{
	if (r == compare.r && g == compare.g && b == compare.b && a == compare.a) return true;
	else return false;
}


Rgba8 Rgba8::operator+(Rgba8 const& toAdd) const
{
	unsigned char newR = static_cast<unsigned char>(GetClamped(r + toAdd.r, 0, 255));
	unsigned char newG = static_cast<unsigned char>(GetClamped(g + toAdd.g, 0, 255));
	unsigned char newB = static_cast<unsigned char>(GetClamped(b + toAdd.b, 0, 255));
	unsigned char newA = static_cast<unsigned char>(GetClamped(a + toAdd.a, 0, 255));

	return Rgba8(newR, newG, newB, newA);
}


Rgba8 Rgba8::operator-(Rgba8 const& toSubtract) const
{
	unsigned char newR = static_cast<unsigned char>(GetClamped(r - toSubtract.r, 0, 255));
	unsigned char newG = static_cast<unsigned char>(GetClamped(g - toSubtract.g, 0, 255));
	unsigned char newB = static_cast<unsigned char>(GetClamped(b - toSubtract.b, 0, 255));
	unsigned char newA = static_cast<unsigned char>(GetClamped(a - toSubtract.a, 0, 255));

	return Rgba8(newR, newG, newB, newA);
}
