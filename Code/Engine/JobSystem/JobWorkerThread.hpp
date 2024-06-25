#pragma once
#include <thread>


//forward declarations
class Job;


class JobWorkerThread
{
//public member functions
public:
	//constructor and destructor
	JobWorkerThread(std::thread* thread, int threadID);
	~JobWorkerThread();

//public member variables
public:
	std::thread* m_thread = nullptr;
	int			 m_threadID = -1;

	Job*		 m_currentJob = nullptr;
};
