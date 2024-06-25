#include "Engine/Core/HashedCaseInsensitiveString.hpp"


//
//constructors
//
HashedCaseInsensitiveString::HashedCaseInsensitiveString(std::string const& text)
	: m_originalText(text)
	, m_hash(CalculateHashForText(text))
{
}


HashedCaseInsensitiveString::HashedCaseInsensitiveString(char const* text)
	: m_originalText(text)
	, m_hash(CalculateHashForText(text))
{
}


//
//const operators
//
bool HashedCaseInsensitiveString::operator<(HashedCaseInsensitiveString const& toCompare) const
{
	if (m_hash < toCompare.m_hash)
	{
		return true;
	}
	else if (m_hash > toCompare.m_hash)
	{
		return false;
	}
	else
	{
		return _stricmp(m_originalText.c_str(), toCompare.m_originalText.c_str()) < 0;
	}
}


bool HashedCaseInsensitiveString::operator>(HashedCaseInsensitiveString const& toCompare) const
{
	if (m_hash > toCompare.m_hash)
	{
		return true;
	}
	else if (m_hash < toCompare.m_hash)
	{
		return false;
	}
	else
	{
		return _stricmp(m_originalText.c_str(), toCompare.m_originalText.c_str()) > 0;
	}
}


bool HashedCaseInsensitiveString::operator==(HashedCaseInsensitiveString const& toCompare) const
{
	if (m_hash != toCompare.m_hash)
	{
		return false;
	}
	else
	{
		return _stricmp(m_originalText.c_str(), toCompare.m_originalText.c_str()) == 0;
	}
}


bool HashedCaseInsensitiveString::operator!=(HashedCaseInsensitiveString const& toCompare) const
{
	return !(*this == toCompare);
}


bool HashedCaseInsensitiveString::operator==(char const* toCompare) const
{
	return _stricmp(m_originalText.c_str(), toCompare) == 0;
}


//
//non-const operators
//
void HashedCaseInsensitiveString::operator=(char const* toAssign)
{
	m_originalText = toAssign;
	m_hash = CalculateHashForText(toAssign);
}


//
//static functions
//
unsigned int HashedCaseInsensitiveString::CalculateHashForText(std::string const& text)
{
	return CalculateHashForText(text.c_str());
}


unsigned int HashedCaseInsensitiveString::CalculateHashForText(char const* text)
{
	unsigned int hash = 0;
	for (char const* readPos = text; *readPos != '\0'; readPos++)
	{
		hash *= 31;
		hash += static_cast<unsigned int>(tolower(*readPos));
	}

	return hash;
}
