#pragma once
#include <map>
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Core/HashedCaseInsensitiveString.hpp"


class NamedPropertyBase
{
	friend class NamedProperties;

//protected member functions
protected:
	virtual ~NamedPropertyBase() = default;
};


template <typename T>
class NamedPropertyByType : public NamedPropertyBase
{
	friend class NamedProperties;

//protected member functions
protected:
	NamedPropertyByType(T value)
		: m_value(value)
	{}

//protected member variables
protected:
	T m_value;
};


class NamedProperties
{
//public member functions
public:
	void PopulateFromXmlElementAttributes(XmlElement const& element);

	//function templates
	template <typename T>
	void SetValue(std::string const& keyName, T const& value);
	template <typename T>
	T GetValue(std::string const& keyName, T const& defaultValue) const;

	//template overrides
	void SetValue(std::string const& keyName, char const* value);
	std::string	 GetValue(std::string const& keyName, char const* defaultValue) const;
 	bool		 GetValue(std::string const& keyName, bool defaultValue) const;
 	int			 GetValue(std::string const& keyName, int defaultValue) const;
 	unsigned int GetValue(std::string const& keyName, unsigned int defaultValue) const;
 	float		 GetValue(std::string const& keyName, float defaultValue) const;
 	Rgba8		 GetValue(std::string const& keyName, Rgba8 const& defaultValue) const;
 	Vec2		 GetValue(std::string const& keyName, Vec2 const& defaultValue) const;
 	Vec3		 GetValue(std::string const& keyName, Vec3 const& defaultValue) const;
 	IntVec2		 GetValue(std::string const& keyName, IntVec2 const& defaultValue) const;
 	EulerAngles  GetValue(std::string const& keyName, EulerAngles const& defaultValue) const;

//public member variables
public:
	std::map<HCIString, NamedPropertyBase*> m_keyValuePairs;
};


template <typename T>
void NamedProperties::SetValue(std::string const& keyName, T const& value)
{
	//pretty sure this causes memory leaks, but this is how Squirrel said to do it
	NamedPropertyByType<T>* propertyByType = new NamedPropertyByType<T>(value);
	m_keyValuePairs[keyName] = propertyByType;
}


template <typename T>
T NamedProperties::GetValue(std::string const& keyName, T const& defaultValue) const
{
	auto keyFound = m_keyValuePairs.find(keyName);
	if (keyFound == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	
	NamedPropertyBase* property = keyFound->second;
	NamedPropertyByType<T>* propertyByType = dynamic_cast<NamedPropertyByType<T>*>(property);
	if (propertyByType == nullptr)
	{
		NamedPropertyByType<std::string>* propertyAsString = dynamic_cast<NamedPropertyByType<std::string>*>(property);
		if (propertyAsString == nullptr)
		{
			ERROR_RECOVERABLE("Type of property did not match type asked for!");
			return defaultValue;
		}

		ERROR_RECOVERABLE("Getting string value as this value type not supported.");
		return defaultValue;
	}

	return propertyByType->m_value;
}
