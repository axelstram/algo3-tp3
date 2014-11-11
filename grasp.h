#include "grafo.h"
#include "busqueda_local.h"

#ifndef ARISTA_Y_POTENCIAL
#define ARISTA_Y_POTENCIAL
typedef pair<Arista, double> AristaYPotencial;
#endif

Camino Grasp(Grafo& grafo, const Vertice u, const Vertice v, const double K, const unsigned int semilla, const unsigned int cotaIteraciones = 100);
Camino GraspInfinito(Grafo&, const Vertice, const Vertice, const double, const unsigned int, std::ostream&);
Camino CaminoRandomGoloso(Grafo&, const Vertice, const Vertice, const double, const vector<double>&, const vector<double>&, std::mt19937&);
bool NoHayCamino(const Vertice, const vector<double>&, const double&);
void InicializarCaminosMinimos(Grafo&, const Vertice, vector<double>&, vector<double>&);
list<Arista> FiltrarInfactibles(const list<Arista>&, const Vertice, const double, const Camino&, const vector<double>&);
Vertice VerticeDeDestino(const Arista&, const Vertice);
vector<AristaYPotencial> Ordenar(const list<Arista>&, const vector<double>&, const Vertice);
bool CompararPotenciales(const AristaYPotencial&, const AristaYPotencial&);
