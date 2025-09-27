#include "Task.h"

Task::Task()
{
	dueDate = std::chrono::system_clock::now();
}

Task::Task(const std::string& assignedName, short diff, 
	int time, const std::string& due, 
	const std::string& dueDateFormat, const std::chrono::time_zone* zone,
	bool debugStatus) 
{
	setName(assignedName);
	setDifficulty(diff);
	setEstimatedTime(time);
	//TODO:: 
	setDueDateString(due, dueDateFormat, zone); 
	setDebug(debugStatus);
	//TODO:: Add a guard for each setter, logging if failed
}

Task::Task(const std::string& assignedName, short diff,
	int time, const std::string& due,
	const std::string& dueDateFormat, const std::chrono::time_zone* zone,
	bool debugStatus, int ID)
{
	setID(ID);
	setName(assignedName);
	setDifficulty(diff);
	setEstimatedTime(time);
	//TODO:: 
	setDueDateString(due, dueDateFormat, zone);
	setDebug(debugStatus);
	//TODO:: Add a guard for each setter, logging if failed
}

TIME_POINT Task::stringToTimePoint(const std::string& dueDate, 
	const std::string& dueDateFormat, 
	const std::chrono::time_zone* zone) const
{

	auto result = parseDueDate(dueDate, dueDateFormat);
	if (!result.has_value()) {
		return TIME_POINT{}; //TODO:: change the return to optional time point
		//and change this part
	}

	TIME_POINT point = result.value();
	auto offset = zone->get_info(std::chrono::time_point_cast<std::chrono::seconds>(
		std::chrono::system_clock::now()
	)).offset;
	
	point -= offset;

	//Round down to minute before returning
	return point;
}

const short Task::getDifficulty() const
{
	return estimatedDifficulty.value_or(-1);
}

const int Task::getEstimatedTime() const
{
	return estimatedTime.value_or(-1);
}

TIME_POINT Task::getDueDate() const
{
	return dueDate;
}

int Task::getDueDateInMinutes() const {
	// Floors the current time to the nearest minute
	std::chrono::system_clock::time_point now = 
		std::chrono::floor<std::chrono::minutes>(std::chrono::system_clock::now());

	// Subtracts current time from the due date, and turns that into minutes
	// Then, counts the minute duration
	return(
		std::chrono::duration_cast<std::chrono::minutes>(dueDate - now).count()
	);
}

const int Task::getID() const
{
	return ID.value_or(-1);
}

std::string Task::getName() const
{
	return name.value_or("[Not Set]");
}

std::string Task::getDueDateString() const
{
	return dueDateString.value_or("[Not Set]");
}

bool Task::getDebug() const
{
	return debug;
}

bool Task::setDifficulty(short newDiff)
{
	// Check whether the new difficulty is in bounds
	if (newDiff < MIN_DIFF || newDiff > MAX_DIFF) {
		LOG("Task", "Difficulty input out of bounds", true);
		return false;
	}
	// Update the difficulty
	estimatedDifficulty = newDiff;
	std::string idPrint = ID.has_value() ? std::to_string(*ID) : "[Not Set]";
	std::string estDiff = estimatedDifficulty.has_value() 
		? std::to_string(*estimatedDifficulty) : "[Not Set]";

	LOG("Task", "Task ID: " + idPrint+
		" difficulty set to " + estDiff, false);
	return true;
}

void Task::setName(const std::string& newName)
{
	if (newName.size() > MAX_NAME) {
		LOG("Task", "Name input out of bounds", true);
		return;
	}
	std::string idPrint = ID.has_value() ? std::to_string(*ID) : "[Not Set]";
	name = newName;
	LOG("Task", "Task ID: " + idPrint + " name set to " + *name, false);
}

bool Task::setEstimatedTime(int newTime)
{
	if (newTime < MIN_TIME || newTime > MAX_TIME) {
		LOG("Task", "Time input out of bounds", true);
		return false;
	}
	estimatedTime = newTime;

	std::string idPrint = ID.has_value() ? std::to_string(*ID) : "[Not Set]";
	std::string estTimePrint = estimatedTime.has_value()
		? std::to_string(*estimatedTime) : "[Not Set]";
	
	LOG("Task", "Task ID: " + idPrint +
		" time set to " + estTimePrint, false);
	return true;
}

