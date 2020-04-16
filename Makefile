INCDIR = ./include/
CFLAGS = -Wall -O2 -std=c11

all: bin_dir client server resetdb

client: src/client.c
	gcc src/client.c -O2 -std=c11 -o bin/client.x -I$(INCDIR)

server: src/server.c src/dbinterface.c
	gcc src/server.c src/dbinterface.c -O2 -std=c11 -o bin/server.x -lsqlite3 -I$(INCDIR)

resetdb: src/resetdb.c
	gcc src/resetdb.c -std=c99 -lsqlite3 -o bin/resetdb.x -I$(INCDIR)

bin_dir:
	@mkdir -p bin
