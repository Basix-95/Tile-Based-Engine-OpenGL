CC = gcc
CFLAGS = -O2 -Wall -std=c11 $(shell pkg-config --cflags glfw3) -I./src
LIBS = $(shell pkg-config --libs glfw3) -lGL -lm -ldl -pthread

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = platformer

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

