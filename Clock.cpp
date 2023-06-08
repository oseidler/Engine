#include "Engine/Core/Clock.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <algorithm>


//static system clock
static Clock s_theSystemClock;


//
//constructors and destructor
//
Clock::Clock()
{
	if (this != &s_theSystemClock)
	{
		s_theSystemClock.AddChild(this);
	}

	m_lastUpdateTimeInSeconds = (float)GetCurrentTimeSeconds();
}


Clock::Clock(Clock& parent)
{
	parent.AddChild(this);

	m_lastUpdateTimeInSeconds = (float)GetCurrentTimeSeconds();
}


Clock::~Clock()
{
	//unlink parent and child links
	if (m_parent != nullptr)
	{
		m_parent->RemoveChild(this);
	}

	for (int childIndex = 0; childIndex < m_children.size(); childIndex++)
	{
		if (m_children[childIndex] != nullptr)
		{
			m_children[childIndex]->m_parent = nullptr;
		}
	}
}


//
//public clock utilties
//
void Clock::Reset()
{
	//reset all book-keeping variables
	m_lastUpdateTimeInSeconds = 0.0f;
	m_totalSeconds = 0.0f;
	m_deltaSeconds = 0.0f;
	m_frameCount = 0;

	//get current time as the last updated time
	m_lastUpdateTimeInSeconds = (float)GetCurrentTimeSeconds();
}


void Clock::StepSingleFrame()
{
	m_stepSingleFrame = true;
}

//
//static public clock utilities
//
Clock& Clock::GetSystemClock()
{
	return s_theSystemClock;
}


void Clock::TickSystemClock()
{
	s_theSystemClock.Tick();
}


//
//protected clock utilities
//
void Clock::Tick()
{
	//Only the system clock should use this function
	
	//get current time and calculate delta seconds from previous time gotten
	float timeCurrent = (float)GetCurrentTimeSeconds();
	float deltaTimeSeconds = timeCurrent - m_lastUpdateTimeInSeconds;

	//clamp delta seconds to max
	if (deltaTimeSeconds > m_maxDeltaSeconds)
	{
		deltaTimeSeconds = m_maxDeltaSeconds;
	}

	//advance self
	Advance(deltaTimeSeconds);
}


void Clock::Advance(float deltaTimeSeconds)
{
	//calculate new delta seconds from pausing and time scale
	if (m_isPaused && !m_stepSingleFrame)
	{
		m_deltaSeconds = 0.0f;
	}
	else
	{
		m_deltaSeconds = deltaTimeSeconds * m_timeScale;
	}

	//update bookkeeping variables
	m_frameCount++;
	m_lastUpdateTimeInSeconds = m_lastUpdateTimeInSeconds + m_deltaSeconds;
	m_totalSeconds = m_totalSeconds + m_deltaSeconds;

	//advance children
	for (int childIndex = 0; childIndex < m_children.size(); childIndex++)
	{
		m_children[childIndex]->Advance(deltaTimeSeconds);
	}

	//pause if it's set to only step a single frame
	if (m_stepSingleFrame)
	{
		m_isPaused = true;
		m_stepSingleFrame = false;
	}
}


void Clock::AddChild(Clock* childClock)
{
	//add clock to list of children
	m_children.push_back(childClock);

	//set self as new parent of child
	childClock->m_parent = this;
}


void Clock::RemoveChild(Clock* childClock)
{
	if (childClock->m_parent != this)
	{
		return;
	}

	childClock->m_parent = nullptr;
	
	//erase-remove idiom
	m_children.erase(std::remove_if(
		m_children.begin(),
		m_children.end(),
		[=](auto const& element)
		{
			return element == childClock;
		}), 
		m_children.end()
	);
}
