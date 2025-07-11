#include "TaskCollection.h"

TaskCollection::TaskCollection()
{
	tasks = std::vector<Task>(0);
	currentIDAssignment = 0;
}

void TaskCollection::displayTasks()
{
	//TODO:: Add mutex
	displayHelper(tasks);
}

void TaskCollection::displayTasksByDifficulty()
{
	std::vector<Task> tasksCopy = tasks;
	quickSort(tasksCopy, tasksCopy.size() - 1, 0, 'd');
	displayHelper(tasksCopy);
}

void TaskCollection::displayTasksByEstimatedTime()
{
	std::vector<Task> tasksCopy = tasks;
	quickSort(tasksCopy, tasksCopy.size() - 1, 0, 't');
	displayHelper(tasksCopy);
}


void TaskCollection::addTask(Task& task)
{
	task.setID(currentIDAssignment);
	
	tasks.push_back(task);
	LOG("TaskCollection", "Added Task with ID " + 
		std::to_string(currentIDAssignment), false);
	currentIDAssignment++;
}

void TaskCollection::createAndAddTask(std::string& name, short estDiff,
	int estTime, std::string& dueDate)
{
	// Create task from given parameters
	Task task(name, estDiff, estTime, dueDate);
	// Add this task to the collection
	(*this).addTask(task);
}

bool TaskCollection::removeTask(const std::string& taskName)
{
	for (int i = 0; i < tasks.size(); i++) {
		// Checks whether the element's name matches the name to remove
		if (tasks[i].getName() == taskName) {
			// Erases the task at the found position
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
		// Checks whether the element's ID matches the ID to remove
		if (tasks[i].getID() == id) {
			// Erases the task at the found position
			tasks.erase(tasks.begin() + i);
			LOG("TaskCollection", "RemoveTaskByID: Removed " + std::to_string(id), false);
			return true;
		}
	}
	LOG("TaskCollection", "RemoveTaskByID: ID not found in collection", true);
	return false;
}


int TaskCollection::partition(std::vector<Task>& vec, int high, 
	int low, char sortBy) {
	
	int pivot = -1;
	switch (sortBy) {
	case 't':
		pivot = vec[high].getEstimatedTime();
		break;
	case 'd':
		pivot = vec[high].getDifficulty();
		break;
	}

	int lowerIndex = low - 1;

	// Move all smaller elements on the left, after every iteration,
	// the values between low and lowerIndex are smaller
	for (int i = low; i <= high - 1; i++) {
		switch (sortBy) {
		case 't':
			if (vec[i].getEstimatedTime() < pivot) {
				lowerIndex++;
				std::swap(vec[i], vec[lowerIndex]);
			}
			break;
		case 'd':
			if (vec[i].getDifficulty() < pivot) {
				lowerIndex++;
				std::swap(vec[i], vec[lowerIndex]);
			}
			break;
		}
		
	}

	std::swap(vec[lowerIndex + 1], vec[high]);
	return lowerIndex + 1;
}

void TaskCollection::displayHelper(std::vector<Task>& vec)
{
	for (int i = 0; i < vec.size(); i++) {
		std::cout << "[Name] " << vec[i].getName() << " "
			<< "[Estimated Time (min)] " << vec[i].getEstimatedTime() << " "
			<< "[Difficulty] " << vec[i].getDifficulty() << " "
			<< "[Due Date (min)] " << vec[i].getDueDateInMinutes() << "\n";
	}
}

void TaskCollection::quickSort(std::vector<Task>& vec, int high, int low, char sortBy)
{
	if (low < high) {
		int partitionIndex = partition(vec, high, low, sortBy);
		quickSort(vec, partitionIndex - 1, low, sortBy);
		quickSort(vec, high, partitionIndex + 1, sortBy);
	}
}