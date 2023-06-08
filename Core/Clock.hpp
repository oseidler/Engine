#pragma once
#include <vector>


class Clock
{
//public member functions
public:
	//constructors and destructor
	Clock();
	explicit Clock(Clock& parent);
	Clock(Clock const& copy) = delete;
	~Clock();

	//clock utilities
	void Reset();
	void Pause() { m_isPaused = true; }
	void Unpause() { m_isPaused = false; }
	void TogglePause() { m_isPaused = !m_isPaused; }
	void StepSingleFrame();
	void SetTimeScale(float timeScale) { m_timeScale = timeScale; }

	//accessors
	bool IsPaused() const { return m_isPaused; }
	float GetTimeScale() const { return m_timeScale; }
	float GetDeltaSeconds() const { return m_deltaSeconds; }
	float GetTotalSeconds() const { return m_totalSeconds; }
	size_t GetFrameCount() const { return m_frameCount; }

	//static functions
	static Clock& GetSystemClock();
	static void TickSystemClock();

//protected member functions
protected:
	//clock utilities
	void Tick();
	void Advance(float deltaTimeSeconds);
	void AddChild(Clock* childClock);
	void RemoveChild(Clock* childClock);

//protected member variables
protected:
	Clock* m_parent = nullptr;
	std::vector<Clock*> m_children;

	float m_lastUpdateTimeInSeconds = 0.0f;
	float m_totalSeconds = 0.0f;
	float m_deltaSeconds = 0.0f;
	size_t m_frameCount = 0;

	float m_timeScale = 1.0f;
	float m_maxDeltaSeconds = 0.1f;

	bool m_isPaused = false;
	bool m_stepSingleFrame = false;
};
