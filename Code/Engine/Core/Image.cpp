#include "Engine/Core/Image.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#define STB_IMAGE_IMPLEMENTATION // Exactly one .CPP (this Image.cpp) should #define this before #including stb_image.h
#include "ThirdParty/stb/stb_image.h"


//
//constructors
//
Image::Image(char const* imageFilePath)
	: m_imageFilePath(imageFilePath)
{
	//read image data from disk
	m_imageDimensions = IntVec2(0, 0);
	int bytesPerTexel = 0;
	int numComponentsRequested = 0;

	// Load (and decompress) the image RGB(A) bytes from a file on disk into a memory buffer (array of bytes)
	stbi_set_flip_vertically_on_load(1); // We prefer uvTexCoords has origin (0,0) at BOTTOM LEFT
	unsigned char* texelData = stbi_load(imageFilePath, &m_imageDimensions.x, &m_imageDimensions.y, &bytesPerTexel, numComponentsRequested);

	// Check if the load was successful
	GUARANTEE_OR_DIE(texelData, Stringf("Failed to load image \"%s\"", imageFilePath));
	GUARANTEE_OR_DIE(bytesPerTexel == 1 || bytesPerTexel == 3 || bytesPerTexel == 4, Stringf("Image creation failed for \"%s\" - unsupported BPP=%i (must be 3 or 4)", imageFilePath, bytesPerTexel));
	GUARANTEE_OR_DIE(m_imageDimensions.x > 0 && m_imageDimensions.y > 0, Stringf("Image creation failed for \"%s\" - illegal texture dimensions (%i x %i)", imageFilePath, m_imageDimensions.x, m_imageDimensions.y));

	//copy data from texelData to rgbaTexels vector
	if (bytesPerTexel == 1)
	{
		unsigned char* tempTexelDataPointer = texelData;
		m_rgbaTexels.resize(m_imageDimensions.x * m_imageDimensions.y);
		for (int texelIndex = 0; texelIndex < m_rgbaTexels.size(); texelIndex++)
		{
			m_rgbaTexels[texelIndex].r = *tempTexelDataPointer;
			m_rgbaTexels[texelIndex].g = *tempTexelDataPointer;
			m_rgbaTexels[texelIndex].b = *tempTexelDataPointer;
			tempTexelDataPointer++;
			m_rgbaTexels[texelIndex].a = 255;
		}
	}
	else if (bytesPerTexel == 3)
	{
		unsigned char* tempTexelDataPointer = texelData;
		m_rgbaTexels.resize(m_imageDimensions.x * m_imageDimensions.y);
		for (int texelIndex = 0; texelIndex < m_rgbaTexels.size(); texelIndex++)
		{
			m_rgbaTexels[texelIndex].r = *tempTexelDataPointer;
			tempTexelDataPointer++;
			m_rgbaTexels[texelIndex].g = *tempTexelDataPointer;
			tempTexelDataPointer++;
			m_rgbaTexels[texelIndex].b = *tempTexelDataPointer;
			tempTexelDataPointer++;
			m_rgbaTexels[texelIndex].a = 255;
		}
	}
	else
	{
		m_rgbaTexels.resize(m_imageDimensions.x * m_imageDimensions.y);
		memcpy(m_rgbaTexels.data(), texelData, m_rgbaTexels.size() * sizeof(Rgba8));
	}

	stbi_image_free(texelData);
}


Image::Image(IntVec2 size, Rgba8 color)
	: m_imageDimensions(size)
{
	m_rgbaTexels.resize(m_imageDimensions.x * m_imageDimensions.y);
	for (int texelIndex = 0; texelIndex < m_imageDimensions.x * m_imageDimensions.y; texelIndex++)
	{
		m_rgbaTexels[texelIndex] = color;
	}
}
