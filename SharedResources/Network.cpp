#include "Network.h"



Network::Network()
{
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		throw NetworkException("WSAStartup failed");
	}
}


Network::~Network()
{
	WSACleanup();
}

#include <iostream>
bool Network::sendData(SOCKET socket, const char* sendbuf)
{
	std::string buffor = sendbuf;
	bool fragment = false;

	if (buffor.length()+2 > DEFAULT_BUFLEN)
	{
		fragment = true;
	}
	else
	{
		buffor += "\r\n";
	}
	
	int iResult = send(socket, buffor.c_str(), (fragment ? DEFAULT_BUFLEN : buffor.length()), 0);

	if (iResult == SOCKET_ERROR) {
		//throw NetworkException("send failed with error");
		printf("send failed with error: %d\n", WSAGetLastError());

		closesocket(socket);
		WSACleanup();
		return false;
	}

	if (fragment)
	{
		this->sendData(socket, buffor.substr(DEFAULT_BUFLEN).c_str());
	}

	return true;
}


bool Network::recData(SOCKET socket, std::string& buffor)
{
	int iResult;
	char recvbuf[DEFAULT_BUFLEN];

	iResult = recv(socket, recvbuf, DEFAULT_BUFLEN, 0);

	if (iResult > 0)
	{
		// success
		std::string test(recvbuf);
		size_t pos = test.find_last_of("\r\n");
		buffor = test.substr(0, pos);

		return true;
	}
	else if (iResult == 0)
	{
		// connection closeing
		//printf("Connection closing...\n");
		return false;
	}
	else
	{
		printf("recv failed with error: %d\n", WSAGetLastError());

		// error
		closesocket(socket);
		WSACleanup();
		return false;
	}
}

