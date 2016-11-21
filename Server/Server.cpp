#include "Server.h"

Server::Server()
{
	status = UNCONNECTED;

	// MOTD
	std::cout << MOTD;

	// Login
	login = "test";
	password = "test";

	messages.push_back("Teraz nale¿y siê zalogowaæ u¿ywaj¹c poleceñ USER i PASS. W poni¿szym przyk³adzie (i wszystkich kolejnych) linie poprzedzone przez U: oznaczaj¹ to, co wpisujesz ty (u¿ytkownik), a linie poprzedzone przez S: oznaczaj¹ odpowiedzi serwera.");
	messages.push_back("Drugaaaaaaa");

	// Commands
	std::map<std::string, Command> commands;
	commands.insert({ "USER", &Server::userCmd });			// USER CMD
	commands.insert({ "PASS", &Server::passCmd });			// USER CMD
	commands.insert({ "LIST", &Server::listCmd });			// USER CMD
	//commands.insert( { "PING", &Server::pingCmd } );

	// Waiting for client
	std::cout << "[Waiting for client...]" << std::endl;
	network.acceptClient();
	Server::greetingsCmd(this);
	std::cout << "[Client connected!]" << std::endl;

	// Command
	std::string buffor;
	bool working = true;

	do {
		working = network.rec(buffor);

		if (working)
		{
			std::vector<std::string> tokens = Utilities::tokenizer(buffor);

			#ifdef DEBUG
			std::cout << buffor << std::endl;
			#endif // DEBUG

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

void Server::greetingsCmd(Server *c, std::vector<std::string>)
{
	c->network.send("+OK POP3 Server ready!\r\n");
	c->status = Server::AUTHORIZATION;
}

void Server::userCmd(Server *c, std::vector<std::string> v)
{
	if (c->status != Server::AUTHORIZATION || v.size() < 2)
	{
		return;
	}

	std::string name = v[1];	// Name of user

	if (c->login == name)
	{
		std::string msg = "+OK " + name + " selected\r\n";
		c->network.send(msg.c_str());

		#ifdef DEBUG
		std::cout << "[User selected]" << std::endl;
		#endif // DEBUG
	}
	else
	{
		c->network.send("-ERR User not selected\r\n");
	}
}

void Server::passCmd(Server *c, std::vector<std::string> v)
{

	if (c->status != Server::AUTHORIZATION || v.size() < 2)
	{
		return;
	}

	std::string name = v[1];	// password of user

	if (c->password == name)
	{
		c->status = Server::TRANSACTION;
		std::string msg = "+OK Congratulations!\r\n";
		c->network.send(msg.c_str());

		#ifdef DEBUG
		std::cout << "[User logged]" << std::endl;
		#endif // DEBUG
	}
	else
	{
		c->network.send("-ERR Wrong password\r\n");

		#ifdef DEBUG
		std::cout << "[Wrong password]" << std::endl;
		#endif // DEBUG
	}
}

void Server::listCmd(Server *c, std::vector<std::string>v)
{
	if (c->status != Server::TRANSACTION)
	{
		return;
	}

	if (v.size() < 2)
	{
		std::ostringstream temp;
		temp << "+OK There are " << c->messages.size() << " messages\r\n";
		c->network.send(temp.str().c_str());

		for (int i = 0; i < c->messages.size(); i++)
		{
			std::ostringstream msg;
			msg << i + 1 << " " << c->messages[i].size() << "\r\n";

			c->network.send(msg.str().c_str());
		}

		c->network.send("\r\n.\r\n");
	}
	else
	{
		int index;
		std::istringstream iss(v[1]);
		iss >> index;
		index--;

		if (index >= 0 && index < c->messages.size())
		{
			std::ostringstream msg;
			msg << index << " " << c->messages[index].size() << "\r\n";

			c->network.send(msg.str().c_str());
		}
		else
		{
			c->network.send("-ERR no such message");
		}
	}


}


/*
void Server::pingCmd(Server* c, std::vector<std::string>)
{
	c->network.send("PONG");
}
*/
