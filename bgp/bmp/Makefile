CC = gcc
CFLAGS = -Wall -Wextra -O2 -g -std=c99 -D_POSIX_C_SOURCE=200809L
LDFLAGS = 

SRCS = bgp.c bgp_client.c
OBJS = $(SRCS:.c=.o)
TARGET = bgp_client

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
