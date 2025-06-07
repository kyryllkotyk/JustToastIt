#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <chrono>
#include <vector>
#include <mutex>

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
	void log(const std::string& className, const std::string& loggedMessage);
	bool printLogs();
	//lexicographical 
	bool printLogsSortedByClass();
private:
	std::mutex logMutex;
	typedef struct {
		std::chrono::system_clock::time_point time;
		std::string className;
		std::string message;
	} Log;
	std::vector<Log> logs;
	Logger() = default;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
};
//TODO:: alertUserMisinput(int errorNum)

#endif