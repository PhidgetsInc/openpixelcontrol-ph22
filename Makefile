platform=$(shell uname)
CFLAGS=-O2 -g
ALL=bin/dummy_client bin/dummy_server bin/Phidget_server

all: $(ALL)

clean:
	rm -rf bin/*

bin/dummy_client: src/dummy_client.c src/opc_client.c src/opc.h src/types.h
	mkdir -p bin
	gcc ${CFLAGS} -o $@ src/dummy_client.c src/opc_client.c

bin/dummy_server: src/dummy_server.c src/opc_server.c src/opc.h src/types.h
	mkdir -p bin
	gcc ${CFLAGS} -o $@ src/dummy_server.c src/opc_server.c

bin/Phidget_server: src/Phidget_server.c src/opc_server.c src/opc.h src/types.h
	mkdir -p bin
	gcc ${CFLAGS} -o $@ src/Phidget_server.c src/opc_server.c -lphidget22

