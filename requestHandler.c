#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h>



// Handle client requests coming in through the server socket.  This code should run
// indefinitiely.  It should wait for a client to send a request, process it, and then
// close the client connection and wait for another client.  The requests that may be
// handled are as follows:
//
//   SHUTDOWN - causes the fair server to go offline.  No response is returned.
//
//   ADMIT - contains guest's process ID as well. return a list of all rides and their
//			 ticketRequirements.
//
//	 GET_WAIT_ESTIMATE - takes a ride ID as well.   It then returns an estimate as to 
//						 how long of a wait (in seconds) the guest would have to wait
//						 in order to get on the ride.
//
//	 GET_IN_LINE - takes a ride ID and guest's process ID as well.  It then causes the
//				   guest to get in line for the specified ride ... assuming that the 
//				   ride ID was valid and that the line hasn't reached its maximum.
//				   An OK response should be returned if all went well, otherwise NO.
// 
//   LEAVE_FAIR - takes a guest's process ID.  It then causes the guest to leave the fair.
//				  No response is returned.

void *handleIncomingRequests(void *x) {

	// Replace the code below with your own code
	while(1) {
	}
}