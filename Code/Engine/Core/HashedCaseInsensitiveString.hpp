#pragma once
#include <string>


class HashedCaseInsensitiveString
{
//public member functions
public:
	//constructors
	HashedCaseInsensitiveString() = default;
	HashedCaseInsensitiveString(std::string const& text);
	HashedCaseInsensitiveString(char const* text);

	//accessors
	std::string  GetOriginalText() const { return m_originalText; }
	unsigned int GetHash() const { return m_hash; }

	//const operators
	bool operator<(HashedCaseInsensitiveString const& toCompare) const;
	bool operator>(HashedCaseInsensitiveString const& toCompare) const;
	bool operator==(HashedCaseInsensitiveString const& toCompare) const;
	bool operator!=(HashedCaseInsensitiveString const& toCompare) const;
	bool operator==(char const* toCompare) const;

	//non-const operators
	void operator=(char const* toAssign);

	//static functions
	static unsigned int CalculateHashForText(std::string const& text);
	static unsigned int CalculateHashForText(char const* text);

//private member variables
private:
	std::string  m_originalText;
	unsigned int m_hash = 0;
};

typedef HashedCaseInsensitiveString HCIString;
