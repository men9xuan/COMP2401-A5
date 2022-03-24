#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>

#include "fair.h"

#include "ride.c"
#include "requestHandler.c"
#include "display.c"


// Initialize a ride
void initializeRide(Ride *r, char *n, unsigned char tr, unsigned char cap,  unsigned char onOff, unsigned short rt, unsigned short wt) {
	r->name = n;
	r->ticketsRequired = tr;
	r->capacity = cap;
	r->onOffTime = onOff;
	r->rideTime = rt;
	r->waitTime = wt;
	r->lineupSize = 0;
	r->riders = (unsigned int *) malloc(sizeof(unsigned int) * r->capacity);
	if (r->riders == NULL) {
		printf("FAIR APP: Error allocating array for riders\n");
		exit(-1);
	}
	r->numRiders = 0;
	r->status = STOPPED;
}



// This is where it all begins
int main() {
	// Create a fair with no guests
	Fair	ottawaFair;
	ottawaFair.numGuests = 0;
	
	// Fill in the fair's ride information
	initializeRide(&(ottawaFair.rides[9]), "Ferris Wheel",   4, 32, 10, 600, 120);
	initializeRide(&(ottawaFair.rides[8]), "Pirate Ship",    4, 30,  5, 240, 120);
	initializeRide(&(ottawaFair.rides[7]), "Merry-Go-Round", 2, 25,  4, 240,  60);
	initializeRide(&(ottawaFair.rides[6]), "Roller Coaster", 5, 24,  5,  75,  60);
	initializeRide(&(ottawaFair.rides[5]), "Fun World",      1, 20,  1, 180,   1);
	initializeRide(&(ottawaFair.rides[4]), "Calm Train",     3, 16,  6, 300,  60);
	initializeRide(&(ottawaFair.rides[3]), "Back Destroyer", 4, 12, 10,  90,  30);
	initializeRide(&(ottawaFair.rides[2]), "Tea Cups",       3, 10, 10, 120,  60);
	initializeRide(&(ottawaFair.rides[1]), "Drop To Death",  5,  8, 20,  20,  30);
	initializeRide(&(ottawaFair.rides[0]), "Tummy Tosser",   5,  6,  7,  60,  30);
	
	// Start up the ride threads
	// ...

	// Spawn a thread to handle incoming requests from guests
  	// ...

  	// Spawn a thread to handle display
  	// ...

    // Wait for the incoming requests thread to complete, from a STOP command
  	// ...

	// Shutdown the ride threads and free up the riders arrays
	// ...
		
  	// Kill all the guest processes for any guests remaining
  	// ...
  	
    printf("FAIR APP: ended successfully\n");
}