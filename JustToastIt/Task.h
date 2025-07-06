#ifndef TASK_H_
#define TASK_H_

#include <string>
#include <chrono>
#include "Logger.h"

// Shortcuts
#define TIME_POINT std::chrono::system_clock::time_point // Shortcut for time point
#define LOG Logger::getInstance().log // Shortcut for using the logger

// Limitations
#define MAXDIFF 10 // Maximum Difficulty
#define MINDIFF 1 // Minimum Difficulty
#define MAXTIME 600000 // Maximum Estimated Completion Time (In Minutes)
#define MINTIME 60 // Minimum Estimated Completion Time (In Minutes)
//TODO:: Maximum due date?
//TODO:: ADD A PRECISION METER FOR SYSTEM_CLOCK.NOW()!! It is more precise than the input
//TODO:: If buggy, ensure that everything was converted to minute precision. mintime and 
//		 maxtime were in hour precision previously, transition couldve caused issues
//TODO:: Ensure defaulting behavior for misinput 
//TODO:: ensure time > 23:59 cannot be entered
//TODO:: confirmed, misinput still goes through. needs to be fixed
class Task
{
public:
	Task();
	Task(std::string& assignedName, int uniqueID, 
		short diff, int time, std::string& due);

	/*
	* @brief Converts string to a time point
	* Parses string as MM DD YYYY date 
	* OR MM DD YYYY HH MM format and converts into a time point
	* @param User inputted time
	* @return The Time Point created from parsed string
	*/
	TIME_POINT stringToTimePoint(std::string& dueDate) const; //TODO:: Add guarding against extreme due dates
	
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
	* @return Time Point of the due date
	*/
	TIME_POINT getDueDate() const;

	/*
	* @brief Get the time from now to the due date in minutes
	* @return Minutes until the due date 
	*/
	int getDueDateInMinutes() const;

	/*
	* @brief Get the collection ID of the task
	* @return Private ID value
	*/
	const int getID() const;

	/*
	* @brief Get name of the task
	* @return String name value
	*/
	std::string getName() const;

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
	* @param New estimated time value in minutes
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
	void setDueDate(TIME_POINT& newPoint); //TODO:: Add guarding against extreme due dates

private:
	std::string name; // Task's name (USER INPUT)
	short estimatedDifficulty; // Estimated difficulty of the task (USER INPUT)
	int estimatedTime; // Estimated completion time (USER INPUT)
	int ID; // Private ID of the task, assigned by TaskCollection

	// Used to compare against current time to determine how much time is left
	TIME_POINT dueDate;
};
#endif