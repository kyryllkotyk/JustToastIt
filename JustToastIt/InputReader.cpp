#include "InputReader.h"

void InputReader::read(TaskCollection& collection)
{
	std::string input;
	std::cin.clear();
	std::getline(std::cin, input);

	if (input.empty()) {
		return;
	}

	std::vector<std::string> tokens;
	parse(input, tokens);
	executeCommand(tokens, collection);
}

void InputReader::parse(const std::string& input, std::vector<std::string>& tokens)
{
	std::istringstream iss(input);
	std::string token;
	while (iss >> token) {
		tokens.emplace_back(token);
	}

	lowercase(tokens[0]);
}

void InputReader::lowercase(std::string& token)
{
	for (int i = 0; i < token.size(); i++) {
		if (token[i] >= 'A' && token[i] <= 'Z') {
			token[i] += 32;
		}
	}
}

void InputReader::executeCommand(std::vector<std::string>& tokens,
	TaskCollection& collection)
{
	if (tokens[0] == "create" || tokens[0] == "add") {
		std::string name = "";
		short diff = -1;
		int time = -1;
		std::string firstDueDate = "", secondDueDate = "";
		bool debugStatus = false;

		bool collectName = false;
		for (int i = 1; i < tokens.size(); i++) {
			bool hasDash = false, hasLetter = false, hasColon = false, hasNumber = false;

			for (int j = 0; j < tokens[i].size(); j++) {
				if (tokens[i][j] == '"' && (j == 0 || tokens[i][j - 1] != '\\')) {
					collectName = !collectName;
					continue;
				}
				if (tokens[i][j] == '-') hasDash = true;
				if (tokens[i][j] == ':') hasColon = true;
				if ((tokens[i][j] >= 'a' && tokens[i][j] <= 'z')
					|| (tokens[i][j] >= 'A' && tokens[i][j] <= 'Z')) hasLetter = true;
				if (tokens[i][j] >= '0' && tokens[i][j] <= '9') hasNumber = true;
			}

			if (collectName) {
				if (!name.empty()) name += " ";
				name += tokens[i];
				continue;
			}

			if (tokens[i].size() >= 2 && tokens[i].front() == '"' && tokens[i].back() == '"') {
				name = tokens[i].substr(1, tokens[i].size() - 2);
				name.erase(std::remove(name.begin(), name.end(), '\\'), name.end());

				continue;
			}

			if (tokens[i] == "--debug") {
				//TODO:: replace with actual flag checking
				debugStatus = true;
			}

			if (hasDash && !hasLetter && !hasColon && hasNumber) {
				firstDueDate = tokens[i];
				continue;
			}

			if (!hasDash && !hasLetter && hasColon && hasNumber) {
				secondDueDate = tokens[i];
				continue;
			}

			if (!hasDash && !hasLetter && !hasColon && hasNumber && diff == -1) {
				diff = stoi(tokens[i]);
				continue;
			}

			if (!hasDash && !hasLetter && !hasColon && hasNumber && time == -1) {
				time = stoi(tokens[i]);
				continue;
			}
		}

		if (collectName || name == "") {
			std::cout << "Enter task's name: ";
			name.erase(std::remove(name.begin(), name.end(), '\\'), name.end());
			std::getline(std::cin, name);
		}
		if (diff == -1) {
			std::cout << "Enter difficulty (1 - 10): ";
			std::cin >> diff;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		if (time == -1) {
			std::cout << "Enter estimated time (in minutes): ";
			std::cin >> time;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		if (firstDueDate == "") {
			std::cout << "Please enter due date (YYYY-MM-DD): ";
			std::cin >> firstDueDate;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		if (secondDueDate == "") {
			secondDueDate = "23:59";
		}

		std::string dueDate = firstDueDate + " " + secondDueDate;
		std::cout << "Created a task named " + name + " with ID " +
			std::to_string(collection.createAndAddTask(name, diff, time, dueDate, debugStatus));
		return;
	}

	if (tokens[0] == "display" || tokens[0] == "d") {
		bool hasDiff = false, hasTime = false;
		for (int i = 1; i < tokens.size(); i++) {
			if (tokens[i] == "diff" || tokens[i] == "difficulty" || tokens[i] == "d"
				|| tokens[i] == "estdiff") {
				hasDiff = true;
			}

			if (tokens[i] == "time" || tokens[i] == "esttime" || tokens[i] == "t") {
				hasTime = true;
			}
		}

		if (hasDiff) collection.displayTasksByDifficulty();
		else if (hasTime) collection.displayTasksByEstimatedTime();
		else collection.displayTasks();
		return;
	}

	if (tokens[0] == "dtbt" || tokens[0] == "dtbd" || tokens[0] == "dt") {
		if (tokens[0].size() == 2) collection.displayTasks();
		else if (tokens[0][3] == 't') collection.displayTasksByEstimatedTime();
		else collection.displayTasksByDifficulty();
		return;
	}

	if (tokens[0] == "help") {
		std::cout << "Help with commands (1) or formats (2)?\n";
		std::string help;
		std::cin >> help;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		lowercase(help);
		while (true) {
			if (help == "commands" || help == "1") {
				std::cout << "Possible commands:\n"
					<< "To create tasks\n"
					<< "\tcreate\n\t"
					<< "create \"Task Name\" estDiff estTime YYYY-MM-DD HH:MM\n\t"
					<< "Note:\n\t\tNot case sensitive.\n\t\tDue time is optional. "
					<< "If it is not entered, it will default to 23:59 (11:59 PM)"
					<< "\n\t\If you are missing any required details, a prompt will appear"
					<< " for you to fill it in.\nTyping \"cancel\" at "
					<< "any point will cancel the command"
					<< "\n\nTo delete task:\n\tdelete id\n\tdelete task id"
					<< "\n\nMisc:\n\texit/close - terminates the program.\n";
				break;
			}
			else if (help == "formats" || help == "2") {
				std::cout << "Formats and rules:\n\tDue Date: YYYY-MM-DD"
					<< "\n\tDue Time: HH:MM\n"
					<< "\n\tTask's name must start and end with quotation marks"
					<< "\n\t\tQuotation marks inside of the name are permitted,"
					<< "\n\t\tas long as an escape character (\\) is before them."
					<< "\n\t\tThe escape character will be removed from the name\n"
					<< "\n\tDifficulty must be an integer between 1 - 10"
					<< "\n\tEstimated time must be an integer greater than 60";
					
				break;
			}
			else {
				std::cout << "Input not recognized. Please enter one of the options: "
					<< "commands (or 1), formats (or 2): ";
				std::cin >> help;
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				lowercase(help);
			}
		}
		return;
	}

	if (tokens[0] == "exit" || tokens[0] == "close") {
		;
	}

	if (tokens[0] == "delete") {
		;
	}

	if (tokens[0] == "archive" || tokens[0] == "a" || tokens[0] == "arch") {
		collection.displayArchive();
		return;
	}

	if (tokens[0] == "set") {
		lowercaseAllTokens(tokens);
		if (tokens.size() < 2) {
			std::cout << "Incomplete command. Enter \"help\" for reference.\n";
			return;
		}

		if (tokens[1] == "format" || tokens[1] == "f") {
			if (tokens.size() < 3) {
				std::cout << "Incomplete command. Enter \"help\" for reference.\n";
				return;
			}

			if (tokens[2] == "due") {
				std::string formatString = joinTokens(tokens, 3);
				if (formatString.empty()) {
					std::cout << "Missing format string for due date.\n";
					return;
				}
				collection.setDueDateFormat(formatString);
				std::cout << "Due date format set to: "
					<< collection.getDueDateFormat()
					<< " . Note: If the due date format is different from your input,"
					<< "\nit means that your format could not be parsed."
					<< " Refer to \"help\" to learn how to set this up\n";
				return;
			}

			std::cout << "Unknown format option: " << tokens[2] << "\n";
			return;
		}

		if (tokens[1] == "value" || tokens[1] == "v") {
			if (tokens.size() < 3) {
				std::cout << "Incomplete command. Please try again\n";
				return;
			}

			if (tokens[2] == "timezone" || tokens[2] == "tz") {
				std::string tz = joinTokens(tokens, 3);
				if (tz.empty()) {
					std::cout << "Missing timezone value.\n";
					return;
				}
				collection.setTimeZone(tz);
				std::cout << "Timezone set to: " << collection.getTimeZone()->name()
					<< ". Note: If this doesn't match your desired area or input,"
					<< "\nit means that your input was not recognized."
					<< " Refer to \"help\" to learn how to set this up\n";
				return;
			}

			std::cout << "Unknown values option: " << tokens[2] << "\n";
			return;
		}
	}


	std::cout << "The set option you entered does not exist."
		<< "\nPlease enter \"help\" for the manual";
}

void InputReader::lowercaseAllTokens(std::vector<std::string>& tokens)
{
	for (std::string str : tokens) {
		lowercase(str);
	}
}


std::string InputReader::joinTokens(const std::vector<std::string>& tokens, int startIndex) {
	std::string result;
	for (int i = startIndex; i < tokens.size(); ++i) {
		if (i > startIndex) {
			result += " ";
		}
		result += tokens[i];
	}
	return result;
}