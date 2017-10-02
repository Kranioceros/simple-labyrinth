#define NORTE 0
#define SUR 1
#define ESTE 2
#define OESTE 3

typedef struct nodo {
	// 0: No visitado 1: Visitado
	int visitado;	
	// 0: Entrada 1: Salida -1: Ninguno
	int extremo;
	// Se inicializan en NULL
	struct nodo* caminos[4];
	// Se inicializa en NULL
	struct nodo* anterior;
} nodo;

nodo* crearNodo();
nodo* agregarNodo(nodo* n, int dir);
nodo* moverse(nodo* n, int dir); // Si no hay nodo en la direccion, devuelve el mismo

void borrarGrafo(nodo* n);
