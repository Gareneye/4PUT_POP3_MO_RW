#include "Client.h"



Client::Client()
{
	// Network
	network.createSocket();
	network.bindSocket();

	// MOTD
	std::cout << MOTD;

	// Commands
	std::map<std::string, Command> commands;
	commands.insert({ "/help", &Client::helpCmd });
	commands.insert({ "/user", &Client::userCmd });

	// Command
	std::string command;
	getline(std::cin, command);

	std::vector<std::string> tokens = Utility::tokenizer(command);
	Command cmd = commands[tokens[0]];

	if (cmd) {
		cmd(this, tokens);
	}
}

Client::~Client() {}

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
