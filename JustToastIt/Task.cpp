#include "Task.h"

Task::Task()
{
	name = "";
	estimatedDifficulty = -1;
	estimatedTime = -1;
	ID = -1;
	dueDate = std::chrono::system_clock::now();
}

Task::Task(const std::string& assignedName, const int uniqueID,
	const short diff, const int time, const std::string& due) {
	name = assignedName;
	ID = uniqueID;
	estimatedDifficulty = diff;
	estimatedTime = time;
	dueDate = convertToTimePoint(due);
}

const short Task::getDifficulty() const
{
	return estimatedDifficulty;
}

const int Task::getEstimatedTime() const
{
	return estimatedTime;
}

TIME_POINT Task::getDueDate() const
{
	return dueDate;
}

const int Task::getID() const
{
	return ID;
}

std::string Task::getName()
{
	return name;
}

bool Task::setDifficulty(short& newDiff)
{
	if (newDiff <= 0 || newDiff > MAXDIFF) {
		LOG("Task", "Difficulty input out of bounds");
		return false;
	}
	estimatedDifficulty = newDiff;
	return true;
}

void Task::setName(std::string& newName)
{
	name = newName;
}

bool Task::setEstimatedTime(int& newTime)
{
	if (newTime < 1 || newTime > MAXTIME) {
		LOG("Task", "Time input out of bounds");
		return false;
	}
	estimatedTime = newTime;
	return true;
}

void Task::setID(int& newID)
{
	// No error handling as all ID changes will be done by TaskCollection
	// The user has no control over this.
	ID = newID;
}

void Task::setDueDate(TIME_POINT& newPoint)
{
	dueDate = newPoint;
}
