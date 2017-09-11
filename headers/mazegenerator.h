typedef enum {LLENO, VACIO, ENTRADA, SALIDA, JUGADOR} sym;

typedef struct repr_celda {
	sym nor, sur, est, oes, cen;		
} repr_celda;

repr_celda** generarLaberinto(int ancho, int alto);
repr_celda** generarLaberinto(int ancho, int alto, int entrada);
repr_celda** generarLaberinto(int ancho, int alto, int entrada, int salida);

char* dibujoLaberinto(repr_celda** l);
