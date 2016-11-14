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

bool Network::sendData(SOCKET socket, char * sendbuf)
{
	int iResult = send(socket, sendbuf, (int)strlen(sendbuf), 0);

	if (iResult == SOCKET_ERROR) {
		throw NetworkException("send failed with error");

		closesocket(socket);
		WSACleanup();
		return false;
	}

	WSACleanup();

	return true;
}

char* Network::recData(SOCKET socket)
{
	int iResult;
	char recvbuf[DEFAULT_BUFLEN];

	do {
		iResult = recv(socket, recvbuf, DEFAULT_BUFLEN, 0);

		if (iResult > 0)
		{}
		else if (iResult == 0)
		{
			throw NetworkException("Connection closed");
		}
		else
		{
			throw NetworkException("recv failed with error");
		}

	} while (iResult > 0);

	WSACleanup();

	return recvbuf;
}

