#include "Server.h"



Server::Server()
{
	// Network
	Network network;
	network.createSocket();
	network.bindSocket();
	network.listenSocket();

	// MOTD
	std::cout << MOTD;

	// Commands list
	std::map<std::string, Command> commands;
	commands.insert(std::pair<std::string, Command>("user", this->userCommand));

	// Command
	std::string command;
	//getline(std::cin, command);
	//std::vector<std::string> tokens = Utility::tokenizer("1Token 2Token 3Token");

	//SOCKET client = network.acceptSocket();
}


Server::~Server()
{
}

bool Server::userCommand(unsigned int, std::string[])
{
	return false;
}
