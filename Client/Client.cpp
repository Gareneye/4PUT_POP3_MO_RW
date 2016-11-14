#include "Client.h"



Client::Client()
{
	// Init
	isWorking = true;

	// MOTD
	std::cout << MOTD;

	// Commands
	std::map<std::string, Command> commands;
	commands.insert({ "/connect", &Client::connectCmd });
	commands.insert({ "/help", &Client::helpCmd });
	commands.insert({ "/user", &Client::userCmd });
	commands.insert({ "/quit", &Client::quitCmd });

	// Command
	std::string command;

	do {
		getline(std::cin, command);

		std::vector<std::string> tokens = Utility::tokenizer(command);

		if (tokens.size() >= 1)
		{
			Command cmd = commands[tokens[0]];

			if (cmd) {
				cmd(this, tokens);
			}
		}

	} while (isWorking);
}

Client::~Client() {}

void Client::disable()
{
	isWorking = false;
}

void Client::userCmd(Client *, std::vector<std::string>)
{

}

void Client::helpCmd(Client *, std::vector<std::string>)
{
	std::cout <<
		"\n \t [cmd] \t [desc] \n" <<
		"\t \\help \t for help \n" <<
		"\t \\user \t for login \n" <<
		"\n";
}

void Client::quitCmd(Client *c, std::vector<std::string>)
{
	c->disable();
}

void Client::connectCmd(Client *c, std::vector<std::string>)
{
	// Network
	bool result = c->network.connectToServer("localhost", POP3_PORT);

	if (result)
	{
		std::cout << "Connected with server!\n";
	}
	else
	{
		std::cout << "Unnable connect to server!\n";
	}
}
