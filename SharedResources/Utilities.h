#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

class Utilities
{
public:
	static std::vector<std::string> tokenizer(std::string line);
	static bool getStatus(const std::string& line);

};

