#include "Engine/Core/NamedProperties.hpp"
#include "Engine/Core/EngineCommon.hpp"


void NamedProperties::PopulateFromXmlElementAttributes(XmlElement const& element)
{
	XmlAttribute const* attributePtr = element.FirstAttribute();
	while (attributePtr != nullptr)
	{
		//store as string, get later as type
		XmlAttribute const& attribute = *attributePtr;
		std::string attributeAsString = ParseXmlAttribute(element, attribute.Name(), "");
		SetValue(attribute.Name(), attributeAsString);
		attributePtr = attributePtr->Next();
	}
}


void NamedProperties::SetValue(std::string const& keyName, char const* value)
{
	std::string valueAsString = value;
	NamedPropertyByType<std::string>* propertyByType = new NamedPropertyByType<std::string>(valueAsString);
	m_keyValuePairs[keyName] = propertyByType;
}


std::string NamedProperties::GetValue(std::string const& keyName, char const* defaultValue) const
{
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	NamedPropertyBase* property = keyFound->second;
	NamedPropertyByType<std::string>* propertyByType = dynamic_cast<NamedPropertyByType<std::string>*>(property);
	if (propertyByType == nullptr)
	{
		ERROR_RECOVERABLE("Type of property did not match type asked for!");
		return defaultValue;
	}

	return propertyByType->m_value.c_str();
}


bool NamedProperties::GetValue(std::string const& keyName, bool defaultValue) const
{
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	NamedPropertyBase* property = keyFound->second;
	NamedPropertyByType<bool>* propertyAsBool = dynamic_cast<NamedPropertyByType<bool>*>(property);
	if (propertyAsBool == nullptr)
	{
		NamedPropertyByType<std::string>* propertyAsString = dynamic_cast<NamedPropertyByType<std::string>*>(property);
		if (propertyAsString == nullptr)
		{
			ERROR_RECOVERABLE("Type of property did not match type asked for!");
			return defaultValue;
		}

		if (propertyAsString->m_value == "true")
		{
			return true;
		}
		else if (propertyAsString->m_value == "false")
		{
			return false;
		}
	}

	return propertyAsBool->m_value;
}


int NamedProperties::GetValue(std::string const& keyName, int defaultValue) const
{
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	NamedPropertyBase* property = keyFound->second;
	NamedPropertyByType<int>* propertyAsInt = dynamic_cast<NamedPropertyByType<int>*>(property);
	if (propertyAsInt == nullptr)
	{
		NamedPropertyByType<std::string>* propertyAsString = dynamic_cast<NamedPropertyByType<std::string>*>(property);
		if (propertyAsString == nullptr)
		{
			ERROR_RECOVERABLE("Type of property did not match type asked for!");
			return defaultValue;
		}

		return atoi(propertyAsString->m_value.c_str());
	}

	return propertyAsInt->m_value;
}


unsigned int NamedProperties::GetValue(std::string const& keyName, unsigned int defaultValue) const
{
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	NamedPropertyBase* property = keyFound->second;
	NamedPropertyByType<unsigned int>* propertyAsInt = dynamic_cast<NamedPropertyByType<unsigned int>*>(property);
	if (propertyAsInt == nullptr)
	{
		NamedPropertyByType<std::string>* propertyAsString = dynamic_cast<NamedPropertyByType<std::string>*>(property);
		if (propertyAsString == nullptr)
		{
			ERROR_RECOVERABLE("Type of property did not match type asked for!");
			return defaultValue;
		}

		return static_cast<int>(atoi(propertyAsString->m_value.c_str()));
	}

	return propertyAsInt->m_value;
}


float NamedProperties::GetValue(std::string const& keyName, float defaultValue) const
{
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	NamedPropertyBase* property = keyFound->second;
	NamedPropertyByType<float>* propertyAsFloat = dynamic_cast<NamedPropertyByType<float>*>(property);
	if (propertyAsFloat == nullptr)
	{
		NamedPropertyByType<std::string>* propertyAsString = dynamic_cast<NamedPropertyByType<std::string>*>(property);
		if (propertyAsString == nullptr)
		{
			ERROR_RECOVERABLE("Type of property did not match type asked for!");
			return defaultValue;
		}

		return static_cast<float>(atof(propertyAsString->m_value.c_str()));
	}

	return propertyAsFloat->m_value;
}


