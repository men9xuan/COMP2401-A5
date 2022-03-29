#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>

// This code runs the ride forever ... until the process is killed
void *runRide(void *p)
{
	Ride *r = (Ride *)p;
	int riderId = -1;
	// Add any code that you need here
	//...
	r->status = STOPPED;
	r->countdownTimer =0;

	// Go into an infinite loop to process the guests in line
	while (r->status != OFF_LINE)
	{
		switch (r->status)
		{
		case STOPPED:

			// Add code here ...
			if ((r->lineupSize) > 0)
			{
				printf("ride.c lineupsize: %d\n", r->lineupSize);
				printf("LOADING\n");
				r->status = LOADING;
				r->countdownTimer = r->onOffTime;
			}
			if ((r->numRiders) > 0 && (r->countdownTimer == 0))
			{
				r->status = RUNNING;
				printf("RUNNNING\n");
				r->countdownTimer = r->rideTime;
			}
			break;
		case LOADING:
			// Add code here ...

			if (r->countdownTimer == 0)
			{
				riderId = r->waitingLine[0];
				printf("riderId: %d\n", riderId);
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
					printf("RUNNING\n");
					r->countdownTimer = r->rideTime;
				}
				else
				{
					r->status = STOPPED;
					printf("STOPPED\n");
					r->countdownTimer = r->waitTime;
				}
			}
			break;
		case RUNNING:

			// Add code here ...
			// usleep(r->rideTime);
			if (r->countdownTimer == 0)
			{
				r->status = UNLOADING;
				printf("UNLOADING\n");
				r->countdownTimer = r->onOffTime;
			}

			break;
		case UNLOADING:
			// Add code here ...

			while ((r->numRiders) > 0 && r->countdownTimer == 0)
			{

				printf("Unloading...\n");
				kill(r->riders[0], SIGUSR2);
				r->numRiders--;
				for (int i = 0; i < (r->numRiders); i++)
				{
					r->riders[i] = r->riders[i + 1];
				}
				r->countdownTimer = r->onOffTime;
			}

			// if (r->countdownTimer == 0)
			// {
			// 	for (int i = 0; i < (r->numRiders); i++)
			// 	{
			// 		kill(r->riders[i], SIGUSR2);
			// 		r->countdownTimer = r->onOffTime;
			// 	}
			// }

			// r->numRiders = 0;
			if ((r->numRiders) == 0)
			{
				printf("STOPPED\n");
				r->status = STOPPED;
				r->countdownTimer = r->waitTime;
			}

			break;
		}

		// Add code here
		// ...

		// Do not remove this line ... but you can put a large number here to slow things down a bit.
		// Make sure it is back to 10000 before you submit your code.
		if (r->countdownTimer > 0)
			r->countdownTimer--;
		usleep(10000);
		// usleep(1000000);
	}
}