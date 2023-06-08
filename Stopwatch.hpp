#pragma once


//forward declarations
class Clock;


class Stopwatch
{
//public member functions
public:
	//constructors
	explicit Stopwatch(float duration);
	Stopwatch(Clock const* clock, float duration);

	//stopwatch utilities
	void Start();
	void Restart();
	void Stop();
	bool DecrementDurationIfElapsed();

	//accessors
	float GetElapsedTime() const;
	float GetElapsedFraction() const;
	bool IsStopped() const;
	bool HasDurationElapsed() const;

//public member functions
public:
	Clock const* m_clock = nullptr;
	float m_startTime = 0.0f;			//if m_startTime = 0.0f, stopwatch is considered stopped
	float m_duration = 0.0f;
};
