#include "Client.h"



Client::Client()
{
	// Init
	status = AUTHORIZATION;
	isWorking = true;

	// MOTD
	std::cout << MOTD;

	// Commands
	std::map<std::string, Command> commands;
	commands.insert({ "/connect", &Client::connectCmd });
	commands.insert({ "/help", &Client::helpCmd });

	// FROM RFC
	commands.insert({ "/user", &Client::userCmd });
	commands.insert({ "/pass", &Client::passCmd });
	commands.insert({ "/list", &Client::listCmd });
	commands.insert({ "/stat", &Client::statCmd });
	commands.insert({ "/retr", &Client::retrCmd });
	commands.insert({ "/dele", &Client::deleCmd });
	commands.insert({ "/rset", &Client::rsetCmd });
	commands.insert({ "/quit", &Client::quitCmd });
	commands.insert({ "/noop", &Client::noopCmd });

	// Command
	std::string command;
	do {
		getline(std::cin, command);

		std::vector<std::string> tokens = Utilities::tokenizer(command);

		if (tokens.size() >= 1)
		{
			Command cmd = commands[tokens[0]];

			if (cmd) {
				cmd(this, tokens);
			}
		}

	} while (isWorking);
}


void Client::disable()
{
	isWorking = false;
}

void Client::userCmd(Client *c, std::vector<std::string> v)
{
	if (c->status != Client::AUTHORIZATION || v.size() < 2)
	{
		return;
	}

	std::string msg = "USER " + v[1];
	c->network.send(msg.c_str());

	// Rec
	std::string buffor;
	bool response = c->network.rec(buffor);

	if (response)
	{
		#ifdef DEBUG
		std::cout << buffor << std::endl;
		#endif // DEBUG

		if (Utilities::getStatus(buffor))
		{
			c->status = Client::AUTHORIZATION;
			std::cout << "[User selected. Type \\pass password]" << std::endl;
		}
		else
		{
			std::cout << "[User not found.]" << std::endl;
		}
	}
	else
	{
		std::cout << "[Unable connect with server...]" << std::endl;
	}
}

void Client::helpCmd(Client *c, std::vector<std::string>)
{
	switch (c->status)
	{
		case Client::AUTHORIZATION:
		{
			std::cout << "You are now in AUTHORIZATION state.";
			std::cout <<
				"\n \t [cmd] \t\t [desc] \n" <<
				"\t /connect \t for create connection with the server \n" <<
				"\n";
			break;
		}

		case Client::TRANSACTION:
		{
			std::cout << "You are now in TRANSACTION state.";
			std::cout <<
				"\n \t [cmd] \t\t [desc] \n" <<
				"\t /connect \t for create connection with the server \n" <<
				"\n";
			break;
		}
	}
}

void Client::connectCmd(Client *c, std::vector<std::string>)
{
	// Network
	bool result = c->network.connectToServer(SERVER_IP, POP3_PORT);

	std::string buffor;
	bool response = c->network.rec(buffor);

	if (response)
	{
		#ifdef DEBUG
		std::cout << buffor << std::endl;
		#endif // DEBUG
		
		if (Utilities::getStatus(buffor))
		{
			std::cout << "[Connected with server successful!]" << std::endl;
		}
		else
		{
			std::cout << "[Unable connect with server...]" << std::endl;
		}
	}
	else
	{
		std::cout << "[Unable connect with server...]" << std::endl;
	}
}

void Client::passCmd(Client *c, std::vector<std::string>v)
{
	if (c->status != Client::AUTHORIZATION || v.size() < 2)
	{
		return;
	}

	std::string msg = "PASS " + v[1];
	c->network.send(msg.c_str());

	// Rec
	std::string buffor;
	bool response = c->network.rec(buffor);

	if (response)
	{
		#ifdef DEBUG
		std::cout << buffor << std::endl;
		#endif // DEBUG

		if (Utilities::getStatus(buffor))
		{
			c->status = Client::TRANSACTION;
			std::cout << "[You are logged now]" << std::endl;
		}
		else
		{
			std::cout << "[Wrong password, try again.]" << std::endl;
		}
	}
	else
	{
		std::cout << "[Unable connect with server...]" << std::endl;
	}
}

