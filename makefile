CC = gcc

build:
	make server
	make client

server:
	$(CC) -o server server.c

client:
	$(CC) -o client client.c

clean:
	rm server client

