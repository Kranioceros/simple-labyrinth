#include <stdio.h>
#include "../headers/mazegenerator.h"
#include <time.h>
#include <stdlib.h>

int main() {
	srand(time(NULL));
	repr_lab* l = generarLaberinto(10, 10, 0, 0, 9, 9, -1);

	puts(dibujoLaberinto(l));

	return 0;
}
