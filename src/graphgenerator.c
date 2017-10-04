#include "../headers/graphgeneration.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int opuesta(int dir) {
	switch(dir) {
		case NORTE: 	return SUR;
		case SUR: 	return NORTE;
		case OESTE: 	return ESTE;
		case ESTE: 	return OESTE;
	}
	return NADA;
}

// Funcion para hacer debugging. No esta en la cabecera.
char dirtoc(int dir) {
	switch(dir) {
		case NORTE: return 'N';
		case SUR: return 'S';
		case ESTE: return 'E';
		case OESTE: return 'O';
		default: return 'X';
	}
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

// WARNING: Esta functin esta hecha de mimbre
void imprimirGrafo(nodo* n, int indentacion, char numeracion[], char sep[]) {
	char nro_nodo[100] = ""; sprintf(nro_nodo, "%d", opuesta(n->dir_anterior));
	strcat(numeracion, nro_nodo);


	fputs("->", stdout);
	printf("(%c): Nodo %s\n", dirtoc(opuesta(n->dir_anterior)), numeracion);

	//getchar();
	int hasta_adonde = 0;
	/* Se determina cuantos subnodos hay */
	for(int dir=NORTE; dir <= OESTE; dir++) {
		if (n->caminos[dir] != NULL && dir != n->dir_anterior)
			hasta_adonde = dir;
	}

	/* Expandimos sep */
	strcat(sep, "     |");

	/* Se imprimen todos los nodos necesarios con ese sep */
	//puts("Primer for");
	for(int dir=NORTE; dir < hasta_adonde; dir++) {
		if (n->caminos[dir] != NULL && dir != n->dir_anterior) {
			//printf("Dir actual: %d\n", dir);
			puts(sep);
			fputs(sep, stdout);
			//getchar();
			imprimirGrafo(n->caminos[dir], indentacion+1, numeracion, sep);
			//getchar();
		}
	}

	/* Acortamos sep, borrando el " |" */
	sep[5*indentacion] = '\0';
	/* Se agregan dos espacios en blanco */
	strcat(sep, "   ");

	/* Se imprimen todos los nodos necesarios con nuevo sep */
	//puts("Segundo for");
	for(int dir=hasta_adonde; dir <= OESTE; dir++) {
		if (n->caminos[dir] != NULL && dir != n->dir_anterior) {
			//printf("Dir actual: %d\n", dir);
			fputs(sep, stdout); fputs("|\n", stdout);
			fputs(sep, stdout); putchar('|');
			//getchar();
			imprimirGrafo(n->caminos[dir], indentacion+1, numeracion, sep);
			//getchar();
		}
	}

}
