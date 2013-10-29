#apt-get install libevent-dev

all: server client

server:server.c
	gcc -o server server.c -levent
client:client.c
	gcc -o client client.c -levent

clean:
	rm -f client server

