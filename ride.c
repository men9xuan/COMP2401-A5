#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>


// This code runs the ride forever ... until the process is killed
void *runRide(void *p) {
	Ride *r = (Ride *)p;
	
	// Add any code that you need here
	//...
  
  	// Go into an infinite loop to process the guests in line
  	while(r->status != OFF_LINE) {
    	switch (r->status) {
			case STOPPED:
				// Add code here ...
				break;
			case LOADING:
				// Add code here ...
				break;
			case RUNNING:
				// Add code here ...
				break;
			case UNLOADING:
				// Add code here ...
				break;
		}
		
		// Add code here
		// ...
		
		
		
		// Do not remove this line ... but you can put a large number here to slow things down a bit.
		// Make sure it is back to 10000 before you submit your code.
		usleep(10000);  // A 1/100 second delay to slow things down a little
	}
}