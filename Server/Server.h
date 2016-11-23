#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>

#include <filesystem>
namespace fs = std::experimental::filesystem;

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
	virtual ~Server() {};

	static void greetingsCmd(Server*, std::vector<std::string> = {});
	static void userCmd(Server *, std::vector<std::string>);
	static void passCmd(Server *, std::vector<std::string>);
	static void listCmd(Server *, std::vector<std::string>);
	static void statCmd(Server *, std::vector<std::string>);
	static void retrCmd(Server *, std::vector<std::string>);
	static void deleCmd(Server *, std::vector<std::string>);
	static void rsetCmd(Server *, std::vector<std::string>);
	static void quitCmd(Server *, std::vector<std::string>);
	static void noopCmd(Server *, std::vector<std::string>);
	
	enum Status { AUTHORIZATION, TRANSACTION, UPDATE };

	struct Message {
		std::string id;

		std::string From;
		std::string To;
		std::string Date;
		std::string Subject;
		std::string Msg;

		bool deleted = false;

		int length() {
			return From.length() + To.length() + Date.length() + Subject.length() + Msg.length();
		}

		std::string content()
		{
			std::string all = "From: " + From + "\r\n"
				+ "To: " + To + "\r\n" 
				+ "Date: " + Date + "\r\n" 
				+ "Subject: " + Subject + "\r\n"
				+ Msg;

			return all;
		}
	};

private:
	NetworkServer network;
	Status status;

	// User
	std::string login;
	std::string password;

	std::vector<Message> messages;
};

