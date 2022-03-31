all:		fairApp generator guest stop
		gcc -o fairApp fairApp.o -lX11 -lpthread -g
		gcc -o generator generator.o -g
		gcc -o guest guest.o -g
		gcc -o stop stop.o -g

fairApp:	fairApp.c display.c ride.c requestHandler.c fair.h display.h
		gcc -c fairApp.c -g

generator:	generator.c fair.h 
		gcc -c generator.c -g

guest: 		guest.c fair.h
		gcc -c guest.c -g

stop:		stop.c fair.h
		gcc -c stop.c -g

clean:
		rm -f fairApp.o generator.o guest.o stop.o fairApp guest generator stop

