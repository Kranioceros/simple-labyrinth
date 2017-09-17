SRC_DIR=src
OBJECTS=mazegenerator.o prueba_mazegen.o
EXECUTABLE=main
CC=clang
CFLAGS=-std=c99 -Wall -g

all: labyrinth $(OBJECTS)

labyrinth: $(SRC_DIR)/labyrinth.c
	$(CC) $(CFLAGS) $(SRC_DIR)/labyrinth.c -o labyrinth.out

prueba_mazegen: $(SRC_DIR)/prueba_mazegen.c $(SRC_DIR)/mazegenerator.c
	$(CC) $(CFLAGS) $(SRC_DIR)/prueba_mazegen.c $(SRC_DIR)/mazegenerator.c -o prueba_mazegen.out
