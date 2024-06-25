#pragma once


class Job
{
//public member functions
public:
	virtual ~Job() {}

	virtual void Execute() = 0;
};
