all:server.c client.c
	gcc -g -pthread -lrt -lncurses -lcurses server.c library.h -o server
	gcc -g -pthread -lrt -lncurses -lcurses client.c library.h -o client