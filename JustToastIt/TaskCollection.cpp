#include "TaskCollection.h"

TaskCollection::TaskCollection()
{
	tasks = std::vector<Task>(0);
	currentIDAssignment = 0;
}

void TaskCollection::addTask(Task& task)
{
	task.setID(currentIDAssignment);
	
	tasks.push_back(task);
	LOG("TaskCollection", "Added Task with ID " + currentIDAssignment, false);
	currentIDAssignment++;
}

bool TaskCollection::removeTask(const std::string& taskName)
{
	for (int i = 0; i < tasks.size(); i++) {
		if (tasks[i].getName() == taskName) {
			tasks.erase(tasks.begin() + i);
			LOG("TaskCollection", "RemoveTaskByName: Removed " + taskName, false);
			return true;
		}
	}
	LOG("TaskCollection", "RemoveTaskByName: Name not found in collection", true);
	return false;
}

bool TaskCollection::removeTask(const int id)
{
	for (int i = 0; i < tasks.size(); i++) {
		if (tasks[i].getID() == id) {
			tasks.erase(tasks.begin() + i);
			LOG("TaskCollection", "RemoveTaskByID: Removed " + std::to_string(id), false);
			return true;
		}
	}
	LOG("TaskCollection", "RemoveTaskByID: ID not found in collection", true);
	return false;
}
