#include <stdio.h>
#include "../headers/mazegenerator.h"
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc < 7) {
		puts("Uso: mazegen <ancho> <alto> <posx> <posy> <pasos> <semilla>");
		return 0;
	}
	int ancho, alto, entradax, entraday, pasos, semilla;

	sscanf(argv[1], "%d", &ancho);
	sscanf(argv[2], "%d", &alto);
	sscanf(argv[3], "%d", &entradax);
	sscanf(argv[4], "%d", &entraday);
	sscanf(argv[5], "%d", &pasos);
	sscanf(argv[6], "%d", &semilla);

	if (semilla < 0) semilla = time(NULL);

	srand(semilla);

	printf("Generando laberinto con las siguientes caracteristicas:\
			\tAncho: %i\
			\tAlto: %i\
			\tEntrada: (%i, %i)\
			\tPasos: %i\n",
			ancho, alto,
			entradax, entraday,
			pasos);

	repr_lab* l = generarLaberinto(ancho, alto,
			entradax, entraday,
			ancho-1, alto-1,
			pasos);

	dibujarLaberinto(l);
	borrarRepr(l);

	return 0;
}
