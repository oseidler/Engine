#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <stdio.h>
#include <iterator>


bool CheckForFile(std::string const& fileName)
{
	FILE* file;
	errno_t error;

	error = fopen_s(&file, fileName.c_str(), "r");
	if(error != 0 || file == nullptr)
	{
		return false;
	}
	else
	{
		fclose(file);
		return true;
	}
}


int FileReadToBuffer(std::vector<uint8_t>& outBuffer, std::string const& fileName)
{
	FILE* file;
	errno_t error;
	
	//open the file
	error = fopen_s(&file, fileName.c_str(), "rb");
	if (error != 0 || file == nullptr)
	{
		ERROR_AND_DIE("Error, could not open file.");
	}

	//get number of bytes to read
	long numBytesInFile;

	fseek(file, 0, SEEK_END);
	numBytesInFile = ftell(file);
	fseek(file, 0, SEEK_SET);

	//read from the file
	outBuffer.resize(numBytesInFile);
	fread(outBuffer.data(), sizeof(uint8_t), numBytesInFile, file);

	//close the file
	fclose(file);

	return (int)outBuffer.size();
}


bool FileWriteFromBuffer(std::vector<uint8_t> const& buffer, std::string const& fileName)
{
	FILE* file;
	errno_t error;

	//open the file
	error = fopen_s(&file, fileName.c_str(), "wb");
	if (error != 0 || file == nullptr)
	{
		ERROR_AND_DIE("Error, could not open file.");
	}

	fwrite(buffer.data(), sizeof(uint8_t), buffer.size(), file);

	//close the file
	fclose(file);

	return true;
}


int FileReadToString(std::string& outString, std::string const& fileName)
{	
	//create byte vector
	std::vector<uint8_t> outBuffer;
	
	//copy file to buffer
	FileReadToBuffer(outBuffer, fileName);

	//copy contents of byte vector to string
	copy(outBuffer.begin(), outBuffer.end(), back_inserter(outString));

	return (int)outString.size();
}
