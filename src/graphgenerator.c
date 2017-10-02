#include "../headers/graphgeneration.h"
#include <stdlib.h>
#include <stdio.h>

nodo* crearNodo() {
	nodo* n = malloc(sizeof(nodo));
	n->extremo = -1;
	n->visitado = 0;
	n->anterior = NULL;
	for (int i = 0; i < 4; i++)
		n->caminos[i] = NULL;

	return n;
}
nodo* agregarNodo(nodo* n, int dir) {
	if ( n->caminos[dir] == NULL) {
		// Se coloca al nodo nuevo en el camino dir de n
		nodo* nuevo = n->caminos[dir] = crearNodo();
		// Se marca el nodo n como el anterior del agregado
		n->caminos[dir]->anterior = n;
		// Se coloca a n en el camino !dir del nodo nuevo
		switch (dir) {
			case NORTE: nuevo->caminos[SUR] = n; break;
			case SUR: nuevo->caminos[NORTE] = n; break;
			case ESTE: nuevo->caminos[OESTE] = n; break;
			case OESTE: nuevo->caminos[ESTE] = n; break;
			default: puts("Error (agregarNodo): Direccion no valida");
		}
	} else {
		puts("Error (agregarNodo): Ya existe un nodo en esta ubicacion");
	}

	return n;
}
nodo* moverse(nodo* n, int dir) {
	if (n->caminos[dir] == NULL)
		puts("Error (moverse): No existe el nodo");
	else
		return n->caminos[dir];
	return NULL;
}
void borrarGrafo(nodo* n) {
	for (int i = 0; i < 4; i++) {
		// Para borrar uno de los nodos que se encuentra en el arreglo
		// caminos, NO debe tratarse del mismo nodo origen (anterior).
		// Tampoco debe ser NULL, ya que no se puede dereferenciar.
		if (n->caminos[i] && n->caminos[i] != n->anterior) {
			borrarGrafo(n->caminos[i]);
		}
	}
	free(n);
}
