#include <stdlib.h>
#include <stdio.h>
#include "../headers/mazegenerator.h"

/* Estructuras de datos internas. Solo son utilizadas para generar
 * laberintos, no para representarlos. Las estructuras para representarlos
 * se encuentran definidas en mazegenerator.h (repr_celda, sym). */

typedef enum {N=0, S, E, O, X} dir; // X: Ninguna direccion

typedef struct celda {
	// Indica si la celda fue visitada por el algoritmo generador
	int visitada;
	// Indica direccion desde la cual la celda fue visitada por primera vez
	dir conexion; 
	// Arreglo de 4 direcciones aleatorias
	dir* dirs;
	// Indica numero de direcciones empleadas del arreglo anterior
	int dir_actual; 
} celda;

typedef struct lab {
	// Datos generales del laberinto
	int alto, ancho;
	int entradax, salidax;
	int entraday, saliday;
	// Arreglo bidimensional de celdas que conforma el laberinto
	celda** celdas;
} lab;

/* Prototipos de funciones internas */

// Constructores y destructores
repr_lab* reprVacio(int, int); 
lab* laberintoVacio(int, int);
void borrarLaberinto(lab*);

// Funciones auxiliares
void inicializarCelda(celda* c); 
int calcX(int x, dir d);
int calcY(int y, dir d);
char dirtostr(dir d);
char diropuesta(dir d);
void romperPared(repr_celda *c, dir d, sym s);
char eligeSimbolo(sym s);
void obtenerDirs(dir*);
void labtostr(repr_celda**, int, int);
void marcarSalida(repr_lab* rep, sym s, int x, int y);

// Aqui sucede la magia
void continuarLaberinto (
		lab* l, repr_lab* rep,
		int posx, int posy,
		int nro_pasos, int celdas_visitadas);

/* Implementacion de Funciones Principales */

char* dibujoLaberinto(repr_lab* l) {
	int largobuf = (7 * l->alto * l->ancho) + (2*l->alto + 1);
	char* buffer = malloc(sizeof(char) * largobuf);
	int bpos=0;
	char centro, arriba, abajo, derecha, izquierda;

	for (int fila=0; fila < l->alto; fila++) {
		for (int col=0; col < l->ancho; col++) {
			//Muros de arriba
			arriba = eligeSimbolo(l->celdas[fila][col].nor);
			buffer[bpos++] = '#';
			buffer[bpos++] = arriba;
			buffer[bpos++] = '#';
		}
		buffer[bpos++] = '\n';
		for (int col=0; col < l->ancho; col++) {
			//Muros del medio
			izquierda = eligeSimbolo(l->celdas[fila][col].oes);
			derecha = eligeSimbolo(l->celdas[fila][col].est);
			centro = eligeSimbolo(l->celdas[fila][col].cen);
			buffer[bpos++] = izquierda;
			buffer[bpos++] = centro;
			buffer[bpos++] = derecha;
		}
		buffer[bpos++] = '\n';
	}

	for (int col=0; col < l->ancho; col++) {
		abajo = eligeSimbolo(l->celdas[l->alto-1][col].sur);
		buffer[bpos++] = '#';
		buffer[bpos++] = abajo;
		buffer[bpos++] = '#';
	}
	buffer[bpos++] = '\n';

	return buffer;
}

void dibujarLaberinto(repr_lab* l) {
	char centro, arriba, abajo, derecha, izquierda;

	for (int fila=0; fila < l->alto; fila++) {
		for (int col=0; col < l->ancho; col++) {
			//Muros de arriba
			arriba = eligeSimbolo(l->celdas[fila][col].nor);
			putchar('#');
			putchar(arriba);
			putchar('#');
		}
		putchar('\n');
		for (int col=0; col < l->ancho; col++) {
			//Muros del medio
			izquierda = eligeSimbolo(l->celdas[fila][col].oes);
			derecha = eligeSimbolo(l->celdas[fila][col].est);
			centro = eligeSimbolo(l->celdas[fila][col].cen);
			printf("%c%c%c", izquierda, centro, derecha);
		}
		putchar('\n');
	}
	for (int col=0; col < l->ancho; col++) {
		abajo = eligeSimbolo(l->celdas[l->alto-1][col].sur);
		putchar('#');
		putchar(abajo);
		putchar('#');
	}
	putchar('\n');
}

