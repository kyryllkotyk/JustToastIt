#define CATCH_CONFIG_MAIN
#include "Logger.h"
#include "Task.h"
#include "TaskCollection.h"
#include "Scheduler.h"
#include <ctime> // for random generation
#include <cstdlib> // for random generation
#include "catch_amalgamated.hpp"

const int MAX_TESTS = 1000;
// Create Task Simplified
// Saves time for testing
Task create(std::string name, short diff, int time, int minutesFromNow) {
	TIME_POINT point = std::chrono::system_clock::now() + std::chrono::minutes(minutesFromNow);
	Task task;
	task.setDifficulty(diff);
	task.setDueDate(point);
	task.setEstimatedTime(time);
	task.setName(name);
	return task;
}

std::string rsg(int min, int max) {
	
	std::string ret = "";
	int randomNumber = rand() % (max - min + 1) + min;
	for (int i = 0; i < randomNumber; i++) {
		//ASCII 48 - 122
		char add = char(rand() % 75 + 48);
		if (!std::isprint(static_cast<unsigned char>(add)) || add == '\\') {
			i--;
		}
		else {
			ret += add;
		}
	}
	return ret;
}

int rng(int min, int max) {
	return(rand() % (max - min + 1) + min);
}
//TODO:: add tests that use string rather than minutesFromNow

TEST_CASE("Task Time Input", "[Task]") {
	srand(time(0));

	SECTION("Typical Time Input") {
		Task task = create("name", 5, 1280, 1000);
		REQUIRE(task.getEstimatedTime() == 1280);

		for (int i = 0; i < MAX_TESTS; i++) {
			int typicalInput = rng(60, 599999);
			task = create("name", 5, typicalInput, 1000);
			REQUIRE(task.getEstimatedTime() == typicalInput);
		}
	}

	SECTION("Simple Negative Time Input") {
		Task task = create("name", 5, -123, 1000);
		REQUIRE(task.getEstimatedTime() == -1);

		for (int i = 0; i < MAX_TESTS; i++) {
			int simpleNegative = rng(-599999, -60);
			task = create("name", 5, simpleNegative, 1000);
			REQUIRE(task.getEstimatedTime() == -1);
		}
	}

	SECTION("Small Negative Time Input", "[Task]") {
		Task task = create("name", 5, -20, 1000);
		REQUIRE(task.getEstimatedTime() == -1);

		for (int i = 0; i < MAX_TESTS; i++) {
			int smallNegative = rng(-59, -1);
			task = create("name", 5, smallNegative, 1000);
			REQUIRE(task.getEstimatedTime() == -1);
		}
	}

	SECTION("Small Positive Time Input", "[Task]") {
		Task task = create("name", 5, 32, 1000);
		REQUIRE(task.getEstimatedTime() == -1);

		for (int i = 0; i < MAX_TESTS; i++) {
			int smallPositive = rng(0, 59);
			task = create("name", 5, smallPositive, 1000);
			REQUIRE(task.getEstimatedTime() == -1);
		}
	}

	SECTION("Large Positive Time Input", "[Task]") {
		Task task = create("name", 5, 7598895, 1000);
		REQUIRE(task.getEstimatedTime() == -1);

		for (int i = 0; i < MAX_TESTS; i++) {
			int largePositive = rng(600001, 2000000000);
			task = create("name", 5, largePositive, 1000);
			REQUIRE(task.getEstimatedTime() == -1);
		}
	}
	
	SECTION("Large Negative Time Input", "[Task]") {
		Task task = create("name", 5, -7598895, 1000);
		REQUIRE(task.getEstimatedTime() == -1);

		for (int i = 0; i < MAX_TESTS; i++) {
			int largeNegative = rng(-2000000000, -600001);
			task = create("name", 5, largeNegative, 1000);
			REQUIRE(task.getEstimatedTime() == -1);
		}
	}
}