void Client::listCmd(Client *c, std::vector<std::string>v)
{
	if (c->status != Client::TRANSACTION)
	{
		return;
	}
	
	if (v.size() > 1)
	{
		std::string msg = "LIST " + v[1];
		c->network.send(msg.c_str());

		// Rec
		std::string buffor;
		bool response = c->network.rec(buffor);

		#ifdef DEBUG
		std::cout << buffor << std::endl;
		#endif // DEBUG
	}
	else
	{
		c->network.send("LIST");

		// Rec
		std::string buffor;
		bool response = c->network.rec(buffor);

		#ifdef DEBUG
		std::cout << buffor << std::endl;
		#endif // DEBUG

		if (Utilities::getStatus(buffor))
		{

			do {
				response = c->network.rec(buffor);

				if (response)
				{
					#ifdef DEBUG
					std::cout << buffor << std::endl;
					#endif // DEBUG
				}
				else
				{
					std::cout << "[Unable connect with server...]" << std::endl;
				}

			} while (buffor.find("\r\n.") == std::string::npos);
		}
	}
}

void Client::statCmd(Client *c, std::vector<std::string>v)
{
	if (c->status != Client::TRANSACTION)
	{
		return;
	}

	c->network.send("STAT");

	// Rec
	std::string buffor;
	bool response = c->network.rec(buffor);

	if (response)
	{
		#ifdef DEBUG
		std::cout << buffor << std::endl;
		#endif // DEBUG

		if (Utilities::getStatus(buffor))
		{

		}
		else
		{

		}
	}
}

void Client::retrCmd(Client *c, std::vector<std::string>v)
{
	if (c->status != Client::TRANSACTION || v.size() < 2)
	{
		return;
	}

	int index;
	std::istringstream iss(v[1]);
	iss >> index;

	std::string command = "RETR " + std::to_string(index);
	c->network.send(command.c_str());

	// Rec
	std::string buffor;
	bool response = c->network.rec(buffor);

	if (response)
	{
		#ifdef DEBUG
		std::cout << buffor << std::endl;
		#endif // DEBUG

		if (Utilities::getStatus(buffor))
		{

			do {
				response = c->network.rec(buffor);

				if (response)
				{
					#ifdef DEBUG
					std::cout << buffor << std::endl;
					#endif // DEBUG
				}
				else
				{
					std::cout << "[Unable connect with server...]" << std::endl;
				}

			} while (buffor.find("\r\n.") == std::string::npos);
		}
	}

}

void Client::deleCmd(Client *c, std::vector<std::string>v)
{
	if (c->status != Client::TRANSACTION || v.size() < 2)
	{
		return;
	}

	int index;
	std::istringstream iss(v[1]);
	iss >> index;

	std::string command = "DELE " + std::to_string(index);
	c->network.send(command.c_str());

	// Rec
	std::string buffor;
	bool response = c->network.rec(buffor);

	if (response)
	{
		#ifdef DEBUG
		std::cout << buffor << std::endl;
		#endif // DEBUG

		if (Utilities::getStatus(buffor))
		{

		}
		else
		{

		}
	}

}

void Client::rsetCmd(Client *c, std::vector<std::string>v)
{
	if (c->status != Client::TRANSACTION)
	{
		return;
	}


	c->network.send("RSET");

	// Rec
	std::string buffor;
	bool response = c->network.rec(buffor);

	if (response)
	{
		#ifdef DEBUG
		std::cout << buffor << std::endl;
		#endif // DEBUG

		if (Utilities::getStatus(buffor))
		{

		}
		else
		{

		}
	}

}

void Client::quitCmd(Client *c, std::vector<std::string>v)
{
	if (c->status == Client::TRANSACTION)
	{
		c->status = Client::UPDATE;
	}

	c->network.send("QUIT");

	// Rec
	std::string buffor;
	bool response = c->network.rec(buffor);

	if (response)
	{
		#ifdef DEBUG
		std::cout << buffor << std::endl;
		#endif // DEBUG
	}

	// close connection
}

void Client::noopCmd(Client *c, std::vector<std::string>v)
{
	if (c->status != Client::TRANSACTION)
	{
		return;
	}

	c->network.send("NOOP");

	// Rec
	std::string buffor;
	bool response = c->network.rec(buffor);

	if (response)
	{
		#ifdef DEBUG
		std::cout << buffor << std::endl;
		#endif // DEBUG
	}
}
