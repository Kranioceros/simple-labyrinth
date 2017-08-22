#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef enum {N, S, E, O, X} dir; // X: Ninguna direccion

typedef struct {
	int visitada;
	dir conexion;
	dir* dirs;
	int dir_actual;
} celda;

typedef struct {
	// Datos generales del laberinto
	int alto, ancho;
	int entradax, salidax;
	int entraday, saliday;
	celda** celdas;
} lab;


// PROTOTIPOS //

/* Funciones fundamentales que operan con laberintos */
lab* laberintoVacio(int, int);
void borrarLaberinto(lab*);
void excavarLaberinto(lab*, int, int);

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

/* Llena un puntero a dir con direcciones aleatorias (usa time() y rand()) */
void obtenerDirs(dir*);
char* labtostr(lab*);

int main() {
	lab *x = laberintoVacio(10, 10);
	puts("laberinto creado");
	excavarLaberinto(x, 0, 0);
	char* ascii = labtostr(x);
	puts(ascii);
	return 0;
}

celda* celdaVacia() {
	celda* nueva; 	
	nueva = malloc(sizeof(celda));	
	nueva->visitada = 0; nueva->dir_actual = 0;
	nueva->conexion = X; obtenerDirs(nueva->dirs);

	return nueva;
}

lab* laberintoVacio(int ancho, int alto) {
	lab* nuevo = malloc(sizeof(lab));
	nuevo->ancho = ancho; nuevo->alto = alto;
	for (int fila = 0; fila < alto; fila++) {
		nuevo->celdas[fila] = malloc(sizeof(celda*) * ancho);
		for (int col = 0; col < ancho; col++) {
			nuevo->celdas[
		}
	}

	return nuevo;
}
void borrarCelda(celda* c) {
	free(c->dirs);	
	free(c);
}

void borrarLaberinto(lab *l) {
	for(int fila = 0; fila < l-> alto; fila++) {
		for(int col = 0; col < l-> ancho; col++) {
			borrarCelda(&l->celdas[fila][col]);
		}
		free(l->celdas[fila]);
	}
}

void excavarLaberinto(lab* l, int x, int y) {
	const int total_celdas = l->alto * l->ancho;

	int celdas_visitadas = 1;
	int distancia_total = 0;
	celda* celda_actual = &l->celdas[y][x];

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

				puts("Siguiente celda disponible!");	
				l->celdas[nuevo_y][nuevo_x].conexion = 
					diropuesta(nueva_dir);

				y = nuevo_y; x = nuevo_x;
				puts("Pasando a siguiente celda...");
				// Marcamos la celda siguiente como visitada
				celda_actual = &l->celdas[y][x]; celdas_visitadas++;
			} else {
			puts("Siguiente celda no disponible...\n");}
		}
		puts("Direcciones agotadas, retrocediendo...");
		celdas_visitadas = total_celdas;
		/*
		// No hay mas direcciones, hay que retroceder
		y = calcY(y, l->conexiones[y][x]);
		x = calcX(x, l->conexiones[y][x]);*/
	}
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

char* labtostr(lab* l) {
	char* resultado = malloc(9 * sizeof(char*) * l->alto * l->ancho + l->alto);

	for(int fila = 0; fila < l->alto; fila++) {
		for(int col = 0; col < l->ancho; col++) {
			resultado[3*fila*(l->ancho)+ fila + 3*col] = 
			resultado[3*fila*(l->ancho)+ fila + 3*col + 1] = 
			resultado[3*fila*(l->ancho)+ fila + 3*col + 2] = '#';
		}
		resultado[3*fila*(l->ancho)+ fila + 3*(l->ancho - 1) + 3] = '\n';
	}

	return resultado;
}

/*std::string asciiLaberinto (laberinto &lab) {
	std::string ascii_lab = "";
	
	for (int fila = 0; fila < lab.alto; fila++){
		// Muros superiores
		for (int cel = 0; cel < lab.ancho; cel++) {	
			ascii_lab.append("##");
			ascii_lab.append(grafico(lab.tabla[fila][cel].muros[N]));
			ascii_lab.append("##");
		}
		ascii_lab.append( "\n" );
		// Muros laterales e interior de las celdas
		for (int cel = 0; cel < lab.ancho; cel++) {	
			// Muro izquierdo
			ascii_lab.append(grafico(lab.tabla[fila][cel].muros[O]));
			// Interior de la celda
			ascii_lab.append(grafico(lab.tabla[fila][cel].interior));
			// Muro derecho
			ascii_lab.append(grafico(lab.tabla[fila][cel].muros[E]));
		}
		ascii_lab.append( "\n" );
		// Muros inferiores
		for (int cel = 0; cel < lab.ancho; cel++) {
			ascii_lab.append("##");
			ascii_lab.append(grafico(lab.tabla[fila][cel].muros[S]));
			ascii_lab.append("##");
		}

		if (fila < lab.alto - 1)
			ascii_lab.append( "\n" );
	}

	return ascii_lab;
}*/
