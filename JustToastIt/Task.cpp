#include "Task.h"

Task::Task()
{
	name = "";
	estimatedDifficulty = -1;
	estimatedTime = -1;
	ID = -1;
	dueDate = std::chrono::system_clock::now();
}

Task::Task(std::string& assignedName, int uniqueID, short diff, 
	int time, std::string& due) {
	setName(assignedName);
	setID(uniqueID);
	setDifficulty(diff);
	setEstimatedTime(time);
	TIME_POINT newDueDate = stringToTimePoint(due);
	setDueDate(newDueDate);
}

TIME_POINT Task::stringToTimePoint(std::string& dueDate) const
{
	TIME_POINT point;
	std::istringstream stream(dueDate);
	
	// Convert from UTC to local time
	std::chrono::local_time<std::chrono::minutes> localPoint;
	// Parse the due date into local time
	stream >> std::chrono::parse("%F %R", localPoint);

	// !! Uncomment to Debug !! 
	/*
	LOG("Task", "Parsed epoch minutes: " +
		std::to_string(std::chrono::duration_cast<std::chrono::minutes>
			(point.time_since_epoch()).count()), true);
	*/


	if (stream.fail()) {
		LOG("Task", "Failed string parse", true);
		return std::chrono::system_clock::now();
	}
	LOG("Task", "Successful string parse of string: " + dueDate, false);

	// !! Uncomment To Debug !!
	/*
	int timeInMinutes =
		std::chrono::duration_cast<std::chrono::minutes>(point.time_since_epoch()).count();
	LOG("Task", "(StringToTimePointDebug) :: Time in minutes from epoch: " 
		+ std::to_string(timeInMinutes), true);
	*/
	
	// Convert the timezone-less point into local timezone time
	auto zonedTime = std::chrono::zoned_time{ std::chrono::current_zone(), localPoint };
	point = zonedTime.get_sys_time();
	return point;
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

int Task::getDueDateInMinutes() const {
	// Floors the current time to the nearest minute
	std::chrono::system_clock::time_point now = 
		std::chrono::floor<std::chrono::minutes>(std::chrono::system_clock::now());
	
	// !! Uncomment To Debug !!
	/*
	auto dueEpoch = std::chrono::duration_cast<std::chrono::minutes>
		(dueDate.time_since_epoch()).count();
	auto nowEpoch = std::chrono::duration_cast<std::chrono::minutes>
		(now.time_since_epoch()).count();
	int diff = dueEpoch - nowEpoch;

	LOG("Task", "dueDate (epoch min): " + std::to_string(dueEpoch), true);
	LOG("Task", "now (epoch min): " + std::to_string(nowEpoch), true);
	LOG("Task", "diff in minutes: " + std::to_string(diff), true);
	*/

	// Subtracts current time from the due date, and turns that into minutes
	// Then, counts the minute duration
	return(
		std::chrono::duration_cast<std::chrono::minutes>(dueDate - now).count()
	);
}

const int Task::getID() const
{
	return ID;
}

std::string Task::getName() const
{
	return name;
}

bool Task::setDifficulty(short& newDiff)
{
	// Check whether the new difficulty is in bounds
	if (newDiff < MINDIFF || newDiff > MAXDIFF) {
		LOG("Task", "Difficulty input out of bounds", true);
		return false;
	}
	// Update the difficulty
	estimatedDifficulty = newDiff;
	LOG("Task", "Task ID: " + std::to_string(ID) +
		" difficulty set to " + std::to_string(estimatedDifficulty), false);
	return true;
}

void Task::setName(std::string& newName)
{
	name = newName;
}

bool Task::setEstimatedTime(int& newTime)
{
	if (newTime < MINTIME || newTime > MAXTIME) {
		LOG("Task", "Time input out of bounds", true);
		return false;
	}
	estimatedTime = newTime;
	LOG("Task", "Task ID: " + std::to_string(ID) +
		" time set to " + std::to_string(newTime), false);
	return true;
}

void Task::setID(int& newID)
{
	// No error handling as all ID changes will be done by TaskCollection
	// The user has no control over this.
	ID = newID;
	LOG("Task", "New Task ID Set: " + std::to_string(ID), false);
}

void Task::setDueDate(TIME_POINT& newPoint)
{
	dueDate = newPoint;
}
