#pragma once
#include "Engine/Core/EngineCommon.hpp"


template<typename T_TypeOfPointer>
void ClearVectorOfPointers(std::vector<T_TypeOfPointer*>& vector)
{
	for (int pointerIndex = 0; pointerIndex < vector.size(); pointerIndex++)
	{
		delete vector[pointerIndex];
	}

	vector.clear();
}
