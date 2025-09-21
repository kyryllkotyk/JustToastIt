#ifndef TASK_COLLECTION_H_
#define TASK_COLLECTION_H_

#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <fstream>
#include <filesystem> 
#include <mutex>
#include <chrono>
#include <string>
#include <thread>
#include <algorithm> // For removeTask
#include "Task.h"

class TaskCollection
{
public:
	TaskCollection();

	void displayTasks();
	//void displayTasksByDueDate(); // Will not be added in MVP
	void displayTasksByDifficulty(); // In ascending order
	void displayTasksByEstimatedTime(); // In ascending order
	void displayArchive();
	
	//TODO:: probably want to update displays to only display some max # at a time
	//otherwise... try displaying 100,000,000 tasks from the test
	
	//TODO:: Ensure correct handling for misinput
	
	//add task should set the unique id to the task
	void addTask(Task& task);
	int createAndAddTask(std::string& name, short estDiff,
		int estTime, std::string& dueDate, bool debugStatus = false);
	
	bool removeTask(const int id);

	std::vector<Task> getTasks();
	std::string getDueDateFormat() const;
	const std::chrono::time_zone* getTimeZone() const;

	void updateFile();

	void setDueDateFormat(const std::string& format);
	void setTimeZone(const std::string& tz);
	
	//TODO:: ADD clearArchive
	//TODO:: ADD commandHistory here or in InputReader
private:
	void quickSort(std::vector<Task>& vec, int high, int low, char sortBy);
	int partition(std::vector<Task>& vec, int high, int low, char sortBy);
	void displayHelper(std::vector<Task>& vec);
	void archiveTask(const Task& task);
	Task parseTaskLine(const std::string& taskInfo, char delim = '\x1F');
	void readConfig(std::ifstream& file);
	void saveConfig();

	std::filesystem::path getFilePath(std::string fileName);

	int currentIDAssignment;
	std::vector<Task> tasks;
	std::mutex saveMutex;

	const std::chrono::time_zone* timeZone = std::chrono::current_zone();
	std::string dueDateFormat = "%F %R";
};

#endif
