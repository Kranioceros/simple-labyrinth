#ifndef MAZEGEN
#define MAZEGEN

typedef enum {LLENO, VACIO, ENTRADA, SALIDA, JUGADOR} sym;

typedef struct repr_celda {
	sym nor, sur, est, oes, cen;		
} repr_celda;

typedef struct repr_lab {
	repr_celda** celdas;
	int ancho, alto;
} repr_lab;

repr_lab* generarLaberinto(
		int ancho, int alto, 
		int entradax, int entraday,
		int salidax, int saliday,
		int pasos);

void borrarRepr(repr_lab*);

char* dibujoLaberinto(repr_lab* l);
void dibujarLaberinto(repr_lab* l);

#endif
