#include "NetworkServer.h"



NetworkServer::NetworkServer()
{
	checkLocalAddress();
	createSocket();
	bindSocket();
	listenSocket();
}


NetworkServer::~NetworkServer()
{}

void NetworkServer::acceptClient()
{
	ClientSocket = accept(ListenSocket, NULL, NULL);

	if (ClientSocket == INVALID_SOCKET) {
		closesocket(ListenSocket);
		WSACleanup();
		throw NetworkException("accept failed with error");
	}
}



void NetworkServer::createSocket()
{
	// Create Socket
	ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET) {
		freeaddrinfo(result);
		WSACleanup();
		throw NetworkException("Error at socket()");
	}
}

void NetworkServer::bindSocket()
{
	int iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR) {
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		throw NetworkException("bind failed with error");
	}
}

void NetworkServer::listenSocket()
{
	int iResult = listen(ListenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR) {
		closesocket(ListenSocket);
		WSACleanup();
		throw NetworkException("listen failed with error");
	}
}

void NetworkServer::checkLocalAddress()
{
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	int iResult = getaddrinfo(NULL, POP3_PORT, &hints, &result);

	if (iResult != 0) {
		WSACleanup();
		throw NetworkException("getaddrinfo failed");
	}
}

bool NetworkServer::send(char * data)
{
	return sendData(ClientSocket, data);
}

char * NetworkServer::rec()
{
	return recData(ClientSocket);
}
