#ifndef TASK_COLLECTION_H_
#define TASK_COLLECTION_H_

#include <vector>
#include "Task.h"

class TaskCollection
{
public:
	TaskCollection();
	void displayTasksByPriority();
	void displayTasks();
	void displayTasksByDueDate();
	void displayTasksByDifficulty();
	void displayTasksByCompletionTime();
	//add task should set the unique id to the task
	void addTask(Task& task);
	bool removeTask(const std::string& taskName);
	bool removeTask(const int id);

private:
	int currentIDAssignment;
	std::vector<Task> tasks;
};

#endif
