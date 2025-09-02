#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <atomic>
#include "TaskCollection.h"

class Scheduler
{
public:
	Scheduler() : continueThread(true) {};
	~Scheduler();
	void work(TaskCollection& collection);

private:
	void threadWorking(TaskCollection& collection);//HANDLE REMOVING FROM TASKCOLLECTION!!!!
	void displayToast(const Task& task);
	int minutesRemaining(const Task& task);

	std::mutex schedulerMutex;
	std::thread processingThread; // Background thread for pushing toasts
	std::atomic<bool> continueThread; // For clean termination

	Scheduler(Scheduler& schedule) = delete;

	//in a thread, wake up once a minute, check whether the time point due date 
	//matches now() !!MINUTE PRECISION!! for every Task, if any match, add them to
	//Toast's queue

	//TODO:: Handle starting at exactly the start of a new minute, including after PC restart.
	

};

#endif