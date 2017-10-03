#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../headers/graphgeneration.h"
#include "../headers/mazegenerator.h"

static int x, y;
static repr_lab* rl;

void moverJugador(int dir);
int nodoDisponible(nodo* n);
int hayPared(int dir);
int estaDentro(int dir);

int main (int argc, char* argv[]) {
	if (argc < 7) {
		puts("Uso:\n\tprueba-graphgen <ancho> <alto> <posx> <posy>"
		"<pasos> <tiempoms>");
		return 0;
	}
	int ancho, alto, entradax, entraday, nropasos;
	long tiempoms; int tiempos;
	int semilla;

	sscanf(argv[1], "%d", &ancho);
	sscanf(argv[2], "%d", &alto);
	sscanf(argv[3], "%d", &entradax);
	sscanf(argv[4], "%d", &entraday);
	sscanf(argv[5], "%d", &nropasos);
	sscanf(argv[6], "%li", &tiempoms);
	sscanf(argv[7], "%i", &semilla);

	tiempos = tiempoms / 1000; tiempoms %= 1000;
	if (semilla < 0) semilla = time(NULL);

	/* Se inicializa el generador de numeros con la semilla */
	srand(semilla);

	struct timespec tiempo;	
	tiempo.tv_sec = tiempos; tiempo.tv_nsec = tiempoms * 1000000;

	rl = generarLaberinto(ancho, alto,
			entradax, entraday,
			ancho-1, alto-1,
		       	-1);	

	x = entradax; y = entraday;
	rl->celdas[y][x].cen = JUGADOR;
	dibujarLaberinto(rl);
	nanosleep(&tiempo, NULL);

	nodo* grafo = crearNodo();
	nodo* nodoActual = grafo;
	nodoActual->visitado=1;
	nodoActual->extremo=1;
	nodoActual->dir_anterior=NADA;

	if (nropasos < 0) nropasos = ancho*alto + 1;
	
	// Se mira alrededor y se agregan los nodos
	// Para agregarse debe cumplir las siguientes condiciones:
	// 	+ Debe existir una celda en esa direccion
	// 	+ No hay una pared que separe a esa celda de la actual
	// Si se intenta asignar un nodo a un camino que ya tiene asignado uno,
	// se lo deja intacto.
	puts("Mirando alrededor y agregando nodos...");
	for (int dir = NORTE; dir <= OESTE; dir++) {
		if (estaDentro(dir) && !hayPared(dir)) {
			puts("Nodo agregado!");
			printf("Direccion: %d\n", dir);
			printf("Direccion de memoria: %p\n", &nodoActual->caminos[dir]);
			agregarNodo(nodoActual, dir);
		}
	}

	int paso = 0;
	int dirActual = NORTE;

	while (paso < nropasos) {
		printf("Paso numero %d\n", paso + 1);
		// Se busca algun nodo adyacente que no haya sido visitado
		// PELIGRO! Puede dereferenciar puntero nulo (en teoria)
		puts("Eligiendo direccion de direcciones disponibles...");
		printf("Direccion actual: %d\n", dirActual);
		while( dirActual <= OESTE &&
		       !nodoDisponible(nodoActual->caminos[dirActual])) 
		{
			printf("Direccion %d no disponible\n", dirActual++);
		}

		// Si no se encontro ninguno, se retrocede
		if (dirActual > OESTE) {
			puts("No se encontro ninguna direccion. Retrocediendo...");
			// Si no hay nodo anterior, se ha vuelto a la entrada
			// y se termina el programa.
			if (nodoActual->dir_anterior == NADA) {
				puts("No hay donde retroceder. Break");
				break;
			} else {
				// Se mueve al jugador a la celda anterior
				moverJugador(nodoActual->dir_anterior);
				// Se mueve el puntero al nodo anterior
				nodoActual = moverse(nodoActual,
						     nodoActual->dir_anterior);
				dirActual = 0;
				dibujarLaberinto(rl);
				nanosleep(&tiempo, NULL);
			}
		} else {
			// Se encontro un nodo y se debe avanzar
			puts("Direccion que lleva a nodo encontrada. Avanzando...");

			paso++;
			// Se mueve al jugador a la celda siguiente
			moverJugador(dirActual);
			// El nodo siguiente pasa a ser el actual
			nodoActual = moverse(nodoActual,
				     dirActual);
			// Se marca el nuevo nodoActual como visitado
			nodoActual->visitado = 1;
			// Se reinicia la direccion actual
			dirActual = 0;

			// Se mira alrededor y se agregan los nodos
			puts("Mirando alrededor y agregando nodos...");
			for (int dir = NORTE; dir <= OESTE; dir++) {
				if (estaDentro(dir) && !hayPared(dir)) {
					puts("Nodo agregado!");
					printf("Direccion: %d\n", dir);
					printf("Direccion de memoria: %p\n",
							&nodoActual->caminos[dir]);
					agregarNodo(nodoActual, dir);
				}
			}
			dibujarLaberinto(rl);
			nanosleep(&tiempo, NULL);
		}

	}

	borrarGrafo(grafo);
	borrarRepr(rl);
}

void moverJugador(int dir) {
	int nuevox = x, nuevoy = y;
	switch(dir) {
		case NORTE:
			nuevoy--;
			break;
		case SUR:
			nuevoy++;
			break;
		case ESTE:
			nuevox++;
			break;
		case OESTE:
			nuevox--;
			break;
		default:
			puts("Error (moverJugador): Direccion no valida");
	}	

	if (estaDentro(dir)) {
		if (!hayPared(dir)) {
			rl->celdas[y][x].cen = VACIO;
			rl->celdas[nuevoy][nuevox].cen = JUGADOR;
			x=nuevox; y=nuevoy;
		} else {
			puts("Error (moverJugador): Hay un muro en la direccion indicada");
		}

	} else {
		puts("Error (moverJugador): Ubicacion fuera del laberinto");
	}	
}

int hayPared(int dir) {
	if (dir == NORTE && rl->celdas[y][x].nor == LLENO) 	return 1;
	if (dir == SUR && rl->celdas[y][x].sur == LLENO) 	return 1;
	if (dir == ESTE && rl->celdas[y][x].est == LLENO) 	return 1;
	if (dir == OESTE && rl->celdas[y][x].oes == LLENO) 	return 1;
	return 0;
}

int estaDentro(int dir) {
	int nuevox = x, nuevoy = y;
	switch(dir) {
		case NORTE:
			nuevoy--;
			break;
		case SUR:
			nuevoy++;
			break;
		case ESTE:
			nuevox++;
			break;
		case OESTE:
			nuevox--;
			break;
		default:
			puts("Error (estaDentro): Direccion no valida");
	}

	if (nuevox < rl->ancho && nuevox > -1 &&
			nuevoy < rl->alto && nuevoy > -1) {
		return 1;
	}
	return 0;
}

int nodoDisponible(nodo* n) {
	if (n) {
		puts("Existe el nodo");
		if (!n->visitado) {
			puts("El nodo no fue visitado!");
			return 1;
		} else {
			puts("El nodo ya fue visitado");
			return 0;
		}
	}
	puts("No existe el nodo");
	return 0;
}
