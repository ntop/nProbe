CC=gcc
CFLAGS= -g -I/usr/local/include
LIBS=-L/usr/local/lib -lzmq -lz
OS := $(shell uname -s)

ifeq ($(OS),Darwin)
CFLAGS +=-fno-color-diagnostics
endif

all: subscriber

subscriber: subscriber.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f subscriber
