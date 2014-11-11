#include "grafo.h"

#ifndef CAMINO_H_
#define CAMINO_H_
typedef list<Arista> Camino;
#endif

Camino MinimoAbsolutoSegunPeso(Grafo&, const Vertice, const Vertice, const double, double (Arista::*)() const);
Camino Goloso(Grafo&, const Vertice, const Vertice, const double);
Camino GolosoMultiple(Grafo&, const Vertice, const Vertice, const double);
void DijkstraDeTodosLosPuntos(Grafo&, const Vertice, const Vertice, const double, Camino&);
