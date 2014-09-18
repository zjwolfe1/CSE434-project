EXEC = run
CC = gcc
CFLAGS = -c -Wall

server:utils.o server.o
	$(CC) -g -o server utils.o server.o

client:utils.o client.o
	$(CC) -g -o client utils.o client.o

server.o:server.h server.c
	  $(CC) $(CFLAGS) server.c

client.o:client.c
	  $(CC) $(CFLAGS) client.c

utils.o :utils.h utils.c
	  $(CC) $(CFLAGS) utils.c

clean :
	rm *.o
	rm server client
