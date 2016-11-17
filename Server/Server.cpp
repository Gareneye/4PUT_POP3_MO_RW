#include "Server.h"

Server::Server()
{
	// MOTD
	std::cout << MOTD;

	// Commands
	std::map<std::string, Command> commands;
	commands.insert( { "PING", &Server::pingCmd } );

	// Waiting for client
	std::cout << "Waiting for client..." << std::endl;
	network.acceptClient();
	std::cout << "Client connected!" << std::endl;

	// Command
	std::string buffor;
	bool working = true;

	do {
		working = network.rec(buffor);
		std::cout << working << std::endl;

		if (working)
		{
			
			std::vector<std::string> tokens = Utilities::tokenizer(buffor);

			std::cout << buffor << std::endl;

			if (tokens.size() >= 1)
			{
				Command cmd = commands[tokens[0]];

				if (cmd) {
					cmd(this, tokens);
				}
			}
			
		}

	} while (working);

}


Server::~Server()
{}


void Server::pingCmd(Server* c, std::vector<std::string>)
{
	c->network.send("PONG");
}

