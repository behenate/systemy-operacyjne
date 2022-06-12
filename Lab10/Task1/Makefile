CC = gcc
CFLAGS = -Wall -g

.PHONY: client server
client:
	gcc -Wall client.c -o client -lpthread -lm

server:
	gcc -Wall server.c -o server -lpthread -lm

all:
	make server
	make client


clean:
	rm -f server client socket
