#include "Utilities.h"


std::vector<std::string> Utilities::tokenizer(std::string line)
{
	std::vector<std::string> tokens;

	std::stringstream stream(line);
	std::string buff;
	while (stream >> buff)
	{
		tokens.push_back(buff);
	}

	return tokens;
}

bool Utilities::getStatus(const std::string& line)
{
	std::string status = line.substr(0, 3);

	if (status == "+OK")
	{
		return true;
	}
	else if (status == "-ERR")
	{
		return false;
	}

	return false;
}
