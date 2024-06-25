#pragma once
//-----------------------------------------------------------------------------------------------
#include <string>
#include <vector>


typedef std::vector<std::string> Strings;


//-----------------------------------------------------------------------------------------------
std::string const Stringf(char const* format, ...);
std::string const Stringf( int maxLength, char const* format, ... );

Strings SplitStringOnDelimiter(std::string const& originalString, char delimiterToSplitOn);
Strings SplitStringWithQuotes(std::string const& originalString, char delimiterToSplitOn);
void ReplacePartOfString(std::string& fullString, std::string const& partToReplace, std::string const& replaceWith);
void TrimString(std::string& originalString, char delimiterToTrim);
