#pragma once
#include <thread>
#include <stdlib.h>
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Core/NamedProperties.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/JobSystem/JobSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#define UNUSED(x) (void)(x);

//global blackboard
extern NamedStrings g_gameConfigBlackboard;
