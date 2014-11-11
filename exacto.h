#include "grafo.h"

#ifndef CAMINO_H_
#define CAMINO_H_
typedef list<Arista> Camino;
#endif

Camino Exacto(Grafo& grafo, Vertice u, Vertice v, double K);
void HallarCaminoMinimoAcotadoPorK(Grafo& grafo, Vertice verticeActual, Vertice verticeDestino, double K, Camino& camino, Camino& caminoMinimo);
void ObtenerElCaminoMinimoEntre(Camino& camino, Camino& caminoMinimo);
		
