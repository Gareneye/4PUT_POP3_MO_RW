#include "Server.h"

Server::Server()
{
	// Delfault Authorization
	status = AUTHORIZATION;

	// MOTD
	std::cout << MOTD;

	// Login
	login = "test";
	password = "test";

	/*
		std::string From;
		std::string To;
		std::string Date;
		std::string Subject;
		std::string Message;
	*/

	std::string path = "messages";
	for (auto & p : fs::directory_iterator(path))
	{
		std::ostringstream file;
		file << p;

		std::string line;

		std::ifstream msgFile(file.str());
		if (msgFile.is_open())
		{
			Message msgObj;

			msgObj.id = file.str();

			getline(msgFile, line);
			msgObj.From = line.substr(line.find("From:"));

			getline(msgFile, line);
			msgObj.To = line.substr(line.find("To:"));

			getline(msgFile, line);
			msgObj.Date = line.substr(line.find("Date:"));

			getline(msgFile, line);
			msgObj.Subject = line.substr(line.find("Subject:"));

			std::string content;
			while (getline(msgFile, line))
			{
				content += line;
			}
			msgObj.Msg = content;

			messages.push_back(msgObj);

			msgFile.close();
		}
		else
		{
			std::cout << "Unable to open file";
		}
	}


	// Commands
	std::map<std::string, Command> commands;
	commands.insert({ "USER", &Server::userCmd });	
	commands.insert({ "PASS", &Server::passCmd });
	commands.insert({ "LIST", &Server::listCmd });
	commands.insert({ "STAT", &Server::statCmd });			
	commands.insert({ "RETR", &Server::retrCmd });
	commands.insert({ "DELE", &Server::deleCmd });			
	commands.insert({ "RSET", &Server::rsetCmd });
	commands.insert({ "QUIT", &Server::quitCmd });
	commands.insert({ "NOOP", &Server::noopCmd });

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


void Server::greetingsCmd(Server *c, std::vector<std::string>)
{
	c->network.send("+OK POP3 Server ready!");
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
		std::string msg = "+OK " + name + " selected";
		c->network.send(msg.c_str());
	}
	else
	{
		c->network.send("-ERR User not selected");
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
		std::string msg = "+OK Congratulations!";
		c->network.send(msg.c_str());

		#ifdef DEBUG
		std::cout << "[User logged]" << std::endl;
		#endif // DEBUG
	}
	else
	{
		c->network.send("-ERR Wrong password");
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

		// Count messages
		int amount = 0;
		for (auto i : c->messages)
		{
			if (!i.deleted) amount++;
		}

		// Send success
		std::string list = "+OK There are " + std::to_string(amount) + " messages";
		c->network.send(list.c_str());

		// Send all messages
		std::string msg;
		for (int i = 0; i < c->messages.size(); i++)
		{
			if (!c->messages[i].deleted)
			{
				msg = std::to_string(i + 1) + " " + std::to_string(c->messages[i].length());
				c->network.send(msg.c_str());

				msg.clear();
			}
		}

		c->network.send("\r\n.");
	}
	else
	{
		// Index parse
		int index;
		std::istringstream iss(v[1]);
		iss >> index;
		index--;

		if (index < 0 && index >= c->messages.size())
		{
			c->network.send("-ERR no such message");
		}
		else
		{
			// Is marked as deleted?
			if (!c->messages[index].deleted)
			{
				std::string msg = "+OK " + std::to_string(index + 1) + " " + std::to_string(c->messages[index].length());
				c->network.send(msg.c_str());
			}
			else
			{
				c->network.send("-ERR message is deleted");
			}
		}
	}


}

void Server::statCmd(Server *c, std::vector<std::string>v)
{
	if (c->status != Server::TRANSACTION)
	{
		return;
	}

	int amount = 0;
	int bytes = 0;

	for (auto msg : c->messages)
	{
		if (!msg.deleted)
		{
			amount++;
			bytes += msg.length();
		}		
	}

	std::string text = "+OK " + std::to_string(amount) + ' ' + std::to_string(bytes);
	c->network.send(text.c_str());
}

void Server::retrCmd(Server *c, std::vector<std::string>v)
{
	if (c->status != Server::TRANSACTION || v.size() < 2)
	{
		return;
	}

	// index parser
	int index;
	std::istringstream iss(v[1]);
	iss >> index;
	index--;

	if (index < 0 && index >= c->messages.size())
	{
		c->network.send("-ERR Message not exists");
	}
	else
	{
		if (c->messages[index].deleted)
		{
			c->network.send("-ERR Message is deleted");
		}
		else
		{
			std::string text = "+OK " + std::to_string(c->messages[index].length());
			c->network.send(text.c_str());
			c->network.send(c->messages[index].content().c_str());

			c->network.send("\r\n.");
		}
	}

}

void Server::deleCmd(Server *c, std::vector<std::string>v)
{
	if (c->status != Server::TRANSACTION || v.size() < 2)
	{
		return;
	}

	// parse index
	int index;
	std::istringstream iss(v[1]);
	iss >> index;
	index--;

	// Is message exist
	if (index < 0 && index >= c->messages.size())
	{
		c->network.send("-ERR no such message");
	}
	else
	{
		c->messages[index].deleted = true;
		c->network.send("+OK message deleted");
	}
}

void Server::rsetCmd(Server *c, std::vector<std::string>v)
{
	if (c->status != Server::TRANSACTION)
	{
		return;
	}

	int size = 0;
	int amount = 0;

	for (auto &i : c->messages)
	{
		if (i.deleted)
		{
			size += i.length();
			amount++;
			i.deleted = false;
		}
	}

	if (amount == 0)
	{
		c->network.send("+OK there's no messages marked as deleted");
	}
	else
	{
		std::string msg = "+OK maildrop has new " + std::to_string(amount) + " messages (" + std::to_string(size) + " octets)";
		c->network.send(msg.c_str());
	}

}

void Server::quitCmd(Server *c, std::vector<std::string>v)
{
	if (c->status == Server::TRANSACTION)
	{
		c->status = Server::UPDATE;
	}

	int amount = 0;

	for (auto it = c->messages.begin(); it != c->messages.end();)
	{
		if ((*it).deleted)
		{
			amount++;
			std::remove((*it).id.c_str());
			it = c->messages.erase(it);
		}
		else
		{
			++it;
		}
	}

	std::string msg = "+OK OP3 server signing off (" + std::to_string(amount) + " messages left)";
	c->network.send(msg.c_str());

	// close connection
	c->network.closeConnection();
}

void Server::noopCmd(Server *c, std::vector<std::string>v)
{
	if (c->status != Server::TRANSACTION)
	{
		return;
	}

	c->network.send("+OK");
}
