#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "NetworkServer.h"
#include "Utilities.h"

#define MOTD "== Welcome in POP3 Server ==\nServer is running on port 110\n\n"
#define DEBUG true

class Server;

typedef void(*Command)(Server *, std::vector<std::string>);

class Server
{
public:
	Server();
	virtual ~Server();

	//static void pingCmd(Server *, std::vector<std::string>);
	static void greetingsCmd(Server*, std::vector<std::string> = {});
	static void userCmd(Server*, std::vector<std::string>);
	static void passCmd(Server*, std::vector<std::string>);
	static void listCmd(Server*, std::vector<std::string>);

	enum Status { UNCONNECTED, AUTHORIZATION, TRANSACTION };

private:
	NetworkServer network;
	Status status;

	// User
	std::string login;
	std::string password;

	std::vector<std::string> messages;
};

