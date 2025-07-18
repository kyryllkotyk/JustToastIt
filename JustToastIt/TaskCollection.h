#ifndef TASK_COLLECTION_H_
#define TASK_COLLECTION_H_

#include <vector>
#include "Task.h"

class TaskCollection
{
public:
	TaskCollection();

	void displayTasksByPriority(); // Will not be added in MVP
	void displayTasks();
	void displayTasksByDueDate(); // Will not be added in MVP
	void displayTasksByDifficulty(); // In ascending order
	void displayTasksByEstimatedTime(); // In ascending order
	
	//TODO:: Either ensure unique names OR remove ability to delete by name
	// likely second option, as the system will always know the ID of the task
	// the user marked for deletion
	
	//TODO:: Ensure correct handling for misinput
	
	//add task should set the unique id to the task
	void addTask(Task& task); // called from main

	void createAndAddTask(std::string& name, short estDiff,
		int estTime, std::string& dueDate);

	bool removeTask(const std::string& taskName);
	
	bool removeTask(const int id);

	std::vector<Task> getTasks() const;

private:
	void quickSort(std::vector<Task>& vec, int high, int low, char sortBy);
	int partition(std::vector<Task>& vec, int high, int low, char sortBy);
	void displayHelper(std::vector<Task>& vec);
	int currentIDAssignment;
	std::vector<Task> tasks;
};

#endif
