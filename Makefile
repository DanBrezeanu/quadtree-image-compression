#!/bin/bash

CC = gcc
CFLAGS = -g -Wall -std=gnu99
EXEC = quadtree

all: build

build: main.o pixel_map.o
	$(CC) $^ $(CFLAGS) -o $(EXEC) -lm

main.o: main.c
	$(CC) $^ $(CFLAGS) -c

pixel_map.o: pixel_map.c
	$(CC) $^ $(CFLAGS) -c

zip:
	zip quadTreeCompression.zip main.c pixel_map.c pixel_map.h
	zip -r quadTreeCompression.zip Images/*
	zip quadTreeCompression.zip COMPILING\ OPTIONS Makefile README checker.sh
run:
	./$(EXEC)

clean:
	rm -rf $(EXEC) *.o
