# Copyright 2023 Toma-Ioan Dumitrescu

# variables
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# globbing
SRCS=$(wildcard *.c)
H=$(wildcard *.h)

OBJECTS=$(SRCS:%.c=%.o)
TARGETS=$(OBJS:%.o=%)

build:
	$(CC) -o my_octave $(SRCS) $(CFLAGS)

pack:
	zip -FSr octave_simualtor.zip 'README' Makefile $(SRCS) $(H)

clean:
	rm -f $(TARGETS) $(OBJECTS)

.PHONY: pack clean
