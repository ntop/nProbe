CC=gcc -I /usr/local/include
CFLAGS= -g -L /usr/local/lib -lzmq

all: publisher

publisher: publisher.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm -f publisher
