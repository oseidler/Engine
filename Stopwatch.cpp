#include "Engine/Core/Stopwatch.hpp"
#include "Engine/Core/Clock.hpp"


//
//constructors
//
Stopwatch::Stopwatch(float duration)
	: m_duration(duration)
{
	m_clock = &Clock::GetSystemClock();
}


Stopwatch::Stopwatch(Clock const* clock, float duration)
	: m_clock(clock)
	, m_duration(duration)
{
}

//
//public stopwatch utilities
//
void Stopwatch::Start()
{
	m_startTime = m_clock->GetTotalSeconds();
}


void Stopwatch::Restart()
{
	if (!IsStopped())
	{
		m_startTime = m_clock->GetTotalSeconds();
	}
}


void Stopwatch::Stop()
{
	m_startTime = 0.0f;
}


bool Stopwatch::DecrementDurationIfElapsed()
{
	if (HasDurationElapsed())
	{
		m_startTime += m_duration;
		return true;
	}

	return false;
}


//
//public accessors
//
float Stopwatch::GetElapsedTime() const
{
	//return 0.0f if stopwatch is stopped
	if (IsStopped())
	{
		return 0.0f;
	}

	return m_clock->GetTotalSeconds() - m_startTime;
}


float Stopwatch::GetElapsedFraction() const
{
	float elapsedTime = GetElapsedTime();

	return elapsedTime / m_duration;
}


bool Stopwatch::IsStopped() const
{
	return m_startTime == 0.0f;
}


bool Stopwatch::HasDurationElapsed() const
{
	if (IsStopped())
	{
		return false;
	}

	float elapsedTime = m_clock->GetTotalSeconds() - m_startTime;

	return elapsedTime > m_duration;
}
