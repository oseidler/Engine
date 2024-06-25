#include "Engine/JobSystem/JobSystem.hpp"
#include "Engine/JobSystem/Job.hpp"


//global variable declaration
JobSystem* g_theJobSystem = nullptr;


//
//static functions
//
void JobSystem::ThreadMain(int threadID)
{
	UNUSED(threadID);

	while (!g_theJobSystem->m_isQuitting)
	{
		//look for available job
		Job* claimedJob = g_theJobSystem->GetNewJobToWorkOn();

		//if found, claim job and move job from unclaimed list to claimed list in thread safe way
		if (claimedJob != nullptr)
		{
			//move job to claimed list in thread safe way
			g_theJobSystem->m_claimedJobsMutex.lock();
			g_theJobSystem->m_claimedJobs.emplace_back(claimedJob);
			g_theJobSystem->m_claimedJobsMutex.unlock();

			//call execute on claimed job
			claimedJob->Execute();

			//move to out of claimed list and into completed list in thread safe way
			g_theJobSystem->m_claimedJobsMutex.lock();

			//erase-remove idiom to remove job from vector
			g_theJobSystem->m_claimedJobs.erase(std::remove_if(
				g_theJobSystem->m_claimedJobs.begin(),
				g_theJobSystem->m_claimedJobs.end(),
				[=](auto const& element)
				{
					return element == claimedJob;
				}),
				g_theJobSystem->m_claimedJobs.end()
			);

			g_theJobSystem->m_claimedJobsMutex.unlock();

			g_theJobSystem->m_completedJobsMutex.lock();
			g_theJobSystem->m_completedJobs.emplace(claimedJob);
			g_theJobSystem->m_completedJobsMutex.unlock();
		}
		else
		{
			//if not found, sleep for ten microseconds
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}
	}
}


//
//game flow functions
//
void JobSystem::Startup()
{
}


void JobSystem::Shutdown()
{
	//backup in case game doesn't destroy all workers on its own (game would want to do it on its own if it restarts without app restarting)
	if (m_workerThreads.size() > 0)
	{
		ClearJobSystem();
	}
}

//
//worker management functions
//
void JobSystem::CreateWorkers(int numWorkers)
{
	for (int threadIndex = 0; threadIndex < numWorkers; threadIndex++)
	{
		std::thread* thread = new std::thread(JobSystem::ThreadMain, threadIndex);
		m_workerThreads.emplace_back(new JobWorkerThread(thread, threadIndex));
	}
}


void JobSystem::ClearJobSystem()
{
	m_isQuitting = true;

	for (int threadIndex = 0; threadIndex < m_workerThreads.size(); threadIndex++)
	{
		JobWorkerThread*& workerThread = m_workerThreads[threadIndex];

		if (workerThread != nullptr)
		{
			workerThread->m_thread->join();
			delete workerThread;
		}
	}

	m_workerThreads.clear();

	//lock completed jobs mutex and delete all completed jobs that weren't already retrieved
	m_completedJobsMutex.lock();
	while (m_completedJobs.size() > 0)
	{
		Job* uncompletedJob = m_completedJobs.front();
		m_completedJobs.pop();
		delete uncompletedJob;
	}
	m_completedJobsMutex.unlock();

	//I don't think you have to delete unclaimed or claimed jobs list because they'll definitely get done before worker threads join
}


//
//job management functions
//
void JobSystem::PostNewJob(Job* job)
{
	m_unclaimedJobsMutex.lock();
	m_unclaimedJobs.emplace(job);
	m_unclaimedJobsMutex.unlock();
}


Job* JobSystem::GetNewJobToWorkOn()
{
	Job* newJob = nullptr;
	
	m_unclaimedJobsMutex.lock();
	if (!m_unclaimedJobs.empty())
	{
		newJob = m_unclaimedJobs.front();
		m_unclaimedJobs.pop();
	}
	m_unclaimedJobsMutex.unlock();

	return newJob;
}


Job* JobSystem::ClaimCompletedJob()
{
	Job* completedJob = nullptr;
	
	m_completedJobsMutex.lock();
	if (!m_completedJobs.empty())
	{
		completedJob = m_completedJobs.front();
		m_completedJobs.pop();
	}
	m_completedJobsMutex.unlock();

	return completedJob;
}


bool JobSystem::AreThereCompletedJobs()
{
	bool isCompletedQueueFull = true;
	
	m_completedJobsMutex.lock();
	if(m_completedJobs.empty())
	{
		isCompletedQueueFull = false;
	}
	m_completedJobsMutex.unlock();

	return isCompletedQueueFull;
}
