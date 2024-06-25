#include "Engine/JobSystem/JobWorkerThread.hpp"
#include "Engine/JobSystem/Job.hpp"


//
//constructor
//
JobWorkerThread::JobWorkerThread(std::thread* thread, int threadID)
	: m_thread(thread)
	, m_threadID(threadID)
{
}


JobWorkerThread::~JobWorkerThread()
{
	delete m_thread;
}
