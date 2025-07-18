#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "TaskCollection.h"
#include "Toast.h"

class Scheduler
{
public:
	Scheduler() = default;
private:
	void threadWorking(const TaskCollection& collection);
	std::mutex schedulerMutex;
	std::thread processingThread; // Background thread for pushing toasts
	Toast toaster;

	Scheduler(Scheduler& schedule) = delete;
	// could make task collection check all task's dueDateInMinutes
	// and if it reaches zero, call the scheduler
	// OR
	// the scheduler itself is responsible for checking the due dates
	// so it calls the task collection 
	// opt1 seems better, TODO:: investigate

	//in a thread, wake up once a minute, check whether the time point due date 
	//matches now() !!MINUTE PRECISION!! for every Task, if any match, add them to
	//Toast's queue

};

#endif