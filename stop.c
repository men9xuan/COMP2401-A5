#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "fair.h"

// This program stops the Fair server
void main()
{
	// WRITE ALL THE NECESSARY CODE
	int clientSocket;
	struct sockaddr_in serverAddress;
	int status, bytesRcv;
	char outStr[80];  // stores guest request
	char buffer[200]; //
	// Create the client socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket < 0)
	{
		printf("*** CLIENT ERROR: Could not open socket.\n");
		exit(-1);
	}
	// Setup address
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
	serverAddress.sin_port = htons((unsigned short)SERVER_PORT);
	// Connect to server
	status = connect(clientSocket, (struct sockaddr *)&serverAddress,
					 sizeof(serverAddress));
	if (status < 0)
	{
		printf("*** CLIENT ERROR: Could not connect.\n");
		exit(-1);
	}

	sprintf(outStr, "%d", SHUTDOWN);
	strcpy(buffer, outStr);

	send(clientSocket, &buffer, strlen(buffer), 0);
	close(clientSocket);
	return;
}
