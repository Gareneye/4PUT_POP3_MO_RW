#include "Server.h"



Server::Server()
{
	// MOTD
	std::cout << MOTD;

	// Commands
	/*
	std::map<std::string, Command> commands;
	commands.insert( { "/stat", &Server::statCmd } );

	// Command
	std::string command;
	getline(std::cin, command);

	std::vector<std::string> tokens = Utility::tokenizer(command);
	Command cmd = commands[tokens[0]];

	if (cmd) {
		cmd(this, tokens);
	}
	*/

	// Waiting for client
	std::cout << "Waiting for client..." << std::endl;
	network.acceptClient();
	std::cout << "Client connected!" << std::endl;
}


Server::~Server()
{
}

/*
void Server::statCmd(Server *, std::vector<std::string>)
{
	std::cout << MOTD << std::endl;
}
*/
