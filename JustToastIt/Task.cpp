#include "Task.h"

Task::Task()
{
	dueDate = std::chrono::system_clock::now();
}

Task::Task(std::string& assignedName, short diff, 
	int time, std::string& due) {
	setName(assignedName);
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
	return estimatedDifficulty.value_or(-1);
}

const int Task::getEstimatedTime() const
{
	return estimatedTime.value_or(-1);
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
	return ID.value_or(-1);
}

std::string Task::getName() const
{
	return name.value_or("[Not Set]");
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
	std::string idPrint = ID.has_value() ? std::to_string(*ID) : "[Not Set]";
	std::string estDiff = estimatedDifficulty.has_value() 
		? std::to_string(*estimatedDifficulty) : "[Not Set]";

	LOG("Task", "Task ID: " + idPrint+
		" difficulty set to " + estDiff, false);
	return true;
}

void Task::setName(std::string& newName)
{
	if (newName.size() > MAXNAME) {
		LOG("Task", "Name input out of bounds", true);
		return;
	}
	std::string idPrint = ID.has_value() ? std::to_string(*ID) : "[Not Set]";
	name = newName;
	LOG("Task", "Task ID: " + idPrint + " name set to " + *name, false);
}

bool Task::setEstimatedTime(int& newTime)
{
	if (newTime < MINTIME || newTime > MAXTIME) {
		LOG("Task", "Time input out of bounds", true);
		return false;
	}
	estimatedTime = newTime;

	std::string idPrint = ID.has_value() ? std::to_string(*ID) : "[Not Set]";
	std::string estTimePrint = estimatedTime.has_value()
		? std::to_string(*estimatedTime) : "[Not Set]";
	
	LOG("Task", "Task ID: " + idPrint +
		" time set to " + estTimePrint, false);
	return true;
}

void Task::setID(int& newID) 
{
	// No error handling as all ID changes will be done by TaskCollection
	// The user has no control over this.
	ID = newID;

	std::string idPrint = ID.has_value() ? std::to_string(*ID) : "[Not Set]";
	LOG("Task", "New Task ID Set: " + idPrint, false);
}

void Task::setDueDate(TIME_POINT& newPoint)
{
	TIME_POINT copy = dueDate;
	dueDate = floor<std::chrono::minutes>(newPoint);
	if (getDueDateInMinutes() > MAXDUEDATE) {
		dueDate = copy;
		LOG("Task", "Due date input out of bounds", true);
		return;
	}
	std::string idPrint = ID.has_value() ? std::to_string(*ID) : "[Not Set]";
	LOG("Task", "Task ID: " + idPrint + " due date set to " +
		std::to_string(getDueDateInMinutes()) + " minutes from now", false);
}
