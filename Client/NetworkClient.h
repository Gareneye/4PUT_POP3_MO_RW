#pragma once

#include "Network.h"

class NetworkClient : public Network
{
public:
	NetworkClient();
	virtual ~NetworkClient();

	bool connectToServer(char* address, char* port);
	bool send(const char*);
	bool rec(std::string&);

protected:
	SOCKET ConnectSocket;
};

