#pragma once

#include <map>
#include <vector>
#include <functional>

#include "Network.h"
#include "Utility.h"

#define MOTD "== Welcome in POP3 Server ==\nServer is running on 110\n\n[Type \help for more info]\n"

typedef bool(*Command)(unsigned int tokenCount, std::string tokens[]);

class Server
{
public:
	Server();
	virtual ~Server();

protected:
	bool userCommand(unsigned int, std::string[]);

};

