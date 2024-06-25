#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Vertex_PCUTBN.hpp"
#include "Engine/Math/Mat44.hpp"


class OBJLoader
{
//public member functions
public:
	static void LoadObjFile(std::string const& fileName, Mat44 const& fixupMatrix, std::vector<Vertex_PCUTBN>& vertexes, std::vector<int>& indexes);
};
