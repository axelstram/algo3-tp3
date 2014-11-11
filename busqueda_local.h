#include "grafo.h"
#include "exacto.h"

Camino BusquedaLocal(Grafo& grafo, Vertice u, Vertice v, double K, Camino caminoInicial = Camino());
Camino GenerarSolucionInicial(Grafo& grafo, Vertice u, Vertice v, double K);
Camino HallarMejorCaminoEnVecindadDeX(Grafo& grafo, const Camino& caminoInicial, const Vertice x, IterCamino& iter1, const double K);
void MejorarCaminoInicial(Camino& caminoInicial, Camino& caminoMinimoEntreTodosLosVecinos, IterCamino& iter1, const Vertice x);