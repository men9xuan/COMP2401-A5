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

void main(int argc, char *argv[]) {
	
	// Set the random seed
	srand(time(NULL));
	
	// Get the number of tickets, willing wait time and first ride from the command line arguments
  	// ...

	// Request a admission to the fair.  If cannot get in (i.e., MAX_GUESTS reached), then quit.
	// ...

	// Now simulate the going on rides until no more tickets remain (you will want to change the "1" here)
	while (1) {
		// Make sure that the guest has enough tickets for the desired ride
		// otherwise chose a different ride
		// ...
		
		
		// Get wait time estimate for that ride
		// ...
		
		
		// If the guest is willing to wait, then get into line for that ride
		// ...
		
		
		// Wait until the ride has boarded this guest, completed the ride and unboarded the guest
		// ...
		
		
		// Delay a bit (DO NOT CHANGE THIS LINE)
		usleep(100000);
		
		// Choose a new ride at random
		// ...
	}
	
	// When out of tickets, inform the Fair that you are leaving
	// ...
}
