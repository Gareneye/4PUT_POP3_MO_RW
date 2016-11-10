#include "Network.h"



Network::Network()
{
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		throw NetworkException("WSAStartup failed");
	}

	this->checkLocalAddress();
}


Network::~Network()
{
	freeaddrinfo(addrInfo);
	closesocket(ListenSocket);
	WSACleanup();
}

void Network::createSocket()
{
	// Create Socket
	this->ListenSocket = INVALID_SOCKET;
	this->ListenSocket = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);

	if (this->ListenSocket == INVALID_SOCKET) {
		throw NetworkException("Error at socket()");
	}
}

void Network::bindSocket()
{
	int iResult = bind(ListenSocket, addrInfo->ai_addr, (int)addrInfo->ai_addrlen);

	if (iResult == SOCKET_ERROR) {
		throw NetworkException("bind failed with error");
	}
}

void Network::listenSocket()
{
	int iResult = listen(ListenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR) {
		throw NetworkException("listen failed with error");
	}
}

SOCKET Network::acceptSocket()
{
	SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);

	if (ClientSocket == INVALID_SOCKET) {
		throw NetworkException("accept failed with error");
	}

	return ClientSocket;
}

void Network::checkLocalAddress()
{
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	int iResult = getaddrinfo(NULL, POP3_PORT, &hints, &addrInfo);

	if (iResult != 0) {
		throw NetworkException("getaddrinfo failed");
	}
}
