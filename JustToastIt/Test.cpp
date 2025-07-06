#include "Test.h"
#include <cassert>
#include "Logger.h"
#include "Task.h"

void testLogging(Logger& logger) {
	logger.log("A", "Test A", true);
	logger.log("E", "Test E", false);
	logger.log("C", "Test C", false);
	logger.log("ABCD", "Test ABCD", true);
	logger.printLogs();
	std::cout << "\nPriting sorted logs:\n";
	logger.printLogsSortedByClass();
}

void testTask() {
	Task task;

	// Test difficulty too high
	short diff = 11;
	assert(!task.setDifficulty(diff));

	// Test difficulty too low
	diff = -1;
	assert(!task.setDifficulty(diff));

	// Test normal difficulty range
	diff = 3;
	assert(task.setDifficulty(diff));


	std::string time = "2025-07-05 21:03";
	std::string name = "Name1";
	Task incorrectTimeTask(name, 5, 8, 1380, time);
	std::cout << incorrectTimeTask.getName() << " " 
		<< incorrectTimeTask.getID() << " "
		<< incorrectTimeTask.getDifficulty() << " " 
		<< incorrectTimeTask.getEstimatedTime() << " "
		<< incorrectTimeTask.getDueDateInMinutes() << "\n";
}

int main() {
	Logger& logger = Logger::getInstance();
	testLogging(logger);
	logger.clearLogs();

	testTask();
	logger.printLogs();
	logger.clearLogs();
	return 0;
}