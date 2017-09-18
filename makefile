SRC_DIR=src
OBJECTS=mazegenerator.o graphgenerator.o prueba_graphgen.o prueba_mazegen.o
EXECUTABLE=main
CC=clang
CFLAGS=-std=c99 -Wall -g

all: labyrinth $(OBJECTS)

labyrinth: $(SRC_DIR)/labyrinth.c
	$(CC) $(CFLAGS) $(SRC_DIR)/labyrinth.c -o labyrinth.out

prueba_mazegen: mazegenerator.o prueba_mazegen.o
	$(CC) $(CFLAGS) mazegenerator.o prueba_mazegen.o -o prueba_mazegen.out

prueba_graphgen: graphgenerator.o mazegenerator.o prueba_graphgen.o
	$(CC) $(CFLAGS) graphgenerator.o mazegenerator.o prueba_graphgen.o -o prueba_graphgen.out

mazegenerator.o: $(SRC_DIR)/mazegenerator.c 
	$(CC) $(CFLAGS) -c $(SRC_DIR)/mazegenerator.c -o mazegenerator.o

graphgenerator.o: $(SRC_DIR)/graphgenerator.c 
	$(CC) $(CFLAGS) -c $(SRC_DIR)/graphgenerator.c -o graphgenerator.o

prueba_mazegen.o: $(SRC_DIR)/prueba_mazegen.c 
	$(CC) $(CFLAGS) -c $(SRC_DIR)/prueba_mazegen.c -o prueba_mazegen.o

prueba_graphgen.o: $(SRC_DIR)/prueba_graphgen.c 
	$(CC) $(CFLAGS) -c $(SRC_DIR)/prueba_graphgen.c -o prueba_graphgen.o

