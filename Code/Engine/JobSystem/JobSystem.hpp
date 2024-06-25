#pragma once
#include "Engine/JobSystem/JobWorkerThread.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <mutex>
#include <queue>


struct JobSystemConfig
{
};


class JobSystem
{
//public member functions
public:
	//static functions
	static void ThreadMain(int threadID);
	
	//constructor and destructor
	JobSystem(JobSystemConfig const& config) 
		: m_config (config) {}
	~JobSystem() {}
	
	//game flow functions
	void Startup();
	void Shutdown();
	void BeginFrame() {}
	void EndFrame() {}

	//worker management functions
	void CreateWorkers(int numWorkers);
	void ClearJobSystem();

	//job management functions
	void PostNewJob(Job* job);
	Job* GetNewJobToWorkOn();
	Job* ClaimCompletedJob();
	bool AreThereCompletedJobs();
	
//private member variables
private:
	JobSystemConfig m_config;

	std::vector<JobWorkerThread*> m_workerThreads;

	std::queue<Job*>  m_unclaimedJobs;
	std::mutex		  m_unclaimedJobsMutex;
	std::vector<Job*> m_claimedJobs;
	std::mutex		  m_claimedJobsMutex;
	std::queue<Job*> m_completedJobs;
	std::mutex		  m_completedJobsMutex;

	std::atomic<bool> m_isQuitting = false;
};

extern JobSystem* g_theJobSystem;
