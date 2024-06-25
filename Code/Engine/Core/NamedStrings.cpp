#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Core/XmlUtils.hpp"


void NamedStrings::PopulateFromXmlElementAttributes(XmlElement const& element)
{
	XmlAttribute const* attributePtr = element.FirstAttribute();
	while (attributePtr != nullptr)
	{
		XmlAttribute const& attribute = *attributePtr;
		std::string attributeAsString = ParseXmlAttribute(element, attribute.Name(), "");
		SetValue(attribute.Name(), attributeAsString);
		attributePtr = attributePtr->Next();
	}
}


void NamedStrings::SetValue(std::string const& keyName, std::string const& newValue)
{
	m_keyValuePairs[keyName] = newValue;
}


std::string NamedStrings::GetValue(std::string const& keyName, std::string const& defaultValue) const
{
	std::string returnValue = defaultValue;
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound != m_keyValuePairs.end())
	{
		returnValue = keyFound->second;
	}
	
	return returnValue;
}


bool NamedStrings::GetValue(std::string const& keyName, bool defaultValue) const
{
	bool returnValue = defaultValue;
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound != m_keyValuePairs.end())
	{
		if (keyFound->second == "true")
		{
			returnValue = true;
		}
		else if (keyFound->second == "false")
		{
			returnValue = false;
		}
	}

	return returnValue;
}


int NamedStrings::GetValue(std::string const& keyName, int defaultValue) const
{
	int returnValue = defaultValue;
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound != m_keyValuePairs.end())
	{
		returnValue = atoi(keyFound->second.c_str());
	}

	return returnValue;
}



unsigned int NamedStrings::GetValue(std::string const& keyName, unsigned int defaultValue) const
{
	unsigned int returnValue = defaultValue;
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound != m_keyValuePairs.end())
	{
		returnValue = static_cast<unsigned int>(atoi(keyFound->second.c_str()));
	}

	return returnValue;
}


float NamedStrings::GetValue(std::string const& keyName, float defaultValue) const
{
	float returnValue = defaultValue;
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound != m_keyValuePairs.end())
	{
		returnValue = static_cast<float>(atof(keyFound->second.c_str()));
	}

	return returnValue;
}


std::string NamedStrings::GetValue(std::string const& keyName, char const* defaultValue) const
{
	std::string returnValue = defaultValue;
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound != m_keyValuePairs.end())
	{
		returnValue = keyFound->second;
	}

	return returnValue;
}


Rgba8 NamedStrings::GetValue(std::string const& keyName, Rgba8 const& defaultValue) const
{
	Rgba8 returnValue = defaultValue;
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound != m_keyValuePairs.end())
	{
		returnValue.SetFromText(keyFound->second.c_str());
	}

	return returnValue;
}


Vec2 NamedStrings::GetValue(std::string const& keyName, Vec2 const& defaultValue) const
{
	Vec2 returnValue = defaultValue;
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound != m_keyValuePairs.end())
	{
		returnValue.SetFromText(keyFound->second.c_str());
	}

	return returnValue;
}


Vec3 NamedStrings::GetValue(std::string const& keyName, Vec3 const& defaultValue) const
{
	Vec3 returnValue = defaultValue;
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound != m_keyValuePairs.end())
	{
		returnValue.SetFromText(keyFound->second.c_str());
	}

	return returnValue;
}


IntVec2 NamedStrings::GetValue(std::string const& keyName, IntVec2 const& defaultValue) const
{
	IntVec2 returnValue = defaultValue;
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound != m_keyValuePairs.end())
	{
		returnValue.SetFromText(keyFound->second.c_str());
	}

	return returnValue;
}


EulerAngles NamedStrings::GetValue(std::string const& keyName, EulerAngles const& defaultValue) const
{
	EulerAngles returnValue = defaultValue;
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound != m_keyValuePairs.end())
	{
		returnValue.SetFromText(keyFound->second.c_str());
	}

	return returnValue;
}
