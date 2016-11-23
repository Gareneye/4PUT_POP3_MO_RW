#pragma once

#include "Network.h"

class NetworkServer : public Network
{
public:
	NetworkServer();
	virtual ~NetworkServer();

	void acceptClient();
	bool send(const char * data);
	bool rec(std::string&);

	void disconnectClient();
	void closeConnection();

protected:
	SOCKET ListenSocket;
	SOCKET ClientSocket;

	void checkLocalAddress();
	void createSocket();
	void bindSocket();
	void listenSocket();
};