Rgba8 NamedProperties::GetValue(std::string const& keyName, Rgba8 const& defaultValue) const
{
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	NamedPropertyBase* property = keyFound->second;
	NamedPropertyByType<Rgba8>* propertyAsRgba8 = dynamic_cast<NamedPropertyByType<Rgba8>*>(property);
	if (propertyAsRgba8 == nullptr)
	{
		NamedPropertyByType<std::string>* propertyAsString = dynamic_cast<NamedPropertyByType<std::string>*>(property);
		if (propertyAsString == nullptr)
		{
			ERROR_RECOVERABLE("Type of property did not match type asked for!");
			return defaultValue;
		}

		Rgba8 returnRgba8 = defaultValue;
		returnRgba8.SetFromText(propertyAsString->m_value.c_str());
		return returnRgba8;
	}

	return propertyAsRgba8->m_value;
}


Vec2 NamedProperties::GetValue(std::string const& keyName, Vec2 const& defaultValue) const
{
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	NamedPropertyBase* property = keyFound->second;
	NamedPropertyByType<Vec2>* propertyAsVec2 = dynamic_cast<NamedPropertyByType<Vec2>*>(property);
	if (propertyAsVec2 == nullptr)
	{
		NamedPropertyByType<std::string>* propertyAsString = dynamic_cast<NamedPropertyByType<std::string>*>(property);
		if (propertyAsString == nullptr)
		{
			ERROR_RECOVERABLE("Type of property did not match type asked for!");
			return defaultValue;
		}

		Vec2 returnVec2 = defaultValue;
		returnVec2.SetFromText(propertyAsString->m_value.c_str());
		return returnVec2;
	}

	return propertyAsVec2->m_value;
}


Vec3 NamedProperties::GetValue(std::string const& keyName, Vec3 const& defaultValue) const
{
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	NamedPropertyBase* property = keyFound->second;
	NamedPropertyByType<Vec3>* propertyAsVec3 = dynamic_cast<NamedPropertyByType<Vec3>*>(property);
	if (propertyAsVec3 == nullptr)
	{
		NamedPropertyByType<std::string>* propertyAsString = dynamic_cast<NamedPropertyByType<std::string>*>(property);
		if (propertyAsString == nullptr)
		{
			ERROR_RECOVERABLE("Type of property did not match type asked for!");
			return defaultValue;
		}

		Vec3 returnVec3 = defaultValue;
		returnVec3.SetFromText(propertyAsString->m_value.c_str());
		return returnVec3;
	}

	return propertyAsVec3->m_value;
}


IntVec2 NamedProperties::GetValue(std::string const& keyName, IntVec2 const& defaultValue) const
{
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	NamedPropertyBase* property = keyFound->second;
	NamedPropertyByType<IntVec2>* propertyAsIntVec2 = dynamic_cast<NamedPropertyByType<IntVec2>*>(property);
	if (propertyAsIntVec2 == nullptr)
	{
		NamedPropertyByType<std::string>* propertyAsString = dynamic_cast<NamedPropertyByType<std::string>*>(property);
		if (propertyAsString == nullptr)
		{
			ERROR_RECOVERABLE("Type of property did not match type asked for!");
			return defaultValue;
		}

		IntVec2 returnIntVec2 = defaultValue;
		returnIntVec2.SetFromText(propertyAsString->m_value.c_str());
		return returnIntVec2;
	}

	return propertyAsIntVec2->m_value;
}


EulerAngles NamedProperties::GetValue(std::string const& keyName, EulerAngles const& defaultValue) const
{
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound == m_keyValuePairs.end())
	{
		return defaultValue;
	}

	NamedPropertyBase* property = keyFound->second;
	NamedPropertyByType<EulerAngles>* propertyAsEulerAngles = dynamic_cast<NamedPropertyByType<EulerAngles>*>(property);
	if (propertyAsEulerAngles == nullptr)
	{
		NamedPropertyByType<std::string>* propertyAsString = dynamic_cast<NamedPropertyByType<std::string>*>(property);
		if (propertyAsString == nullptr)
		{
			ERROR_RECOVERABLE("Type of property did not match type asked for!");
			return defaultValue;
		}

		EulerAngles returnEulerAngles = defaultValue;
		returnEulerAngles.SetFromText(propertyAsString->m_value.c_str());
		return returnEulerAngles;
	}

	return propertyAsEulerAngles->m_value;
}
