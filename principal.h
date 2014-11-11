#include "exacto.h"
#include "goloso.h"
#include "grasp.h"
#include <fstream>
#include <algorithm>

void CargarEntrada(Grafo& grafo, Vertice& u, Vertice& v, double& K);
void ResolverNormalmente(string& algoritmoACorrer, Grafo& grafo, Vertice u, Vertice v, double K);
void ResolverYTomarMedicionesDe(string& nombreDeArchivo, string& algoritmoACorrer, Grafo& grafo, Vertice u, Vertice v, double K);
timespec diff(timespec start, timespec end);
void ImprimirCamino(Camino& caminoMinimo);