repr_lab* generarLaberinto (int ancho, int alto,
		int entradax, int entraday,
		int salidax, int saliday,
		int pasos)
	{

	lab* l = laberintoVacio(ancho, alto);
	repr_lab* rep = reprVacio(ancho, alto);

	l->celdas[entraday][entradax].visitada = 1;
	rep->celdas[entraday][entradax].cen = VACIO;

	continuarLaberinto(l, rep, entradax, entraday, pasos, 1);

	marcarSalida(rep, ENTRADA, entradax, entraday);
	marcarSalida(rep, SALIDA, salidax, saliday);

	borrarLaberinto(l);

	return rep;
}

void continuarLaberinto (
		lab* l, repr_lab* rep,
		int posx, int posy,
		int nro_pasos, int celdas_visitadas)
	{

	const int total_celdas = l->ancho * l->alto;
	celda* celda_actual = &l->celdas[posy][posx];
	int paso = 0;
	int pasos_alcanzados = paso >= nro_pasos;

	while (celdas_visitadas < total_celdas) {
		// Si quedan direcciones, se elige una
		// Si no, se vuelve atras
		while (celda_actual->dir_actual < 4) {
			puts("----------------------");
			printf("X:%i Y:%i Celdas visitadas: %i\n",
					posx, posy,
					celdas_visitadas);

			dir nueva_dir = celda_actual->dirs[celda_actual->dir_actual];
			int nuevo_y = calcY(posy, nueva_dir);
			int nuevo_x = calcX(posx, nueva_dir);
			celda_actual->dir_actual++;

			printf("Nueva dir: %c, Nuevo X: %i, Nuevo Y: %i\n", 
					dirtostr(nueva_dir), nuevo_x, nuevo_y);
			printf("Numero de direccion: %i\n", celda_actual->dir_actual);
			printf("Numero de paso: %i\n", paso);

			// Si la siguiente celda no esta ocupada, se avanza
			// De lo contrario, se intenta con otra direccion
			if (	nuevo_y < l->alto && nuevo_y >= 0 &&
				nuevo_x < l->ancho && nuevo_x >= 0 &&
				l->celdas[nuevo_y][nuevo_x].visitada == 0	)
			{
				celda *celda_siguiente = &l->celdas[nuevo_y][nuevo_x];

				// Se hace la conexion con la celda actual
				puts("Siguiente celda disponible!");	
				celda_siguiente->conexion = diropuesta(nueva_dir);
				
				// Se rompen las paredes correspondientes
				romperPared(&rep->celdas[posy][posx],
						nueva_dir,
						VACIO);
				romperPared(&rep->celdas[nuevo_y][nuevo_x],
						diropuesta(nueva_dir),
						VACIO);
				romperPared(&rep->celdas[nuevo_y][nuevo_x],
						X,
						VACIO);

				posy = nuevo_y; posx = nuevo_x;
				puts("Pasando a siguiente celda...");

				// Se incrementa en uno el paso si la celda
				// no habia sido visitada. Ademas se la marca como
				// visitada.
				if (!celda_siguiente->visitada) {
					paso++;
					celda_siguiente->visitada = 1;
					pasos_alcanzados = paso >= nro_pasos;
				}

				// Marcamos la celda siguiente como actual
				celda_actual = celda_siguiente;
				// Se incrementa el total de celdas visitadas
				celdas_visitadas++;

				// Si se realizaron los pasos especificados, se
				// termina la funcion.
				if (pasos_alcanzados && nro_pasos > 0) {
					puts("Numero de pasos alcanzados, retornando..");
					return;
				}

			} else {
				puts("Siguiente celda no disponible...\n");
			}
		}
		puts("Direcciones agotadas, retrocediendo...");
		// No hay mas direcciones, hay que retroceder
		posy = calcY(posy, celda_actual->conexion);
		posx = calcX(posx, celda_actual->conexion);
		celda_actual = &l->celdas[posy][posx];
	}
	puts("Todas las celdas visitadas, retornando...");
}

void borrarRepr(repr_lab* rl) {
	for (int fila = 0; fila < rl->alto; fila++) {
		free(rl->celdas[fila]);	
	}
	free(rl->celdas);
	free(rl);
}

/* Implementacion de funciones internas */

// Constructores y destructores

