#pragma once

#include <map>
#include <vector>
#include <functional>
#include <string>
#include <iostream>

#include "Network.h"
#include "Utility.h"

#define MOTD "== Welcome in POP3 Server ==\nServer is running on 110\n\n[Type \help for more info]\n"

class Server;

typedef void(*Command)(Server *, std::vector<std::string>);

class Server
{
public:
	Server();
	virtual ~Server();


	static void statCmd(Server*, std::vector<std::string>);

private:
	Network network;
};

