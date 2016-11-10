#pragma once

#include <map>
#include <vector>
#include <functional>
#include <string>
#include <iostream>

#include "Network.h"
#include "Utility.h"

#define MOTD "== Welcome in POP3 Client ==\nClient is running\nType /help for more info\n\n"

class Client;

typedef void(*Command)(Client *, std::vector<std::string>);

class Client
{
public:
	Client();
	virtual ~Client();

	static void userCmd(Client*, std::vector<std::string>);
	static void helpCmd(Client*, std::vector<std::string>);

private:
	Network network;
};

