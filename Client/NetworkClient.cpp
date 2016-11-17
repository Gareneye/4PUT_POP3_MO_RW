#include "NetworkClient.h"



NetworkClient::NetworkClient()
{
	// Ini socket
	ConnectSocket = INVALID_SOCKET;

	// Hints
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

}


NetworkClient::~NetworkClient()
{
	closesocket(ConnectSocket);
}

bool NetworkClient::connectToServer(char* address, char* port)
{
	// Resolve the server address and port
	int iResult = getaddrinfo(address, port, &hints, &result);
	if (iResult != 0)
	{
		WSACleanup();
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (ConnectSocket == INVALID_SOCKET) {
			WSACleanup();
			return false;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}

		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		return false;
	}
	else
	{
		return true;
	}
}

bool NetworkClient::send(char * data)
{
	return sendData(ConnectSocket, data);
}

bool NetworkClient::rec(std::string& buffor)
{
	return recData(ConnectSocket, buffor);
}
