#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <stdarg.h>


//-----------------------------------------------------------------------------------------------
constexpr int STRINGF_STACK_LOCAL_TEMP_LENGTH = 2048;


//-----------------------------------------------------------------------------------------------
const std::string Stringf( char const* format, ... )
{
	char textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}


//-----------------------------------------------------------------------------------------------
const std::string Stringf( int maxLength, char const* format, ... )
{
	char textLiteralSmall[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	char* textLiteral = textLiteralSmall;
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		textLiteral = new char[ maxLength ];

	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		delete[] textLiteral;

	return returnValue;
}


Strings SplitStringOnDelimiter(std::string const& originalString, char delimiterToSplitOn)
{
	//create Strings object to hold the new split strings
	Strings splitStrings;
	int startOfNextString = 0;

	//check each character to see if it's the delimiter
	for (int charIndex = 0; charIndex <= originalString.length(); charIndex++)
	{
		if (originalString[charIndex] == delimiterToSplitOn || originalString[charIndex] == '\0')
		{
			std::string newString = originalString.substr(startOfNextString, charIndex - startOfNextString);

			splitStrings.push_back(newString);

			startOfNextString = charIndex + 1;
		}
	}

	return splitStrings;
}


Strings SplitStringWithQuotes(std::string const& originalString, char delimiterToSplitOn)
{
	Strings splitStrings;
	int startOfNextString = 0;
	bool withinQuotes = false;

	//check each character to see if it's the delimiter
	for (int charIndex = 0; charIndex <= originalString.length(); charIndex++)
	{
		if (originalString[charIndex] == '\"')
		{
			withinQuotes = !withinQuotes;
		}
		
		if ((!withinQuotes && originalString[charIndex] == delimiterToSplitOn) || originalString[charIndex] == '\0')
		{
			std::string newString = originalString.substr(startOfNextString, charIndex - startOfNextString);

			splitStrings.push_back(newString);

			startOfNextString = charIndex + 1;
		}
	}

	return splitStrings;
}


void ReplacePartOfString(std::string& fullString, std::string const& partToReplace, std::string const& replaceWith)
{
	size_t posToStartCheckingFrom = 0;
	size_t posOfPartToReplace = 0;
	
	while (fullString.find(partToReplace, posToStartCheckingFrom) != fullString.npos)
	{
		posOfPartToReplace = fullString.find(partToReplace, posToStartCheckingFrom);
		fullString.replace(posOfPartToReplace, partToReplace.length(), replaceWith);
		posToStartCheckingFrom = posOfPartToReplace + replaceWith.length();
	}
}


void TrimString(std::string& originalString, char delimiterToTrim)
{
	if (originalString[0] == delimiterToTrim)
	{
		originalString.replace(0, 1, "");
	}
	if (originalString[originalString.length() - 1] == delimiterToTrim)
	{
		originalString.replace(originalString.length() - 1, 1, "");
	}
}
