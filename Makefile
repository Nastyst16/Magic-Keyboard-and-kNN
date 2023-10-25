# Copyright 2023 Nastase Cristian-Gabriel 315CA

# compiler setup
CC=gcc -g
CFLAGS=-Wall -Wextra -Wshadow -Wpedantic -std=c99 -O0 -g

# define targets
TARGETS=kNN mk

#define object-files
OBJ=mk.o kNN.o trie.o bst_lab09.o

build: $(TARGETS)

mk: mk.o trie.o
	$(CC) $(CFLAGS) $^ -o $@

kNN: kNN.o bst_lab09.o -lm
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

pack:
	zip -FSr 315CA_Cristian-Gabriel_Nastase_Tema3.zip README.md Makefile *.c *.h

clean:
	rm -f $(TARGETS) $(OBJ)

.PHONY: pack clean
