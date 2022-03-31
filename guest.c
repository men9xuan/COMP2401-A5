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

int wait;
int clientSocket;
struct sockaddr_in serverAddress;
int status, bytesRcv;
char outStr[80];  // stores guest request
char buffer[200]; 

void handleSig1(int i)
{
	wait=1;
}
void handleSig2(int i)
{
	wait=0;
}

// helper function for guest to connect to server
void connectServer()
{
	// Set up client server
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
}

void main(int argc, char *argv[])
{
	// SIGUSR handler
	signal(SIGUSR1, handleSig1);
	signal(SIGUSR2, handleSig2);

	// Set the random seed using nano-seconds
	// Otherwise guest will get same seed because they are generated so closely
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	srand((time_t)ts.tv_nsec);

	// Get the number of tickets, willing wait time and first ride from the command line arguments
	int ticketNum = atoi(argv[1]);
	int willWaitTime = atoi(argv[2]);
	int rideId = atoi(argv[3]);
	int tickets[NUM_RIDES];
	connectServer();
	// send ADMIT and guest id to server
	sprintf(outStr, "%d", ADMIT);
	sprintf(outStr + strlen(outStr), "%d", getpid());
	strcpy(buffer, outStr);

	send(clientSocket, &buffer, strlen(buffer), 0);
	bytesRcv = recv(clientSocket, buffer, 200, 0);
	close(clientSocket);

	// Request a admission to the fair.  If cannot get in (i.e., MAX_GUESTS reached), then quit.
	if (buffer[0] == '0')
	{
		return;
	}
	else
	{
		// if admitted convert returned string into tickets[];
		buffer[bytesRcv] = 0;
		char *p = strtok(buffer, " ");
		int i = 0;
		while (p)
		{
			int temp = strtol(p, NULL, 10);
			if (temp)
			{
				tickets[i++] = temp;
			}
			p = strtok(NULL, " ");
		}
	}

	// Now simulate the going on rides until no more tickets remain
	while (ticketNum > 0)
	{
		// Make sure that the guest has enough tickets for the desired ride
		// otherwise chose a different ride
		while (ticketNum < tickets[rideId])
		{
			rideId = rand() % NUM_RIDES;
		}
		// Get wait time estimate for that ride
		memset(outStr, 0, sizeof(outStr));
		sprintf(outStr, "%d", GET_WAIT_ESTIMATE);
		sprintf(outStr + strlen(outStr), "%d", rideId);
		strcpy(buffer, outStr);
		connectServer();
		send(clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, sizeof(buffer));
		bytesRcv = recv(clientSocket, buffer, 200, 0);
		buffer[bytesRcv] = 0;
		close(clientSocket);
		int waitTime = atoi(buffer);
		// If the guest is willing to wait, then get into line for that ride
		if (waitTime <= willWaitTime)
		{
			// connect and send to server that the guest is willing to get in line
			memset(outStr, 0, sizeof(outStr));
			sprintf(outStr, "%d %d  %d", GET_IN_LINE, rideId, getpid());
			strcpy(buffer, outStr);
			connectServer();
			send(clientSocket, buffer, strlen(buffer), 0);
			bytesRcv = recv(clientSocket, buffer, 200, 0);
			close(clientSocket);
			if (buffer[0] == '1')
			{
				ticketNum -= tickets[rideId];
				// Wait until the ride has boarded this guest, completed the ride and unboarded the guest
				wait = 2;
				// Wait till the USRIG1 and USRSIG2
				while (wait == 2)
				{
					sleep(1);
				}
				while (wait == 1)
				{
					sleep(1);
				}
			}
		}
		// pick next ride 
		rideId = rand() % NUM_RIDES;
		// Delay a bit (DO NOT CHANGE THIS LINE)
		usleep(100000);
	}

	// When out of tickets, inform the Fair that you are leaving
	// sent LEAVE_FAIR and guestId to server 
	memset(outStr, 0, sizeof(outStr));
	sprintf(outStr, "%d", LEAVE_FAIR);
	sprintf(outStr + strlen(outStr), "%d", getpid());
	strcpy(buffer, outStr);
	connectServer();
	send(clientSocket, buffer, strlen(buffer), 0);
	close(clientSocket);
}
