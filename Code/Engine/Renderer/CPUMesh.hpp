#pragma once
#include "Engine/Core/Vertex_PCUTBN.hpp"
#include "Engine/Core/EngineCommon.hpp"


class CPUMesh
{
//public member functions
public:


//public member variables
public:
	std::vector<Vertex_PCUTBN> m_vertexes;
	std::vector<int>		 m_indexes;
};
