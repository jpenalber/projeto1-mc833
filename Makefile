all: client server resetdb

client: client.c
	gcc client.c -O2 -std=c11 -o client.x

server: server.c
	gcc server.c -O2 -std=c11 -o server.x

resetdb: resetdb.c
	gcc resetdb.c -std=c99 -lsqlite3 -o resetdb.x

dbtest: dbtest.c dbinterface.c dbinterface.h
	gcc dbtest.c dbinterface.c -std=c99 -lsqlite3 -o test.x