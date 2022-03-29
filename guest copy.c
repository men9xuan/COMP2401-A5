#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "fair.h"

// This program takes 3 command line arguments.
// 1 - The number of tickets that the guest has (e.g., 5 to 40)
// 2 - The maximum time (in seconds) that the guest is willing to wait in line for a ride (e.g., 600 - 1200)
// 3 - The first ride that this guest wants to go on (i.e., 0 to NUM_RIDES)

// ch5 pg 203 need a handler
volatile int wait;

void handleSig1(int i)
{
	wait--;
}
void handleSig2(int i)
{
	wait--;
}

void main(int argc, char *argv[])
{
	// SIGUSR handler
	signal(SIGUSR1, handleSig1);
	signal(SIGUSR2, handleSig2);

	// Set the random seed
	srand(time(NULL));

	// Get the number of tickets, willing wait time and first ride from the command line arguments
	// ...
	int ticketNum = atoi(argv[1]);
	int willWaitTime = atoi(argv[2]);
	int rideId = atoi(argv[3]);

	int tickets[NUM_RIDES];

	// Set up client server
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

	sprintf(outStr, "%d", ADMIT);
	sprintf(outStr + strlen(outStr), "%d", getpid());
	strcpy(buffer, outStr);
	printf("CLIENT: Sending \"%s\" to server.\n", buffer);

	send(clientSocket, &buffer, strlen(buffer), 0);
	bytesRcv = recv(clientSocket, buffer, 200, 0);
	// close(clientSocket);

	// Request a admission to the fair.  If cannot get in (i.e., MAX_GUESTS reached), then quit.
	if (buffer[0] == '0')
	{
		printf("not admitted\n");
		return;
	}
	else
	{
		// if admitted store the ride tickets into tickets[];
		printf("admitted\n");
		buffer[bytesRcv] = 0;
		printf("%s\n", buffer);
		char *p = strtok(buffer, " ");
		int i = 0;
		while (p)
		{
			// tickets[i++] = strtol(p, NULL, 10);
			int temp = strtol(p, NULL, 10);
			if (temp)
			{
				tickets[i++] = temp;
				// printf("## %d\n", temp);
			}
			p = strtok(NULL, " ");
		}
	}

	// Now simulate the going on rides until no more tickets remain (you will want to change the "1" here)
	while (ticketNum > 0)
	{
		// Make sure that the guest has enough tickets for the desired ride
		// otherwise chose a different ride
		while (ticketNum < tickets[rideId])
		{
			rideId = rand() % NUM_RIDES;
		}

		// Get wait time estimate for that ride
		// ...
		// status = connect(clientSocket, (struct sockaddr *)&serverAddress,
		// 				 sizeof(serverAddress));
		// if (status < 0)
		// {
		// 	printf("*** CLIENT ERROR: Could not connect.\n");
		// 	exit(-1);
		// }
		memset(outStr, 0, sizeof(outStr));
		sprintf(outStr, "%d", GET_WAIT_ESTIMATE);
		sprintf(outStr + strlen(outStr), "%d", rideId);
		strcpy(buffer, outStr);
		// buffer[2] = 0;
		printf("Request waitTime: Sending \"%s\" to server.\n", buffer);
		send(clientSocket, buffer, strlen(buffer), 0);
		// memset(buffer, 0, sizeof(buffer));
		bytesRcv = recv(clientSocket, buffer, 200, 0);
		buffer[bytesRcv] = 0;

		int waitTime = atoi(buffer);
		printf("**recived waitTime: %d\n", waitTime);
		// close(clientSocket);

		// If the guest is willing to wait, then get into line for that ride
		if (waitTime <= willWaitTime)
		{
			memset(outStr, 0, sizeof(outStr));
			sprintf(outStr, "%d", GET_IN_LINE);
			sprintf(outStr + strlen(outStr), "%d", getpid());
			strcpy(buffer, outStr);
			printf("requestGetInLine: Sending \"%s\" to server.\n", buffer);
			send(clientSocket, buffer, strlen(buffer), 0);
			bytesRcv = recv(clientSocket, buffer, 200, 0);
			buffer[bytesRcv] = 0;
			if (buffer[0] == '1')
			{
				ticketNum -= tickets[rideId];
				printf("Tickets left: %d\n", ticketNum);
				// int waitTime = atoi(buffer + 1);
				// printf("sleep time %d\n", waitTime);
				// usleep(waitTime / 10);
				wait = 2;

				while (wait == 2);
				// wait for board

				while (wait == 1);
				// wait for unboard
			}
		}

		// Wait until the ride has boarded this guest, completed the ride and unboarded the guest

		//?? usleap or sleep very long wait for sleep

		// Delay a bit (DO NOT CHANGE THIS LINE)
		usleep(100000);
		rideId = rand() % NUM_RIDES;
		// ...
	}

	// When out of tickets, inform the Fair that you are leaving
	memset(outStr, 0, sizeof(outStr));
	sprintf(outStr, "%d", LEAVE_FAIR);
	sprintf(outStr + strlen(outStr), "%d", getpid());
	strcpy(buffer, outStr);
	printf("LeaveFair: Sending \"%s\" to server.\n", buffer);
	send(clientSocket, buffer, strlen(buffer), 0);
}
