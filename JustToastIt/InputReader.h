#ifndef INPUT_READER_H_
#define INPUT_READER_H_

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "TaskCollection.h"

class InputReader
{
public:
	// Main method of the reader, will be called by main 
	void read(TaskCollection& collection);
	
private:
	void parse(const std::string& input, std::vector<std::string>& tokens); // Called by read
	void executeCommand(std::vector<std::string>& tokens, 
		TaskCollection& collection);
	void lowercaseAllTokens(std::vector<std::string>& tokens);
	void lowercase(std::string& token);
	std::string joinTokens(const std::vector<std::string>& tokens, int startIndex);

};

#endif