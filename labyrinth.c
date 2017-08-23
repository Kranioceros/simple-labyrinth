#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef enum {N=0, S, E, O, X} dir; // X: Ninguna direccion
typedef enum {LLENO, VACIO, ENTRADA, SALIDA} sym;

typedef struct celda {
	int visitada;
	dir conexion; // Indica direccion desde la cual la celda fue visitada por primera vez
	dir* dirs;    // Arreglo de 4 direcciones aleatorias
	int dir_actual; // Indica numero de direcciones empleadas del arreglo anterior
} celda;

typedef struct lab {
	// Datos generales del laberinto
	int alto, ancho;
	int entradax, salidax; // Coordenadas de la entrada y salida
	int entraday, saliday;
	celda** celdas; // Arreglo bidimensional de celdas que conforma el laberinto
} lab;

/* Esta es una estructura auxiliar que facilita la impresion en pantalla del laberinto */
typedef struct repr_celda {
	sym nor, sur, est, oes, cen;		
} repr_celda;

// POR HACER
/* Una estructura de lista enlazada para ir recorriendo el laberinto */
typedef struct nodo {
	struct nodo* conexiones[4];	
	int visitado;
} nodo;


// PROTOTIPOS //

/* Funciones fundamentales que operan con laberintos */
// Genera una tabla bidimensional de repr_celda que va a conformar
// la representacion del laberinto
repr_celda** reprVacio(int, int); 
// Genera un laberinto vacio
lab* laberintoVacio(int, int);
// Funciones que desalojan de memoria las estructuras anteriores
void borrarLaberinto(lab*);
void borrarRepr(repr_celda**, int, int);
// Algoritmo principal que trabaja sobre un laberinto y su representacion. Toma como parametros
// las coordenadas de la posicion inicial
void excavarLaberinto(lab*, repr_celda**, int, int);
// Encuentra la salida y la marca (backrecursive algorithm)
void marcarSolucion(repr_celda**, int, int);

/* Funciones sencillas */
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
		case E: return 'E';
		case O: return 'O';
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
		case VACIO: return ' ';
		case LLENO: return '#';
		case ENTRADA: return 'E';
		case SALIDA: return 'S';
	}
}



// Llena un puntero a dir* con direcciones aleatorias.
void obtenerDirs(dir*);
// Funcion que imprime el laberinto. Toma un arreglo bidimensional de repr_celda como
// parametro; ademas del alto y ancho del laberinto.
void labtostr(repr_celda**, int, int);
// Marca la salida/entrada en la representacion del laberinto (repr_celda**)
void marcarSalida(repr_celda** rep, sym s, int x, int y, int ancho, int alto);

/////// MAIN ///////

int main(int argc, char* argv[]) {
	if(argc == 5) {
		// Se utiliza como semilla para rand() el tiempo actual del sistema
		srand(time(NULL));

		int alto, ancho;
		int posx, posy;

		ancho = atoi(argv[1]); alto = atoi(argv[2]);
		posx = atoi(argv[3]); posy = atoi(argv[4]);	

		lab *x = laberintoVacio(ancho, alto);
		repr_celda** rep = reprVacio(ancho, alto);	

		excavarLaberinto(x, rep, 0, 0);

		// Marcamos la entrada del laberinto
		marcarSalida(rep, ENTRADA, posx, posy, ancho, alto);
		// Marcamos la salida del laberinto
		marcarSalida(rep, SALIDA, ancho-1, alto-1, ancho, alto);

		labtostr(rep, ancho, alto);
		borrarLaberinto(x);
		borrarRepr(rep, ancho, alto);


	} else {
		puts("Numero invalido de argumentos.");
		puts("uso: labyrinth <ancho> <alto> <posx> <posy>");
	}
	return 0;
}

void celdaVacia(celda* c) {
	c->dirs = malloc(sizeof(dir)*4);
	c->visitada = 0; c->dir_actual = 0;
	c->conexion = X; obtenerDirs(c->dirs);
}

// POR HACER
void nodoVacio(nodo* n) {
	n = malloc(sizeof(nodo));
	// Se inicializan todas las conexiones en NULL
	for(int i=0; i < 4; i++) {
		n->conexiones[i] = NULL;
	}
}

void borrarRepr(repr_celda** r, int ancho, int alto) {
	for (int fila=0; fila < alto; fila++)
		free(r[fila]);
	free(r);
}

repr_celda** reprVacio(int ancho, int alto) {
	repr_celda** rep = malloc(sizeof(repr_celda*) * alto);
	for(int fila = 0; fila < alto; fila++) {
		rep[fila] = malloc(sizeof(repr_celda) * ancho);
		for(int col=0; col < 5; col++) {
			rep[fila][col].nor = rep[fila][col].sur =
			rep[fila][col].est = rep[fila][col].oes =
			rep[fila][col].cen = LLENO;
		}
	}

	return rep;
}

