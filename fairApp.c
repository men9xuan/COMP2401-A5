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

sem_t   serverBusyIndicator;
// Initialize a ride
void initializeRide(Ride *r, char *n, unsigned char tr, unsigned char cap, unsigned char onOff, unsigned short rt, unsigned short wt)
{
	r->name = n;
	r->ticketsRequired = tr;
	r->capacity = cap;
	r->onOffTime = onOff;
	r->rideTime = rt;
	r->waitTime = wt;
	r->lineupSize = 0;
	r->riders = (unsigned int *)malloc(sizeof(unsigned int) * r->capacity);
	if (r->riders == NULL)
	{
		printf("FAIR APP: Error allocating array for riders\n");
		exit(-1);
	}
	r->numRiders = 0;
	r->status = STOPPED;
}

// This is where it all begins
int main()
{
	// Initialize semaphore
	sem_init(&serverBusyIndicator, 0, 1);
	// Create a fair with no guests
	Fair ottawaFair;
	ottawaFair.numGuests = 0;

	// Fill in the fair's ride information
	initializeRide(&(ottawaFair.rides[9]), "Ferris Wheel", 4, 32, 10, 600, 120);
	initializeRide(&(ottawaFair.rides[8]), "Pirate Ship", 4, 30, 5, 240, 120);
	initializeRide(&(ottawaFair.rides[7]), "Merry-Go-Round", 2, 25, 4, 240, 60);
	initializeRide(&(ottawaFair.rides[6]), "Roller Coaster", 5, 24, 5, 75, 60);
	initializeRide(&(ottawaFair.rides[5]), "Fun World", 1, 20, 1, 180, 1);
	initializeRide(&(ottawaFair.rides[4]), "Calm Train", 3, 16, 6, 300, 60);
	initializeRide(&(ottawaFair.rides[3]), "Back Destroyer", 4, 12, 10, 90, 30);
	initializeRide(&(ottawaFair.rides[2]), "Tea Cups", 3, 10, 10, 120, 60);
	initializeRide(&(ottawaFair.rides[1]), "Drop To Death", 5, 8, 20, 20, 30);
	initializeRide(&(ottawaFair.rides[0]), "Tummy Tosser", 5, 6, 7, 60, 30);

	// Start up the ride threads
	pthread_t rideThreads[NUM_RIDES];
	for (int i = 0; i < NUM_RIDES; i++)
	{
		pthread_create(&rideThreads[i], NULL, runRide, &(ottawaFair.rides[i]));
	}

	// Spawn a thread to handle incoming requests from guests
	pthread_t handler;
	pthread_create(&handler, NULL, handleIncomingRequests, &ottawaFair);

	// Spawn a thread to handle display
	pthread_t display;
	pthread_create(&display, NULL, showSimulation, &ottawaFair);

	// Wait for the incoming requests thread to complete, from a STOP command
	// wait for 	pthread_t handle to stop
	pthread_join(handler, NULL);

	// Shutdown the ride threads and free up the riders arrays
	// ...
	for (int i = 0; i < NUM_RIDES; i++)
	{
		// ((Ride *)&rideThreads[i])->status = OFF_LINE;
		ottawaFair.rides[i].status = OFF_LINE;
		pthread_join(rideThreads[i], NULL);
		free(((ottawaFair.rides[i]).riders));
	}
	// pthread_join(display, NULL);

	// Kill all the guest processes for any guests remaining
	// ... // MAX_GUESTS
	for (int i = 0; i < ottawaFair.numGuests; i++)
	{
		kill(ottawaFair.guestIDs[i], SIGKILL);
	}

	// free pthreads
	// pthread_join(display, NULL);
	// // pthread_exit(&display);
	// for (int i = 0; i < NUM_RIDES; i++)
	// {
	// 	pthread_join(rideThreads[i], NULL);
	// 	// pthread_exit(&rideThreads[i]);
	// }

	printf("FAIR APP: ended successfully\n");
}