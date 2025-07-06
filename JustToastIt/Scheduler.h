#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "Task.h"
#include <map>
//DO NOT TOUCH FURTHER UNTIL TASK COLLECTION IS TESTED
class Scheduler
{
public:
	void scheduleTask(const Task& task);
private:
	std::multimap<TIME_POINT, Task> scheduledTasks;
	void calculateTask(const Task& task, TIME_POINT& point);

};

#endif