lab* laberintoVacio(int ancho, int alto) {
	lab* nuevo = malloc(sizeof(lab));
	nuevo->ancho = ancho; nuevo->alto = alto;
	nuevo->celdas = malloc(sizeof(celda*) * alto);
	for (int fila = 0; fila < alto; fila++) {
		nuevo->celdas[fila] = malloc(sizeof(celda) * ancho);
		for (int col = 0; col < ancho; col++) {
			celdaVacia(&nuevo->celdas[fila][col]);
		}
	}

	return nuevo;
}
void borrarCelda(celda* c) {
	free(c->dirs);	
}

void borrarLaberinto(lab *l) {
	for(int fila = 0; fila < l-> alto; fila++) {
		for(int col = 0; col < l-> ancho; col++)
			borrarCelda(&l->celdas[fila][col]);
		free(l->celdas[fila]);
	}
	free(l);
}

void excavarLaberinto(lab* l, repr_celda** rep, int x, int y) {
	const int total_celdas = l->alto * l->ancho;

	int celdas_visitadas = 1;
	int distancia_total = 0;

	celda* celda_actual = &l->celdas[y][x];
	romperPared(&rep[y][x], X, VACIO);
	celda_actual->visitada = 1;

	while (celdas_visitadas < total_celdas) {
		// Si quedan direcciones, se elige una
		// Si no, se vuelve atras
		while (celda_actual->dir_actual < 4) {
			puts("----------------------");
			printf("X:%i Y:%i Celdas visitadas: %i\n", x, y, celdas_visitadas);

			dir nueva_dir = celda_actual->dirs[celda_actual->dir_actual];
			int nuevo_y = calcY(y, nueva_dir);
			int nuevo_x = calcX(x, nueva_dir);
			celda_actual->dir_actual++;

			printf("Nueva dir: %c, Nuevo X: %i, Nuevo Y: %i\n", 
					dirtostr(nueva_dir), nuevo_x, nuevo_y);
			printf("Numero de direccion: %i\n", celda_actual->dir_actual);

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
				romperPared(&rep[y][x], nueva_dir, VACIO);
				romperPared(&rep[nuevo_y][nuevo_x], diropuesta(nueva_dir), VACIO);
				romperPared(&rep[nuevo_y][nuevo_x], X, VACIO);

				y = nuevo_y; x = nuevo_x;
				puts("Pasando a siguiente celda...");
				// Marcamos la celda siguiente como visitada
				celda_siguiente->visitada = 1;
				// Marcamos la celda siguiente como actual
				celda_actual = celda_siguiente;
				// Se incremente el total de celdas visitadas
				celdas_visitadas++;
			} else {
			puts("Siguiente celda no disponible...\n");}
		}
		puts("Direcciones agotadas, retrocediendo...");
		// No hay mas direcciones, hay que retroceder
		y = calcY(y, celda_actual->conexion);
		x = calcX(x, celda_actual->conexion);
		celda_actual = &l->celdas[y][x];
	}
	puts("Todas las celdas han sido visitadas. Laberinto excavado exitosamente!");
	puts("----------------------");
}

void obtenerDirs(dir* dir_arr) {
	dir direcciones[4] = { N, S, E, O };
       	int r;		// Aqui se guardan los numeros aleatorios
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

void labtostr(repr_celda** rep, int ancho, int alto) {
	char* resultado;
	char centro, arriba, abajo, derecha, izquierda;
	for (int fila=0; fila < alto; fila++) {
		for (int col=0; col < ancho; col++) {
			//Muros de arriba
			arriba = eligeSimbolo(rep[fila][col].nor);
			printf("#%c#", arriba);
		}
		puts("");
		for (int col=0; col < ancho; col++) {
			//Muros del medio
			izquierda = eligeSimbolo(rep[fila][col].oes);
			derecha = eligeSimbolo(rep[fila][col].est);
			centro = eligeSimbolo(rep[fila][col].cen);
			printf("%c%c%c", izquierda, centro, derecha);
		}
		puts("");
	}

	for (int col=0; col < ancho; col++) {
		abajo = eligeSimbolo(rep[alto-1][col].sur);
		printf("#%c#", abajo);
	}
	puts("");
}

void marcarSalida(repr_celda** rep, sym s, int x, int y, int ancho, int alto) {
	// Si se encuentra en un borde, se rompe la pared
	// Si se encuentra en una esquina, se rompe preferentemente las paredes verticales
	// Si no es ninguna de los anteriores, se marca el centro como entrada
	int salida_marcada = 0;
	if (y == 0)		romperPared(&rep[y][x], N, s);
	else if (y == alto-1)	romperPared(&rep[y][x], S, s);	
	else if (x == 0)	romperPared(&rep[y][x], O, s); 
	else if (x == ancho-1)	romperPared(&rep[y][x], E, s);
	else 			romperPared(&rep[y][x], X, s);
}

// POR HACER
void marcarSolucion(repr_celda** l, int x, int y) {
	dir* direcciones; obtenerDirs(direcciones);
}
