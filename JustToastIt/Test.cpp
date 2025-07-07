#include "Test.h"
#include <cassert>
#include "Logger.h"
#include "Task.h"
#include "TaskCollection.h"

/****************************************************\
* Instructions for fetching the updated project:     *
*	Open Git Bash									 *		
*	Write "cd path/to/the/proj" (Replace the path)	 *
*	Write "git checkout main"						 *
*	Pull the changes "git pull origin main"			 *
*		Resolve any conflicts if present			 *
*													 *
* Create new branch for testing:					 *
*	Repeat the first two steps						 *
*	git checkout -b add-testing						 *
*													 *
* Stage and commit changes:							 *
*	git add Test.cpp								 *
*	git commit -m "Add testing code for X feature"	 *
*	Replace X with what was tested. Keep it brief	 *
*													 *
* Push to GitHub:									 *
*	git push origin add-testing						 *
*													 *
* Open a pull request:								 *
*	Go to GitHub									 *
*	Open the repository								 *
*	main must be the base branch					 *
*	add-testing will be the compare branch			 *
*	Add a description of what you tested			 *
*	Click "Create a Pull Request"					 *
\****************************************************/

/****************************************************\
* Instructions for testing the project:				 *
*													 *
* TBD												 *
*													 *
*													 *
*													 *
*													 *
*													 *
*													 *
*													 *
\****************************************************/



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
	//For some reason, this breaks both difficulty and time, misinput improperly handled
	//TODO:: FIX
	collection.createAndAddTask(name, 1, 23, dueDate); 
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
	//logger.printLogs();
	//logger.printLogsSortedByClass();
	logger.clearLogs();

	return 0;
}