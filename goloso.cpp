#include "goloso.h"

Camino MinimoAbsolutoSegunPeso(Grafo& grafo, const Vertice inicio, const Vertice destino, const double K, double (Arista::*Peso)() const) {
	Camino res = grafo.HallarCaminoMinimoEntre(inicio, destino, Peso);
	if (res.PesoTotalEnW1() <= K)
		return res;
	else
		return Camino();
}

Camino Goloso(Grafo& grafo, const Vertice inicio, const Vertice destino, const double K) {
	Camino res = MinimoAbsolutoSegunPeso(grafo, inicio, destino, K, &Arista::PesoW2);
	if (res.Longitud() > 0)
		return res;
	
	res = MinimoAbsolutoSegunPeso(grafo, inicio, destino, K, &Arista::PesoW1);
	return res;
}

Camino GolosoMultiple(Grafo& grafo, const Vertice inicio, const Vertice destino, const double K) {
	Camino caminoMinimo = MinimoAbsolutoSegunPeso(grafo, inicio, destino, K, &Arista::PesoW2);
	if (caminoMinimo.Longitud() > 0)
		return caminoMinimo;        //Devuelvo porque es el mínimo absoluto y está acotado.
	
	caminoMinimo = MinimoAbsolutoSegunPeso(grafo, inicio, destino, K, &Arista::PesoW1);
	if (caminoMinimo.Longitud() > 0)
		DijkstraDeTodosLosPuntos(grafo, inicio, destino, K, caminoMinimo);
	
	return caminoMinimo;
}

void DijkstraDeTodosLosPuntos(Grafo& grafo, const Vertice inicio, const Vertice destino, const double K, Camino& caminoMinimo) 
{
	double pesoW2Minimo = caminoMinimo.PesoTotalEnW2();
	Camino caminoPrincipio;
	Camino caminoFin;
	double pesoW1;
	double pesoW2;
	unsigned int vertices = grafo.CantidadDeVertices();

	for (unsigned int i=1; i <= vertices; i++){
	
		// Dijkstra Minimizando W1
		caminoPrincipio = grafo.HallarCaminoMinimoEntre(inicio, i, &Arista::PesoW1);
		caminoFin = grafo.HallarCaminoMinimoEntre(i, destino, &Arista::PesoW1);
		caminoPrincipio.UnirSinCiclos(caminoFin);
		pesoW1 = caminoPrincipio.PesoTotalEnW1();
		pesoW2 = caminoPrincipio.PesoTotalEnW2();
		if (pesoW1 <= K && pesoW2 < pesoW2Minimo){
			pesoW2Minimo = pesoW2;
			caminoMinimo = caminoPrincipio;
		}
		
		// Dijkstra Minimizando W2
		caminoPrincipio = grafo.HallarCaminoMinimoEntre(inicio, i, &Arista::PesoW2);
		caminoFin = grafo.HallarCaminoMinimoEntre(i, destino, &Arista::PesoW2);
		caminoPrincipio.UnirSinCiclos(caminoFin);
		pesoW1 = caminoPrincipio.PesoTotalEnW1();
		pesoW2 = caminoPrincipio.PesoTotalEnW2();
		if (pesoW1 <= K && pesoW2 < pesoW2Minimo){
			pesoW2Minimo = pesoW2;
			caminoMinimo = caminoPrincipio;
		}
	}
}
