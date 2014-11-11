#include <fstream>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include "../grafo.h"

using namespace std;

int main() {
	srand(time(NULL));
	ofstream testFile;
	unsigned int verticesMin;
	unsigned int verticesMax;
	unsigned int porcentajeAristas;
	cout << "inserte la cantidad de vertices minima:";
	cin>> verticesMin;
	cout << "inserte la cantidad de vertices maxima:"; 
	cin>> verticesMax;
	cout << "inserte la chance de que una arista este en el grafo (entero de 0 a 100):"; 
	cin>> porcentajeAristas;

	testFile.open("tests_random");
	#define PesoMax 100
	
	for (unsigned int vertices = verticesMin; vertices <= verticesMax; vertices++) {
		//Genero todas las aristas primero, para poder saber cuantas hay
		list<Arista> listaDeAristas;

		for (unsigned int i = 1; i <= vertices; i++) {
			for (unsigned int j = i+1; j <= vertices; j++) {
				unsigned int chanceArista = (rand() % 100);
				bool existeArista = (chanceArista < porcentajeAristas);
				if (existeArista){
					unsigned int peso1 = 1;
					unsigned int peso2 = (rand() % PesoMax)+2;
					Arista arista(i, j, peso1, peso2);
					listaDeAristas.push_back(arista);
				}
			}
		}
		unsigned int cantAristas = listaDeAristas.size();
		unsigned int u = (rand() % vertices)+1;
		unsigned int v = (rand() % vertices)+1;
		while (u == v)
			v = (rand() % vertices)+1;
		unsigned int k = ((rand() % vertices)+1)*((rand() % (PesoMax-1))+1);
		testFile << vertices+2 << " " << cantAristas+4 << " " << u << " " << v << " " << k << endl;
		list<Arista>::iterator it = listaDeAristas.begin();

		for (unsigned int i=0; i < cantAristas; i++){
			Vertice v1 = it->ObtenerExtremos().first;
			Vertice v2 = it->ObtenerExtremos().second;
			double w1 = it->PesoW1();
			double w2 = it->PesoW2();
			testFile << v1 << " " << v2 << " " << w1 << " " << w2 << endl;
			it++;
		}

		unsigned int verticeNuevo = vertices+1;
		testFile << u << " " << verticeNuevo << " " << PesoMax+10 << " " << 0 << endl;
		testFile << verticeNuevo << " " << v << " " << PesoMax+10 << " " << 0 << endl;

		verticeNuevo++;
		testFile << u << " " << verticeNuevo << " " << k-2 << " " << 1 << endl;
		testFile << verticeNuevo << " " << v << " " << 1 << " " << 0 << endl;

	}
	
	testFile << 0;
	testFile.close();
}
	
