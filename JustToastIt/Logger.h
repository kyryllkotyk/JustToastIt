#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <chrono>
#include <vector>
#include <mutex>
#include <iostream>

class Logger
{
public:
	/*
	* @brief Used to access the shared logger 
	* On first call, initializes the logger as a static 
	* @return Reference to the shared logger
	*/
	static Logger& getInstance();

	/*
	* @brief Logs a message 
	* Adds a logged message, current time, and name of sending class
	* to an array of logs
	*/
	void log(const std::string& className, const std::string& loggedMessage, bool error);
	
	/*
	* @brief Prints all stored logs
	*/
	void printLogs();

	/*
	* @brief Prints all stored logs grouped by class name
	* Lexicographically sorts the logs by the class they're from and prints them
	*/
	void printLogsSortedByClass();
	
	//TODO:: printLogsByError

	void clearLogs();

	std::string retrieveLastLog();

private:
	typedef struct {
		std::chrono::system_clock::time_point time; //TODO:: delete?
		std::string className;
		std::string message;
		bool error;
	} Log; // Logging structure that keeps all needed information
	//TODO:: create a log file that saves all logs (useful in case of crash!!!)

	/*
	* @brief Support method for quicksorting logs
	* Used for quicksorting by class name
	* @param Bounds of the array to check
	*/
	int partition(std::vector<Log>& vec, int high, int low);

	/*
	* @brief Support method for printing logs by class via Lomuto partition
	* Sorts logs by class name lexicographically
	* @param Bounds of the array 
	*/
	void quickSort(std::vector<Log>& vec, int high, int low);

	Logger() = default;
	Logger(const Logger&) = delete; // Disables use of copy constructor
	Logger& operator=(const Logger&) = delete; // Disables use of copy assignment operator
	
	std::mutex logMutex; // For locking data
	std::vector<Log> logs; // Storage of all the logs in the order of arrival
};
//TODO:: alertUserMisinput(int errorNum) <-- sets off alarms and prints them 
// (see last proj in 430 for similar), could use enum instead of int

#endif