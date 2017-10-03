#include "../headers/graphgeneration.h"
#include <stdlib.h>
#include <stdio.h>

int opuesta(int dir) {
	switch(dir) {
		case NORTE: 	return SUR;
		case SUR: 	return NORTE;
		case OESTE: 	return ESTE;
		case ESTE: 	return OESTE;
	}
	return NADA;
}

nodo* crearNodo() {
	nodo* n = malloc(sizeof(nodo));
	n->extremo = -1;
	n->visitado = 0;
	n->dir_anterior = NADA;
	for (int i = NORTE; i <= OESTE; i++)
		n->caminos[i] = NULL;

	return n;
}
nodo* agregarNodo(nodo* n, int dir) {
	if ( dir == NADA ) {
		puts("Error (agregarNodo): NADA no es una direccion valida");
		return NULL;
	}
	if ( n->caminos[dir] == NULL) {
		// Se calcula la direccion opuesta a dir
		int dir_op = opuesta(dir);
		// Se coloca al nodo nuevo en el camino dir de n
		nodo* nuevo = n->caminos[dir] = crearNodo();
		// Se marca el nodo n como el anterior del agregado
		n->caminos[dir]->dir_anterior = opuesta(dir);
		// Se coloca a n en el camino dir_op del nodo nuevo
		nuevo->caminos[dir_op] = n;
	} else {
		puts("Error (agregarNodo): Ya existe un nodo en esta ubicacion");
		return NULL;
	}

	return n;
}
nodo* moverse(nodo* n, int dir) {
	if (dir == NADA) {
		puts("Error (moverse): NADA no es una direccion valida");
		return NULL;
	}
	if (n->caminos[dir] == NULL) {
		puts("Error (moverse): No existe el nodo");
		return NULL;
	}
	else
		return n->caminos[dir];

	return NULL;
}

void borrarGrafo(nodo* n) {
	for (int i = NORTE; i <= OESTE; i++) {
		// Para borrar uno de los nodos que se encuentra en el arreglo
		// caminos, NO debe tratarse del mismo nodo origen (anterior).
		// Tampoco debe ser NULL, ya que no se puede dereferenciar.
		if (n->caminos[i] && i != n->dir_anterior) {
			borrarGrafo(n->caminos[i]);
		}
	}
	free(n);
}