TEST_CASE("Task Difficulty Input", "[Task]") {
	SECTION("Typical Difficulty Input") {
		Task task = create("name", 4, 60, 1000);
		REQUIRE(task.getDifficulty() == 4);

		for (int i = 0; i < MAX_TESTS / 10; i++) {
			int typicalDifficulty = rng(1, 10);
			task = create("name", typicalDifficulty, 60, 1000);
			REQUIRE(task.getDifficulty() == typicalDifficulty);
		}
	}

	SECTION("Small Positive Difficulty Input") {
		Task task = create("name", 0, 60, 1000);
		REQUIRE(task.getDifficulty() == -1);
	}

	SECTION("Large Positive Difficulty Input") {
		Task task = create("name", 124, 60, 1000);
		REQUIRE(task.getDifficulty() == -1);

		for (int i = 0; i < MAX_TESTS / 10; i++) {
			int largePositive = rng(11, 32767);
			task = create("name", largePositive, 60, 1000);
			REQUIRE(task.getDifficulty() == -1);
		}
	}

	SECTION("Small Negative Difficulty Input") {
		Task task = create("name", -5, 60, 1000);
		REQUIRE(task.getDifficulty() == -1);

		for (int i = 0; i < MAX_TESTS / 10; i++) {
			int smallNegative = rng(-10, -1);
			task = create("name", smallNegative, 60, 1000);
			REQUIRE(task.getDifficulty() == -1);
		}
	}

	SECTION("Large Negative Difficulty Input") {
		Task task = create("name", -7189, 60, 1000);
		REQUIRE(task.getDifficulty() == -1);

		for (int i = 0; i < MAX_TESTS / 10; i++) {
			int largeNegative = rng(-32767, -11);
			task = create("name", largeNegative, 60, 1000);
			REQUIRE(task.getDifficulty() == -1);
		}
	}
}

TEST_CASE("Task Name Input", "[Task]") {
	//TODO:: create max name size
}

TEST_CASE("Task Date Input", "[Task]") {
	//TODO:: create farthest due date

}

TEST_CASE("Task Collection Task Input", "[TaskCollection]") {

}

TEST_CASE("Task Collection ID Assignment", "[TaskCollection]") {

}

// NOTE: Not ideal for testing. When adding 1 sec wait, it'll wait 1 minute
// before joining the thread, if not waiting, the background thread
// will never execute, so the tasks will never get toasted. 
// This will be fine in a non testing environment.
// Possible fix is to add a testing boolean, and change the sleep timer
// in the work method based on its' value.
// 
//TEST_CASE("Scheduler toasting", "[Scheduler]") {
//	TaskCollection collection;
//	Task task = create("One", 5, 125, 0);
//	Task task2 = create("Two", 5, 124, 0);
//	collection.addTask(task);
//	collection.addTask(task2);
//
//	Scheduler schedule;
//	schedule.work(collection);
//}


//TEST_CASE("Benchmarks") {	
//	BENCHMARK("Many tasks") {
//		TaskCollection collection;
//		for (int i = 0; i < MAX_TESTS * 100; i++) {
//			Task task("name", 5, 652, "2025-12-15 18:57", collection.getDueDateFormat(), 
//				collection.getTimeZone(), true);
//			collection.addTask(task);
//		}
//	};
//}







/* !! Legacy Testing !! 
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
	Task incorrectTimeTask(name, 8, 1380, time);
	std::cout << incorrectTimeTask.getName() << " "
		<< incorrectTimeTask.getID() << " "
		<< incorrectTimeTask.getDifficulty() << " " 
		<< incorrectTimeTask.getEstimatedTime() << " "
		<< incorrectTimeTask.getDueDateInMinutes() << "\n";
}

void testTaskCollection() {
	TaskCollection collection;
	std::string name = "Taskie";
	std::string dueDate = "2025-07-06 23:59";
	collection.createAndAddTask(name, 5, 71234, dueDate);
	name = "Taskie2";
	//For some reason, this breaks both difficulty and time, misinput improperly handled
	//TODO:: FIX
	//collection.createAndAddTask(name, 1, 23, dueDate); 
	//name = "Taskie3";
	//collection.createAndAddTask(name, 7, 1823, dueDate);
	//std::cout << "Displaying Tasks in collection:\n";
	//collection.displayTasks();
	//std::cout << "Displaying Tasks sorted by difficulty:\n";
	//collection.displayTasksByDifficulty();
	//std::cout << "Displaying Tasks sorted by estimated time:\n";
	//collection.displayTasksByEstimatedTime();
	//std::cout << "\n";
	
}


int main() {
	Logger& logger = Logger::getInstance();
	//testLogging(logger);
	//logger.clearLogs();

	//testTask();
	//logger.printLogs();
	//logger.clearLogs();

	testTaskCollection(); 
	logger.printLogs();
	//logger.printLogsSortedByClass();
	logger.clearLogs();

	return 0;
}
*/