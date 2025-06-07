#ifndef TASK_H_
#define TASK_H_

#include <string>
#include <chrono>
#include "Logger.h"

#define TIME_POINT std::chrono::system_clock::time_point
#define LOG Logger::getInstance().log
#define MAXDIFF 10
#define MAXTIME 10000
class Task
{
public:
	Task();
	Task(const std::string& assignedName, const int uniqueID, 
		const short diff, const int time, const std::string& due);

	/*
	* @brief Converts string to a time point
	* Parses string as MM DD YYYY date 
	* OR MM DD YYYY HH MM format and converts into a time point
	* @param User inputted time
	* @return The Time Point created from parsed string
	*/
	TIME_POINT stringToTimePoint(std::string dueDate);
	/*
	* @brief Get the difficulty of the task
	* @return Private difficulty value
	*/
	const short getDifficulty() const;
	/*
	* @brief Get the estimated time of the task
	* @return Private time value
	*/
	const int getEstimatedTime() const;
	/*
	* @brief Get the time point of the due date
	* @return private Time Point of the due date
	*/
	TIME_POINT getDueDate() const;
	/*
	* @brief Get the collection ID of the task
	* @return Private ID value
	*/
	const int getID() const;
	/*
	* @brief Get name of the task
	* @return String name value
	*/
	std::string getName();
	/*
	* @brief Set the difficulty of the task
	* @param New difficulty value
	* @return True if successful, False if misinput
	*/
	bool setDifficulty(short& newDiff);
	/*
	* @brief Set the name of the task
	* @param New name value
	*/
	void setName(std::string& newName);
	/*
	* @brief Set the estimated time of the task
	* @param New estimated time value in hours
	* @return True if successful, False if misinput
	*/
	bool setEstimatedTime(int& newTime);
	/*
	* @brief Set the ID of the task
	* @param New ID of the task
	*/
	void setID(int& newID);
	/*
	* @brief Set the due date of the task
	* @param New due date time point
	*/
	void setDueDate(TIME_POINT& newPoint);

private:
	std::string name;
	short estimatedDifficulty;
	int estimatedTime;
	int ID;
	// Used to compare against current time to determine how much time is left
	TIME_POINT dueDate;
};
#endif