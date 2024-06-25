#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"


int ParseXmlAttribute(XmlElement const& element, char const* attributeName, int defaultValue)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		return atoi(valueAsText);
	}

	//otherwise return the default value
	return defaultValue;
}


char ParseXmlAttribute(XmlElement const& element, char const* attributeName, char defaultValue)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		return *valueAsText;
	}

	//otherwise return the default value
	return defaultValue;
}


bool ParseXmlAttribute(XmlElement const& element, char const* attributeName, bool defaultValue)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		std::string valueAsString = valueAsText;
		if (valueAsString == "true" || valueAsString == "True" || valueAsString == "TRUE")
		{
			return true;
		}
		else if (valueAsString == "false" || valueAsString == "False" || valueAsString == "FALSE")
		{
			return false;
		}
		
	}

	//otherwise return the default value
	return defaultValue;
}


float ParseXmlAttribute(XmlElement const& element, char const* attributeName, float defaultValue)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		return static_cast<float>(atof(valueAsText));
	}

	//otherwise return the default value
	return defaultValue;
}


Rgba8 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Rgba8 const& defaultValue)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		Rgba8 color;
		color.SetFromText(valueAsText);
		return color;
	}

	//otherwise return the default value
	return defaultValue;
}


Vec2 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Vec2 const& defaultValue)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		Vec2 vec;
		vec.SetFromText(valueAsText);
		return vec;
	}

	//otherwise return the default value
	return defaultValue;
}


Vec3 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Vec3 const& defaultValue)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		Vec3 vec;
		vec.SetFromText(valueAsText);
		return vec;
	}

	//otherwise return the default value
	return defaultValue;
}



AABB2 ParseXmlAttribute(XmlElement const& element, char const* attributeName, AABB2 const& defaultValue)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		AABB2 aabb;
		aabb.SetFromText(valueAsText);
		return aabb;
	}

	//otherwise return the default value
	return defaultValue;
}


EulerAngles ParseXmlAttribute(XmlElement const& element, char const* attributeName, EulerAngles const& defaultValue)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		EulerAngles angles;
		angles.SetFromText(valueAsText);
		return angles;
	}

	//otherwise return the default value
	return defaultValue;
}


FloatRange ParseXmlAttribute(XmlElement const& element, char const* attributeName, FloatRange const& defaultValue)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		FloatRange range;
		range.SetFromText(valueAsText);
		return range;
	}

	//otherwise return the default value
	return defaultValue;
}



IntVec2 ParseXmlAttribute(XmlElement const& element, char const* attributeName, IntVec2 const& defaultValue)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		IntVec2 vec;
		vec.SetFromText(valueAsText);
		return vec;
	}

	//otherwise return the default value
	return defaultValue;
}


std::string ParseXmlAttribute(XmlElement const& element, char const* attributeName, std::string const& defaultValue)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		return valueAsText;
	}

	//otherwise return the default value
	return defaultValue;
}


Strings ParseXmlAttribute(XmlElement const& element, char const* attributeName, Strings const& defaultValues)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		Strings returnStrings;
		returnStrings.push_back(valueAsText);
		return returnStrings;
	}

	//otherwise return the default value
	return defaultValues;
}


std::string ParseXmlAttribute(XmlElement const& element, char const* attributeName, char const* defaultValue)
{
	char const* valueAsText = element.Attribute(attributeName);
	//if there is a valid value for that attribute, return it
	if (valueAsText != nullptr)
	{
		return valueAsText;
	}

	//otherwise return the default value
	return defaultValue;
}
