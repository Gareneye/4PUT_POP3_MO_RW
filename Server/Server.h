#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "NetworkServer.h"
#include "Utilities.h"

#define MOTD "== Welcome in POP3 Server ==\nServer is running on port 110\n\n"

class Server;

typedef void(*Command)(Server *, std::vector<std::string>);

class Server
{
public:
	Server();
	virtual ~Server();

	static void pingCmd(Server *, std::vector<std::string>);

private:
	NetworkServer network;
};

