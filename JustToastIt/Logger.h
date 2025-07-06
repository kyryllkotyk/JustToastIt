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
private:
	std::mutex logMutex; // For locking data
	typedef struct {
		std::chrono::system_clock::time_point time; //TODO:: delete?
		std::string className;
		std::string message;
		bool error;
	} Log; // Logging structure that keeps all needed information

	std::vector<Log> logs; // Storage of all the logs in the order of arrival
	Logger() = default; 
	Logger(const Logger&) = delete; // Disables use of copy constructor
	Logger& operator=(const Logger&) = delete; // Disables use of copy assignment operator

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
};
//TODO:: alertUserMisinput(int errorNum) <-- sets of alarms and prints them 
// (see last proj in 430 for similar)

#endif