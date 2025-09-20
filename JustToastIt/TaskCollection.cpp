#include "TaskCollection.h"

TaskCollection::TaskCollection()
{
	// Restore config settings
	std::ifstream configFile(getFilePath("config.txt"));
	readConfig(configFile);

	currentIDAssignment = 0;
	// Restore tasks 
	std::ifstream taskFile(getFilePath("tasks.txt"));
	std::string taskInfo;
	while (getline(taskFile, taskInfo)) {
		Task task = parseTaskLine(taskInfo);
		tasks.emplace_back(task);
		if (task.getID() > currentIDAssignment) {
			currentIDAssignment = task.getID() + 1;
		}
	}
	taskFile.close();
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

void TaskCollection::displayArchive()
{
	std::ifstream taskFile(getFilePath("archived.txt")); 
	std::string taskInfo;
	while (getline(taskFile, taskInfo)) {
		Task task = parseTaskLine(taskInfo);
		std::cout << "Task " << task.getID()
			<< ": " << task.getName()
			<< " [Diff: " << task.getDifficulty()
			<< ", Time: " << task.getEstimatedTime()
			<< ", Due: " << task.getDueDateString()
			<< "]\n";
	}
}


void TaskCollection::addTask(Task& task)
{
	if (task.getID() == -1) {
		task.setID(currentIDAssignment);
		currentIDAssignment++;
	}
	
	tasks.push_back(task);
	LOG("TaskCollection", "Added Task with ID " + 
		std::to_string(currentIDAssignment), false);
}

int TaskCollection::createAndAddTask(std::string& name, short estDiff,
	int estTime, std::string& dueDate, bool debugStatus)
{
	// Create task from given parameters
	Task task(name, estDiff, estTime, dueDate, dueDateFormat, timeZone, 
		debugStatus, currentIDAssignment);
	currentIDAssignment++;
	// Add this task to the collection
	(*this).addTask(task);
	return task.getID();
}

bool TaskCollection::removeTask(const int id)
{
	std::lock_guard<std::mutex> lock(saveMutex);

	auto removed = std::erase_if(tasks, [id](const Task& task) {
		return task.getID() == id;
		});

	if (removed > 0) {
		LOG("TaskCollection", "RemoveTaskByID: Removed " + std::to_string(id), false);
		return true;
	}

	LOG("TaskCollection", "RemoveTaskByID: ID not found in collection", true);
	return false;
}

std::vector<Task> TaskCollection::getTasks()
{
	std::lock_guard<std::mutex> lock(saveMutex);
	return tasks;
}

std::string TaskCollection::getDueDateFormat() const
{
	return dueDateFormat;
}

const std::chrono::time_zone* TaskCollection::getTimeZone() const
{
	return timeZone;
}

void TaskCollection::updateFile()
{
	char delim = '\x1F';

	std::lock_guard<std::mutex> lock(saveMutex);

	std::ofstream file(getFilePath("tasks.txt"));
	for (const Task& task : tasks) {
		file << task.getName() << delim
			<< task.getDifficulty() << delim
			<< task.getEstimatedTime() << delim
			<< task.getDueDateString() << delim
			<< task.getID() << '\n';

	}

	file.close();	
}

void TaskCollection::setDueDateFormat(const std::string& format)
{
	dueDateFormat = format;
	LOG("TaskCollection", "Saved format " + format, false);
	saveConfig();
}

void TaskCollection::setTimeZone(const std::string& tz)
{
	if (!tz.empty()) {
		try {
			timeZone = std::chrono::locate_zone(tz);
		}
		catch (const std::runtime_error&) {
			timeZone = std::chrono::current_zone(); // fallback
			LOG("TaskCollection",
				"Incorrect time zone entered, changed to current zone as fallback"
				, true);
		}
	}
	else {
		timeZone = std::chrono::current_zone();
	}
	saveConfig();
}

void TaskCollection::archiveTask(const Task& task)
{
	char delim = '\x1F';
	bool lockedHere = saveMutex.try_lock();
	std::ofstream file(getFilePath("archive.txt"), std::ios::app);
	
	file << task.getName() << delim
		<< task.getDifficulty() << delim
		<< task.getEstimatedTime() << delim
		<< task.getDueDateString() << delim
		<< task.getID() << '\n';

	if (lockedHere) {
		saveMutex.unlock();
	}
}

Task TaskCollection::parseTaskLine(const std::string& taskInfo, char delim) {
	std::string args[5];
	int currentPos = 0, count = 0;
	size_t found = taskInfo.find(delim, currentPos);

	while (found != std::string::npos && count < 4) {
		args[count] = taskInfo.substr(currentPos, found - currentPos);
		currentPos = found + 1;
		found = taskInfo.find(delim, currentPos);
		count++;
	}
	args[count] = taskInfo.substr(currentPos);

	Task task;
	task.setID(stoi(args[4]));
	task.setName(args[0]);
	task.setDifficulty(stoi(args[1]));
	task.setEstimatedTime(stoi(args[2]));
	task.setDueDateString(args[3], dueDateFormat, timeZone);
	return task;
}

void TaskCollection::readConfig(std::ifstream& file)
{
	const std::string tzKey = "time_zone:";
	const std::string ddfKey = "due_date_format:";
	std::string configLine = "";

	int pos = 0;
	while (getline(file, configLine)) {
		pos = configLine.find(tzKey);
		if (pos != std::string::npos) {
			std::string tz = configLine.substr(pos + tzKey.size());
			setTimeZone(tz);
			continue;
		}
		
		pos = configLine.find(ddfKey);
		if (pos != std::string::npos) {
			setDueDateFormat(configLine.substr(pos + ddfKey.size()));
			continue;
		}
	}
}

void TaskCollection::saveConfig() {
	std::ofstream file(getFilePath("config.txt"));
	file << "time_zone:" << getTimeZone()->name() << "\n";
	file << "due_date_format:" << getDueDateFormat() << "\n";
	file.close();
}

std::filesystem::path TaskCollection::getFilePath(std::string taskName) {
#ifdef _WIN32
	// Windows
	char* appdata = getenv("APPDATA");
	std::filesystem::path dir = appdata ? appdata : ".";
#else
	// Linux/Mac
	char* home = getenv("HOME");
	std::filesystem::path dir = home ? (std::string(home) + "/.config") : ".";
#endif

	dir /= "JustToastIt";
	std::filesystem::create_directories(dir);
	return dir / taskName;
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
			<< "[ID] " << vec[i].getID() << " "
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