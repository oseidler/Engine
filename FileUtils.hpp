#pragma once
#include "Engine/Core/EngineCommon.hpp"


bool CheckForFile(std::string const& fileName);
int FileReadToBuffer(std::vector<uint8_t>& outBuffer, std::string const& fileName);
bool FileWriteFromBuffer(std::vector<uint8_t> const& buffer, std::string const& fileName);
int FileReadToString(std::string& outString, std::string const& fileName);
