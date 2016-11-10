#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#include "Network.h"

int main()
{
	// Network
	Network network;
	network.createSocket();
	network.bindSocket();
	network.listenSocket();

	SOCKET client = network.acceptSocket();


	system("PAUSE");
	return 0;
}