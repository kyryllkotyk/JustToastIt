#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "Logger.h"
#include "Scheduler.h"
#include "Task.h"
#include "TaskCollection.h"
#include "InputReader.h"


void printGuide() {
	//TODO:: Allow for a direct time entry, "TaskName Time
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
		<< "\nEntering EXIT at any time will terminate the program\n";
}

int whatCommand(std::string input) {
	//Tokenize string
	//Find the command its calling
	//Check if the input is ok for this command
	//Return -1 if not and log
	//Return -1 if wrong command and log differently
	//Return the corresponding int to the command and acceptInput
	//Call other functions to figure out how to execute
	return -1;
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
	std::string linput = "";
	while (linput != "exit") {
		std::cin >> input;
		linput = lowercase(input);
		if (linput == "guide") {
			printGuide();
		}
		else {
			//parse(input);
		}
	}
	exit(0);
}

//TODO:: Handle restarting
	// On startup, Storage class should fill in TaskCollection with stored information
	// Every time TaskCollection is modified, Storage is modified as well in the same way
//TODO:: Schedule interactions		

void handleSig(int sig) {
	exit(0); //so atexit works!
}

int main() {
	signal(SIGINT, handleSig);
	signal(SIGTERM, handleSig);

	static TaskCollection collection;
	InputReader reader;
	Scheduler scheduler;

	std::cout << "Welcome to JustToastIt! Please enter a command.\n";
	std::cout << "Enter \"help\" for manual\n";
	scheduler.work(collection);

	atexit([] {
		collection.updateFile();
	});

	while (true) {
		reader.read(collection);
		std::cout << "\nPlease enter a command (\"help\" for manual):\n";
	}
}
