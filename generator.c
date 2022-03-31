#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "fair.h"

void main()
{
	// Set up the random seed
	srand(time(NULL));

	// Startup 100 guests with ...
	// - a random number of tickets in the range of 5 to 40
	// - a random willing-to-wait time in the range of 600 to 900 seconds
	// - a random ride to go on first in teh range of 0 to NUM_RIDES
	//
	for (int i = 0; i < 100; i++)
	{
		// Add code here
		int ticketsNum = rand() % 36 + 5;
		int waitTime = rand() % 301 + 600;
		int firstRide = rand() % NUM_RIDES;
		// ...
		char command[50];
		// generate system command string
		sprintf(command, "./guest %d %d %d &", ticketsNum, waitTime, firstRide);
		system(command);
		usleep(10000); // Keep this 1/100 second delay between guest creation
	}
}