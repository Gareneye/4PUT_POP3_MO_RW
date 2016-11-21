#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <exception>
#include <string>
#include <thread>

#define DEFAULT_BUFLEN 512
#define POP3_PORT "110"

typedef std::exception NetworkException;

class Network
{
public:
	Network();
	virtual ~Network();

protected:
	WSADATA wsaData;
	addrinfo *result = NULL, *ptr = NULL, hints;

	bool sendData(SOCKET, const char*);
	bool recData(SOCKET, std::string&);
};

