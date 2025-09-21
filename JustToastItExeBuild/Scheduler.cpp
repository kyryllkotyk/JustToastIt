#include "Scheduler.h"


Scheduler::~Scheduler()
{
	if (processingThread.joinable()) {
		continueThread = false;
		processingThread.join();
	}
}

void Scheduler::work(TaskCollection& collection)
{
	processingThread = std::thread(&Scheduler::threadWorking, this, std::ref(collection));
}

void Scheduler::threadWorking(TaskCollection& collection)
{
	std::vector<int> idsToRemove;
	while (continueThread) {
		{
			std::lock_guard lock(schedulerMutex);
			for (const Task& task : collection.getTasks()) {
				if ((task.getDebug() && task.getDueDate() <= floor<std::chrono::minutes>
					(std::chrono::system_clock::now()))
					|| (!task.getDebug() && minutesRemaining(task) <= 0)) {
					// StorageSolution.archiveTask
					displayToast(task);
					idsToRemove.push_back(task.getID());
				}

			}

			for (int& id : idsToRemove) {
				collection.removeTask(id);
			}
			idsToRemove.clear();

		} // Mutex lock guard scope
		collection.updateFile();
		
		std::this_thread::sleep_for(std::chrono::minutes(1));
	}
}

void Scheduler::displayToast(const Task& task)
{
	std::string title = "Start this task now!";
	std::string message =
		"You should start " + task.getName() + " now to complete it on time.";

	std::string command = 
		"powershell -ExecutionPolicy Bypass -Command \" Import-Module BurntToast; New-BurntToastNotification -Text '"
		+ title + "', '" + message + "'\"";
	// This program has to run in administrator mode for it to work
	system(command.c_str());
}

int Scheduler::minutesRemaining(const Task& task)
{
	int dueDate = task.getDueDateInMinutes() - 
		task.getEstimatedTime() * task.getDifficulty();
	//TODO:: update formula for a better one.. this is just for testing purposes
	return dueDate;
	//TODO:: let the user set the formula / scaling
}

