#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
extern sem_t serverBusyIndicator;

// This code runs the ride forever ... until the process is killed
void *runRide(void *p)
{
	Ride *r = (Ride *)p;
	int riderId = -1;
	// default time and state
	r->status = STOPPED;
	r->countdownTimer = 0;

	// Go into an infinite loop to process the guests in line
	while (r->status != OFF_LINE)
	{
		switch (r->status)
		{
		case STOPPED:
			// Code for stoped
			if ((r->lineupSize) > 0)
			{
				r->status = LOADING;
				r->countdownTimer = r->onOffTime;
			}
			if ((r->numRiders) > 0 && (r->countdownTimer == 0))
			{
				r->status = RUNNING;
				r->countdownTimer = r->rideTime;
			}
			break;
		case LOADING:
			// Code for loading
			if (r->countdownTimer == 0)
			{
				sem_wait(&serverBusyIndicator);
				riderId = r->waitingLine[0];
				// send SIGUSR1 to first guest
				kill(riderId, SIGUSR1);
				r->lineupSize--;
				r->riders[r->numRiders] = riderId;
				r->numRiders++;
				for (int i = 0; i < (r->lineupSize); i++)
				{
					r->waitingLine[i] = r->waitingLine[i + 1];
				}
				if ((r->numRiders) == (r->capacity))
				{
					r->status = RUNNING;
					r->countdownTimer = r->rideTime;
				}
				else
				{
					r->status = STOPPED;
					r->countdownTimer = r->waitTime;
				}
				sem_post(&serverBusyIndicator);
			}
			break;
		case RUNNING:
			// Code for running
			if (r->countdownTimer == 0)
			{
				r->status = UNLOADING;
				r->countdownTimer = r->onOffTime;
			}
			break;
		case UNLOADING:
			// Code for unloading
			while ((r->numRiders) > 0 && r->countdownTimer == 0)
			{
				// send SIGUSR2 to first guest
				kill(r->riders[0], SIGUSR2);
				sem_wait(&serverBusyIndicator);
				// decrease r->numRiders by 1
				r->numRiders--;
				// shift r->riders array
				for (int i = 0; i < (r->numRiders); i++)
				{
					r->riders[i] = r->riders[i + 1];
				}
				sem_post(&serverBusyIndicator);
				// reset unload timer
				r->countdownTimer = r->onOffTime;
			}

			// after all guests unloaded reset timer
			if ((r->numRiders) == 0)
			{
				r->status = STOPPED;
				r->countdownTimer = r->waitTime;
			}
			break;
		}
		// decrease timer 
		if (r->countdownTimer > 0)
			r->countdownTimer--;
		// Do not remove this line ... but you can put a large number here to slow things down a bit.
		// Make sure it is back to 10000 before you submit your code.
		usleep(10000);
	}
}