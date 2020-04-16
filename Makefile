all: client server

client: client.c
	gcc client.c -O2 -std=c11 -o client.x

server: server.c
	gcc server.c -O2 -std=c11 -o server.x
