#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <exception>
#include <string>

#define POP3_PORT "110"

typedef std::exception NetworkException;

class Network
{
public:
	Network();
	virtual ~Network();

	void createSocket();
	void bindSocket();
	void listenSocket();
	SOCKET acceptSocket();

private:
	WSADATA wsaData;
	addrinfo *addrInfo;
	SOCKET ListenSocket;

	void checkLocalAddress();
};