void Task::setID(int newID) 
{
	// No error handling as all ID changes will be done by TaskCollection
	// The user has no control over this.
	ID = newID;

	std::string idPrint = ID.has_value() ? std::to_string(*ID) : "[Not Set]";
	LOG("Task", "New Task ID Set: " + idPrint, false);
}

void Task::setDebug(bool debugStatus)
{
	debug = debugStatus;
}

void Task::setDueDate(const TIME_POINT& newPoint)
{
	TIME_POINT copy = dueDate;
	dueDate = floor<std::chrono::minutes>(newPoint);
	if (getDueDateInMinutes() > MAX_DUE_DATE) {
		dueDate = copy;
		LOG("Task", "Due date input out of bounds", true);
		return;
	}
	std::string idPrint = ID.has_value() ? std::to_string(*ID) : "[Not Set]";
	LOG("Task", "Task ID: " + idPrint + " due date set to " +
		std::to_string(getDueDateInMinutes()) + " minutes from now", false);
}

void Task::setDueDateString(const std::string& newDueDate, 
	const std::string& dueDateFormat, const std::chrono::time_zone* zone)
{
	setDueDate(stringToTimePoint(newDueDate, dueDateFormat, zone));
	dueDateString = newDueDate;
	//TODO:: add guarding against misinput.
}

std::optional<TIME_POINT> Task::parseDueDate(const std::string& dueDate,
	const std::string& dueDateFormat) const
{
	auto it = formatParsers.find(dueDateFormat);

	if (it != formatParsers.end()) {
		TIME_POINT p = it->second(dueDate);
		if (p != TIME_POINT{}) {
			return p;
		}
		return std::nullopt;
	}

	try {
		std::stringstream stream(dueDate);
		std::chrono::sys_time<std::chrono::minutes> sysTime;

		stream >> std::chrono::parse(dueDateFormat, sysTime);

		if (stream.fail()) {
			return std::nullopt; //fail
		}

		return sysTime;
	}
	catch (const std::format_error&) {
		return std::nullopt; //fail
	}
}

int64_t Task::timeSinceEpoch(int year, int month, int day, int hours, int minutes)
{
	int64_t ans = 0;
	int leapDays = 0;
	// Starting from development year to the scheduled year, calculate leap day #
	for (int i = 1970; i < year; i++) {
		if (i % 4 == 0 && (i % 100 != 0 || i % 400 == 0)) {
			leapDays++;
		}
	}
	if (year % 4 == 0 &&
		(year % 100 != 0 || year % 400 == 0) && 
		month > 2) {
		leapDays++;
	}
	
	ans = (year - 1970) * 365LL * 86400;
	
	static const int monthDays[12] = {
	31, // Jan
	28, // Feb
	31, // Mar
	30, // Apr
	31, // May
	30, // Jun
	31, // Jul
	31, // Aug
	30, // Sep
	31, // Oct
	30, // Nov
	31  // Dec
	};

	for (int m = 1; m < month; m++) {
		ans += monthDays[m - 1] * 86400;
		if (m == 2 && year % 4 == 0 &&
			(year % 100 != 0 || year % 400 == 0)) {
			ans += 86400; // leap
		}
	}

	ans += (day - 1) * 86400;
	ans += leapDays * 86400;
	
	ans += hours * 3600;
	ans += minutes * 60;
	return ans;
}

