/* 	Copyright 2017, Ramiro Garay
 
	This program is free software: you can redistribute it and/or modify
    	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

enum direccion { N=0, S, E, O };
enum simbolo { VACIO, LLENO, SALIDA, ENTRADA };

struct celda {
	bool visitada = false;
	simbolo muros[4] = { LLENO, LLENO, LLENO, LLENO };
	simbolo interior = { LLENO };
};

struct laberinto {
	int alto, ancho;
	int entrada_y, entrada_x;
	int salida_y, salida_x;
	int distancia=0;
	celda** tabla;
	
	// Aloja en memoria la tabla para el laberinto, aparte de definir
	// el alto y el ancho del mismo.
	laberinto(int=10,int=10,int=0,int=0);
	// Romper los muros correspondientes a la entrada y salida del laberinto
	void romperBordes();
};

//////////// PROTOTIPOS //////////// 

// Devuelve en orden aleatorio un arreglo con las cuatro direcciones
std::vector<direccion> obtenerDirecciones ();
// Devuelve string con direccion
std::string strDir (direccion);
// Devuelve el laberinto en formato ASCII
std::string asciiLaberinto (laberinto&);
// Genera el laberinto usando "backtrack-recursive algorithm". Por defecto genera un laberinto cerrado.
void backRecursive (laberinto&, int=0, int=0, int=0);
// Devuelven las coordenadas x e y a partir de la posicion original y la direccion
int calcX (int, direccion);
int calcY (int, direccion);
// Devuelve la direccion opuesta a la dada
direccion opuesto (direccion);
// Devuelve caracter correspondiente al simbolo
std::string grafico (simbolo);

//////////// MAIN ////////////

int main (int argc, char* argv[]) {
	// Valor semilla para rand()
	std::srand(std::time(NULL));

	if ( argc == 5 ) {
		int alto = std::stoi(argv[1]), ancho = std::stoi(argv[2]);
		int y = std::stoi(argv[3]), x = std::stoi(argv[4]); 
		laberinto lab(alto, ancho, y, x);
		backRecursive(lab, y, x, 0);
		lab.romperBordes();
		std::cout << asciiLaberinto(lab) << std::endl;
	} else {
		std::cout << "Error: Se requieren 4 parametros\n\tmakelab <alto> <ancho> <posy> <posx>\n\nEste programa genera un laberinto del ancho y alto especificado. Ademas se asegura que la entrada al mismo se encuentre en las coordenadas especificadas.\nLa salida la coloca en la celda cuya distancia sea la maxima posible con respecto a la entrada" << std::endl;

	}

	return 0;
}

//////////// FUNCIONES //////////// 

laberinto::laberinto(int al, int an, int en_y, int en_x) {
	alto = al; ancho = an;
	salida_y = entrada_y = en_y; salida_x = entrada_x = en_x;

	tabla = new celda*[alto];
	for (int i = 0; i < alto; i++)
		tabla[i] = new celda[ancho];
}

void laberinto::romperBordes () {
	if (entrada_y == 0)           tabla[entrada_y][entrada_x].muros[N] = ENTRADA;
	else if (entrada_y == alto-1) tabla[entrada_y][entrada_x].muros[S] = ENTRADA;
	else if (entrada_x == 0)      tabla[entrada_y][entrada_x].muros[O] = ENTRADA;
	else if (entrada_x == ancho-1)tabla[entrada_y][entrada_x].muros[E] = ENTRADA;
	else tabla[entrada_y][entrada_x].interior = ENTRADA;

	if (salida_y == 0)           tabla[salida_y][salida_x].muros[N] = SALIDA;
	else if (salida_y == alto-1) tabla[salida_y][salida_x].muros[S] = SALIDA;
	else if (salida_x == 0)      tabla[salida_y][salida_x].muros[O] = SALIDA;
	else if (salida_x == ancho-1)tabla[salida_y][salida_x].muros[E] = SALIDA;
	else tabla[salida_y][salida_x].interior = SALIDA;
}

std::vector<direccion> obtenerDirecciones () {
	std::vector<direccion> direcciones;
	std::vector<direccion> disponibles = {N, S, E, O};
	int aleatorio;

	for (int i = 0; i < 4; i++) {
		aleatorio = rand() % disponibles.size();
		direcciones.push_back(disponibles[aleatorio]); // Direccion aleatoria
		disponibles.erase(disponibles.begin() + aleatorio); // Borramos la direccion de disponibles
	}
	
	return direcciones;
}

std::string strDir (direccion dir) {
	switch (dir) {
		case N: return "N";
		case S: return "S";
		case E: return "E";
		case O: return "O";
	}	
}

std::string asciiLaberinto (laberinto &lab) {
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
}

void backRecursive (laberinto &lab, int y, int x, int distancia) {
	// Se marca la celda como visitada
	lab.tabla[y][x].visitada = true;
	// Se vacia el interior
	lab.tabla[y][x].interior = VACIO;
	// Se obtienen 4 direcciones aleatorias
	// Y por cada una de ellas se intenta seguir un camino
	for ( direccion dir : obtenerDirecciones() ) {
		int nuevo_y = calcY(y, dir), nuevo_x = calcX(x, dir);
		//std::cout << "X: " << x << ", Y: " << y << ", Dir: " << strDir(dir) << std::endl;
		//std::cout << "nuevo_X: " << nuevo_x << ", nuevo_Y: " << nuevo_y << "\n-------------------------------" << std::endl;
		
		// Si la celda siguiente está dentro de la tabla Y no está visitada, entonces romper los muros
		// y llamar a la función de nuevo pero con las coordenadas nuevas.
		if ( nuevo_y < lab.alto && nuevo_y > -1 && nuevo_x < lab.ancho && nuevo_x > -1 &&
		!lab.tabla[calcY(y,dir)][calcX(x,dir)].visitada ) {
			//std::cout << "Cumple las condiciones!\n";
			lab.tabla[y][x].muros[dir] = VACIO; 
			lab.tabla[nuevo_y][nuevo_x].muros[opuesto(dir)] = VACIO;
			backRecursive (lab, nuevo_y, nuevo_x, distancia+1);
		}
		
	} 	

	// Si la celda esta en un borde y la distancia con respecto a la entrada
	// es mayor a la de la salida actual, convertirla en salida
	if (distancia > lab.distancia && (y == 0 || x == 0 || y == lab.alto-1 || x == lab.ancho-1)) {
		lab.salida_y = y; lab.salida_x = x;
		lab.distancia = distancia;
	}
}

int calcX (int x, direccion dir) {
	if (dir == E) return x + 1;
	else if (dir == O) return x - 1;
	else return x;
}

int calcY (int y, direccion dir) {
	if (dir == S) return y + 1;
	else if (dir == N) return y - 1;
	else return y;
}

direccion opuesto (direccion dir) {
	switch (dir) {
		case N: return S;
		case S: return N;
		case E: return O;
		case O: return E;
	}
}

std::string grafico (simbolo sim) {
	switch (sim) {
		case LLENO: return "##";
		case VACIO: return "  ";
		case ENTRADA: return "EE";
		case SALIDA: return "SS";
	}
}
