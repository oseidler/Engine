#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/Rgba8.hpp"


class Image
{
	friend class Renderer;

//public member functions
public:
	//constructors and destructor
	Image() {}
	Image(char const* imageFilePath);
	Image(IntVec2 size, Rgba8 color);
	~Image() {}

	//accessors
	std::string const& GetImageFilePath() const { return m_imageFilePath; }
	IntVec2		GetImageDimensions() const { return m_imageDimensions; }
	void const* GetRawData() const { return m_rgbaTexels.data(); }

//private member variables
private:
	std::string		   m_imageFilePath;
	IntVec2			   m_imageDimensions;
	std::vector<Rgba8> m_rgbaTexels;
};
