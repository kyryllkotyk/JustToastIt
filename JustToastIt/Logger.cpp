#include "Logger.h"

Logger& Logger::getInstance()
{
	static Logger instance;
	return instance;
}

void Logger::log(const std::string& className, const std::string& loggedMessage)
{
	Log log;
	log.time = std::chrono::system_clock::now();
	log.className = className;
	log.message = loggedMessage;
	std::lock_guard<std::mutex> lock(logMutex);
	logs.emplace_back(log);
}

