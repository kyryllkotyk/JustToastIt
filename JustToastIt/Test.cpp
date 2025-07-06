#include "Test.h"
#include <cassert>
#include "Logger.h"
#include "Task.h"
#include "TaskCollection.h"

void testLogging(Logger& logger) {
	logger.log("A", "Test A", true);
	logger.log("E", "Test E", false);
	logger.log("C", "Test C", false);
	logger.log("ABCD", "Test ABCD", true);
	//logger.printLogs();
	//std::cout << "\nPriting sorted logs:\n";
	//logger.printLogsSortedByClass();
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
	/*std::cout << incorrectTimeTask.getName() << " "
		<< incorrectTimeTask.getID() << " "
		<< incorrectTimeTask.getDifficulty() << " " 
		<< incorrectTimeTask.getEstimatedTime() << " "
		<< incorrectTimeTask.getDueDateInMinutes() << "\n";*/
}

void testTaskCollection() {
	TaskCollection collection;
	std::string name = "Taskie";
	std::string dueDate = "2025-07-06 23:59";
	collection.createAndAddTask(name, 5, 71234, dueDate);
	name = "Taskie2";
	collection.createAndAddTask(name, 1, 1283, dueDate);
	name = "Taskie3";
	collection.createAndAddTask(name, 7, 1823, dueDate);
	std::cout << "Displaying Tasks in collection:\n";
	collection.displayTasks();
	std::cout << "Displaying Tasks sorted by difficulty:\n";
	collection.displayTasksByDifficulty();
	std::cout << "Displaying Tasks sorted by estimated time:\n";
	collection.displayTasksByEstimatedTime();
	std::cout << "\n";
	
}


int main() {
	Logger& logger = Logger::getInstance();
	testLogging(logger);
	logger.clearLogs();

	testTask();
	//logger.printLogs();
	logger.clearLogs();

	testTaskCollection(); 
	logger.printLogs();
	//logger.printLogsSortedByClass();
	return 0;
}