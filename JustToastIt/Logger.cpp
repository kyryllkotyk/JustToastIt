#include "Logger.h"

Logger& Logger::getInstance()
{
	static Logger instance;
	return instance;
}

void Logger::log(const std::string& className, const std::string& loggedMessage, bool error)
{
	Log log;
	log.time = std::chrono::system_clock::now();
	log.className = className;
	log.message = loggedMessage;
	log.error = error;
	std::lock_guard<std::mutex> lock(logMutex);
	logs.emplace_back(log);
}

void Logger::printLogs()
{
	std::lock_guard<std::mutex> lock(logMutex);
	for (int i = 0; i < logs.size(); i++) {
		std::cout << "[" << logs[i].className << "] " << "[" << logs[i].message << "] ";
		if (logs[i].error) {
			std::cout << "[Error]\n";
		}
		else {
			std::cout << "[Debug]\n";
		}
	}
}

void Logger::printLogsSortedByClass()
{
	std::lock_guard<std::mutex> lock(logMutex);
	std::vector<Log> logCopy = logs;
	quickSort(logCopy, logCopy.size() - 1, 0);
	for (int i = 0; i < logCopy.size(); i++) {
		std::cout << "[" << logCopy[i].className << "] " << logCopy[i].message << " ";
		if (logCopy[i].error) {
			std::cout << "[Error]\n";
		}
		else {
			std::cout << "[Debug]\n";
		}
	}
}

void Logger::clearLogs()
{
	logs.clear();
}

int Logger::partition(std::vector<Log>& vec, int high, int low) {
	std::string pivot = vec[high].className;
	int lowerIndex = low - 1;

	// Move all smaller elements on the left, after every iteration,
	// the values between low and lowerIndex are smaller
	for (int i = low; i <= high - 1; i++) {
		if (vec[i].className < pivot) {
			lowerIndex++;
			std::swap(vec[i], vec[lowerIndex]);
		}
	}

	std::swap(vec[lowerIndex + 1], vec[high]);
	return lowerIndex + 1;
}

void Logger::quickSort(std::vector<Log>& vec, int high, int low)
{
	if (low < high) {
		int partitionIndex = partition(vec, high, low);
		quickSort(vec, partitionIndex - 1, low);
		quickSort(vec, high, partitionIndex + 1);
	}
}




