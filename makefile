EXEC = run
CC = gcc
CFLAGS = -c -Wall

server:utils.o server.o
	$(CC) -o server utils.o server.o

client:utils.o client.o
	$(CC) -o client utils.o client.o

server.o:server.c
	  $(CC) $(CFLAGS) server.c

client.o:client.c
	  $(CC) $(CFLAGS) client.c

utils.o :utils.h utils.c
	  $(CC) $(CFLAGS) utils.c

clean :
	rm *.o
	rm server client
