#include "Scheduler.h"

void Scheduler::scheduleTask(const Task& task)
{
	TIME_POINT point;
	calculateTask(task, point);
	scheduledTasks.emplace(point, task);
}

void Scheduler::calculateTask(const Task& task, TIME_POINT& point)
{
	// Calculate how long before due date to send the task
	double time = (double)task.getEstimatedTime() * pow(1.1,task.getDifficulty());

	// Subtract the found time from the due date to find 
	// the time point for when the notification must be sent
	point = task.getDueDate() -
		std::chrono::duration_cast<std::chrono::hours>(
			std::chrono::duration<double>(time));

	// If due date is too close compared to the calculated time distance
	// make the notification time now
	if (point < std::chrono::system_clock::now()) {
		point = std::chrono::system_clock::now();
	}
}
