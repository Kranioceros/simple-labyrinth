#include <stdio.h>
#include "../headers/mazegenerator.h"
#include <time.h>
#include <stdlib.h>

int main() {
	srand(time(NULL));
	repr_lab* l = generarLaberinto(5, 5, 0, 0, 4, 4, -2);

	puts(dibujoLaberinto(l));

	return 0;
}
