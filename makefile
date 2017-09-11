SRC_DIR=src
OBJECTS=mazerunner.o
EXECUTABLE=main
CC=clang
CFLAGS=-x c -std=c99 -Wall -g

all: labyrinth $(OBJECTS)

labyrinth: $(SRC_DIR)/labyrinth.c
	$(CC) $(CFLAGS) $(SRC_DIR)/labyrinth.c -o labyrinth.out

mazerunner.o: $(SRC_DIR)/mazerunner.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/mazerunner.c -o mazerunner.o