const std::unordered_map<std::string,
	std::function<TIME_POINT(const std::string&)>>
	Task::formatParsers = {
	/*
	* Extract year, month, day, hours, and minutes from string
	* using stoi. Then, calculate time from Jan 1st 1970 00:00 to the found params
	* Watch out for misinput, but let the setter worry about maximum time. 
	* Specifically, check whether its a number before calling stoi
	* If misinput is found, throw an invalid argument error and print the error
	*/

	// Default
	{ "%F %R", [](const std::string& input) -> TIME_POINT {
		// "YYYY-MM-DD HH:MM"
		//  0123456789012345
		return parseDateTime(input, 16, 0, 5, 8, 11, 14, false);
	}},
	
	{ "%d-%m-%Y %H:%M", [](const std::string& input) -> TIME_POINT {
		// "DD-MM-YYYY HH:MM"
		//  0123456789012345
		return parseDateTime(input, 16, 6, 3, 0, 11, 14, false);
	}},

	// American styles
	{ "%m/%d/%Y %I:%M %p", [](const std::string& input) -> TIME_POINT {
		// "MM/DD/YYYY HH:MM AM/PM"
		//  0123456789012345678
		return parseDateTime(input, 19, 6, 0, 3, 11, 14, true);
	}},

	{ "%m-%d-%y %I:%M %p", [](const std::string& input) -> TIME_POINT {
		// "MM-DD-YY HH:MM AM/PM"
		//  01234567890123456
		return parseDateTime(input, 17, 6, 0, 3, 9, 12, true, 2);
	}},

	// Compact
	{ "%y%m%d %H%M", [](const std::string& input) -> TIME_POINT {
		// "YYMMDD HH:MM"
		//  012345678901
		return parseDateTime(input, 12, 0, 2, 4, 7, 10, false, 2);
	}},

	//// Words
	//{ "%A, %B %d %Y %H:%M", [](const std::string& input) -> TIME_POINT {
	//	// "Monday, August 22 2025 19:45"
	//	return TIME_POINT{};
	//}}

	
};


TIME_POINT Task::parseDateTime(const std::string& input, size_t expectedSize, 
	size_t yearPos, size_t monthPos, size_t dayPos, size_t hourPos, 
	size_t minutePos, bool american, size_t yearSize) 
{
	try {
		if (input.size() != expectedSize) {
			throw(std::invalid_argument("Input doesn't match format"));
		}
	}
	catch (const std::invalid_argument&) {
		std::cerr << "Input doesn't match the set due date format\n";
		return TIME_POINT{};//TODO:: redo this error handling system
	}

	int year, month, day, hours, minutes;

	try {
		year = stoi(input.substr(yearPos, yearSize));
		if (yearSize == 2) year += 2000;
	}
	catch (const std::invalid_argument&) {
		auto today = floor<std::chrono::days>(std::chrono::system_clock::now());
		std::chrono::year_month_day ymd{ today };
		year = int(ymd.year());
	}

	try {
		month = stoi(input.substr(monthPos, 2));
	}
	catch (const std::invalid_argument&) {
		auto today = floor<std::chrono::days>(std::chrono::system_clock::now());
		std::chrono::year_month_day ymd{ today };
		month = unsigned(ymd.month());
	}

	try {
		day = stoi(input.substr(dayPos, 2));
	}
	catch (const std::invalid_argument&) {
		auto today = floor<std::chrono::days>(std::chrono::system_clock::now());
		std::chrono::year_month_day ymd{ today };
		day = unsigned(ymd.day());
	}

	try {
		hours = stoi(input.substr(hourPos, 2));
		std::string copyOfInput = input;
		std::transform(copyOfInput.begin(), copyOfInput.end(), 
			copyOfInput.begin(), [](unsigned char c) {return std::tolower(c);});

		if (american) {
			if (copyOfInput.find("pm") != std::string::npos && hours != 12) {
				hours += 12;
			}
			else if (copyOfInput.find("am") != std::string::npos && hours == 12) {
				hours -= 12;
			}
		}
		
	}
	catch (const std::invalid_argument&) {
		// Default to 23
		hours = 23;
	}

	try {
		minutes = stoi(input.substr(minutePos, 2));
	}
	catch (const std::invalid_argument&) {
		// Default to 59
		minutes = 59;
	}
	int64_t fromEpoch = timeSinceEpoch(year, month, day, hours, minutes);
	return TIME_POINT{ std::chrono::seconds{fromEpoch} };
}