void inicializarCelda(celda* c) {
	c->dirs = malloc(sizeof(dir)*4);
	c->visitada = 0; c->dir_actual = 0;
	c->conexion = X; obtenerDirs(c->dirs);
}

void borrarCelda(celda* c) {
	free(c->dirs);	
}

repr_lab* reprVacio(int ancho, int alto) {
	repr_lab* rl = malloc(sizeof(repr_lab));
	rl->celdas = malloc(sizeof(repr_celda*) * alto);
	for(int fila = 0; fila < alto; fila++) {
		rl->celdas[fila] = malloc(sizeof(repr_celda) * ancho);
		for(int col=0; col < ancho; col++) {
			rl->celdas[fila][col].nor = rl->celdas[fila][col].sur =
			rl->celdas[fila][col].est = rl->celdas[fila][col].oes =
			rl->celdas[fila][col].cen = LLENO;
		}
	}

	rl->alto = alto; rl->ancho = ancho;

	return rl;
}



lab* laberintoVacio(int ancho, int alto) {
	lab* nuevo = malloc(sizeof(lab));
	nuevo->ancho = ancho; nuevo->alto = alto;
	nuevo->celdas = malloc(sizeof(celda*) * alto);
	for (int fila = 0; fila < alto; fila++) {
		nuevo->celdas[fila] = malloc(sizeof(celda) * ancho);
		for (int col = 0; col < ancho; col++) {
			inicializarCelda(&nuevo->celdas[fila][col]);
		}
	}

	return nuevo;
}

void borrarLaberinto(lab *l) {
	for(int fila = 0; fila < l-> alto; fila++) {
		for(int col = 0; col < l-> ancho; col++)
			borrarCelda(&l->celdas[fila][col]);
		free(l->celdas[fila]);
	}
	free(l->celdas);
	free(l);
}

// Funciones auxiliares
int calcX(int x, dir d) {
	if(d == E) return x + 1;
	if(d == O) return x - 1;
	return x;
}
int calcY(int y, dir d) {
	if(d == S) return y + 1;
	if(d == N) return y - 1;
	return y;
}
char dirtostr(dir d) {
	switch (d) {
		case N: return 'N';
		case S: return 'S';
		case O: return 'O';
		case E: return 'E';
		case X: return 'X';
	}
}
char diropuesta(dir d) {
	switch (d) {
		case N: return S;
		case S: return N;
		case E: return O;
		case O: return E;
		case X: return d;
	}
}
void romperPared(repr_celda *c, dir d, sym s) {
	switch(d) {
		case N: c->nor = s; break;
		case S: c->sur = s; break; 
		case E: c->est = s; break;
		case O: c->oes = s; break;
		case X: c->cen = s; break;
	}
}
char eligeSimbolo(sym s) {
	switch(s) {
		case VACIO:	return ' ';
		case LLENO: 	return '#';
		case ENTRADA: 	return 'E';
		case SALIDA: 	return 'S';
		case JUGADOR: 	return '@';
	}
}

void obtenerDirs(dir* dir_arr) {
	dir direcciones[4] = { N, S, E, O };
       	int r;	// Aqui se guardan los numeros aleatorios
	for (int i = 0; i < 4; i++) {
		// Se obtiene un indice aleatorio que va de 0 a (4 - i)
		r = rand() % (4 - i);
		// Se asigna una direccion aleatoria de la lista de direcciones
		dir_arr[i] = direcciones[r];
		// Se intercambia la obtenida por la ultima
		dir aux = direcciones[3-i];
		direcciones[3-i] = direcciones[r];
		direcciones[r] = aux;
	}
}

void marcarSalida(repr_lab* rep, sym s, int x, int y) {
	// Si se encuentra en un borde, se rompe la pared
	// Si se encuentra en una esquina, se rompe preferentemente
	// las paredes verticales
	// Si no es ninguna de los anteriores, se marca el centro como entrada
	if (y == 0)			romperPared(&rep->celdas[y][x], N, s);
	else if (y == rep->alto-1)	romperPared(&rep->celdas[y][x], S, s);	
	else if (x == 0)		romperPared(&rep->celdas[y][x], O, s); 
	else if (x == rep->ancho-1)	romperPared(&rep->celdas[y][x], E, s);
	else 				romperPared(&rep->celdas[y][x], X, s);
}
