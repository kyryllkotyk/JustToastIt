#include "Logger.h"
#include "Scheduler.h"
#include "Task.h"
#include "TaskCollection.h"
#include <iostream>

void printGuide() {
	std::cout << "Guide:\nNOTE, NOT CASE SENSITIVE\n"
		<< "Accepted entries are :"
		<< "\nTask Creation: \ntask (starts the creation process),"
		<< "\n'task taskName estTime estDiff date(as shown below)'"
		<< "\ncreates the task in place without going into the creation menu\n"
		<< "\nDate: MM DD YYYY or MM DD YYYY HH : MM"
		<< "\nIf the hours aren't filled, it will default to 11:59 PM (23:59)"
		<< "\n\nSort: Full command(shortened command), both do the same thing"
		<< "\nDisplay by Difficulty(ddiff), Display by Due Date(ddue)"
		<< "\nDisplay by Priority (dprior), Display (d),"
		<< "\nDisplay by Completion Time (dct)"
		<< "\nEntering EXIT at any time will terminate the program";
}

int whatCommand(std::string input) {
	//Tokenize string
	//Find the command its calling
	//Check if the input is ok for this command
	//Return -1 if not and log
	//Return -1 if wrong command and log differently
	//Return the corresponding int to the command and acceptInput
	//Call other functions to figure out how to execute
}

std::string lowercase(std::string input) {
	for (int i = 0; i < input.size(); i++) {
		if (input[i] >= 'A' && input[i] <= 'Z') {
			input[i] += 32;
		}
	}
	return input;
}
//TODO:: error handling. worry about misinput.
void acceptInput(TaskCollection collection) {
	std::string input;
	std::string linput = lowercase(input);
	while (linput != "exit") {
		std::cin >> input;
		if (linput == "guide") {
			printGuide();
		}
		else {
			parse(input);
		}
	}
	exit(0);
}

int main() {
	TaskCollection collection;
	std::cout <<
		"Welcome to the toast notifier!\nEnter 'GUIDE' to learn how it works!";
	acceptInput(collection);
}