#ifndef TASK_H_
#define TASK_H_

#include <string>
#include <chrono>
#include <unordered_map>
#include <functional>
#include "Logger.h"

// Shortcuts
#define TIME_POINT std::chrono::system_clock::time_point // Shortcut for time point
#define LOG Logger::getInstance().log // Shortcut for using the logger

// Limitations
#define MAX_DIFF 10 // Maximum Difficulty
#define MIN_DIFF 1 // Minimum Difficulty
#define MAX_TIME 600000 // Maximum Estimated Completion Time (In Minutes)
#define MIN_TIME 60 // Minimum Estimated Completion Time (In Minutes)
#define MAX_NAME 200 // Maximum String Size
#define MAX_DUE_DATE 60000000 // Maximum Minutes From Now to the Due Date
#define DUE_DATE_STRING_SIZE 10
//TODO:: If buggy, ensure that everything was converted to minute precision. mintime and 
//		 maxtime were in hour precision previously, transition couldve caused issues
//TODO:: Ensure defaulting behavior for misinput 
//TODO:: ensure time > 23:59 cannot be entered
//TODO:: confirmed, misinput still goes through. needs to be fixed | seems to be fixed, need to check again
//TODO:: add an optional description string, where the user can add more details

//TODO:: rewrite unupdated method descriptions
class Task
{
public:
	Task();
	Task(const std::string& assignedName, short diff, 
		int time, const std::string& due, 
		const std::string& dueDateFormat, const std::chrono::time_zone* zone,
		bool debugStatus/* = false*/);

	/*
	* @brief Converts string to a time point
	* Parses string as MM DD YYYY date 
	* OR MM DD YYYY HH MM format and converts into a time point
	* @param User inputted time
	* @return The Time Point created from parsed string
	*/
	TIME_POINT stringToTimePoint(const std::string& dueDate, 
		const std::string& dueDateFormat, 
		const std::chrono::time_zone* zone) const; 
	//TODO:: Add guarding against extreme due dates ^^
	
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

	std::string getDueDateString() const;

	bool getDebug() const;

	/*
	* @brief Set the difficulty of the task
	* @param New difficulty value
	* @return True if successful, False if misinput
	*/
	bool setDifficulty(short newDiff);

	/*
	* @brief Set the name of the task
	* @param New name value
	*/
	void setName(const std::string& newName);

	/*
	* @brief Set the estimated time of the task
	* @param New estimated time value in minutes
	* @return True if successful, False if misinput
	*/
	bool setEstimatedTime(int newTime);

	/*
	* @brief Set the ID of the task
	* @param New ID of the task
	*/
	void setID(int newID);

	void setDebug(bool debugStatus);

	/*
	* @brief Set the due date of the task
	* @param New due date time point
	*/
	void setDueDate(const TIME_POINT& newPoint); //TODO:: Add guarding against extreme due dates

	void setDueDateString(const std::string& newDueDate,
		const std::string& dueDateFormat, const std::chrono::time_zone* zone);

private:
	std::optional<TIME_POINT> parseDueDate(const std::string& dueDate,
		const std::string& dueDateFormat) const;

	static int64_t timeSinceEpoch(int year, int month, int day, int hours, int minutes);

	static const std::unordered_map<std::string,
		std::function<TIME_POINT(const std::string&)>> formatParsers;

	std::optional<std::string> name; // Task's name (USER INPUT)
	std::optional<short> estimatedDifficulty; // Estimated difficulty of the task (USER INPUT)
	std::optional<int> estimatedTime; // Estimated completion time (USER INPUT)
	std::optional<int> ID; // Private ID of the task, assigned by TaskCollection
	
	// True -> Ignore the predictive aspect and only use the due date & time
	// False -> Normal mode
	bool debug = false;

	std::optional<std::string> dueDateString;
	// Used to compare against current time
	TIME_POINT dueDate;


};
#endif