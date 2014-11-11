#include "exacto.h"

Camino Exacto(Grafo& grafo, Vertice inicio, Vertice destino, double K)
{		
	Camino camino(grafo);
	Camino caminoMinimo(grafo);

	HallarCaminoMinimoAcotadoPorK(grafo, inicio, destino, K, camino, caminoMinimo); /*minimo respecto de W2*/

	return caminoMinimo;
}



void HallarCaminoMinimoAcotadoPorK(Grafo& grafo, Vertice verticeActual, Vertice verticeDestino, double K, Camino& camino, Camino& caminoMinimo)
{	
	grafo.Visitar(verticeActual);

	for (Arista e : grafo.AristasIncidentesA(verticeActual)) {

		Vertice vecino = e.ObtenerExtremos().second;	// e = <verticeActual, vecino>
				
		if (camino.PesoTotalEnW1() + e.PesoW1() < K && !grafo.FueVisitado(vecino)) {
			
			if (vecino != verticeDestino) {
				
				camino.AgregarArista(e);
				HallarCaminoMinimoAcotadoPorK(grafo, vecino, verticeDestino, K, camino, caminoMinimo);
				camino.RemoverArista(e);
				
			} else {
				
				camino.AgregarArista(e);
				ObtenerElCaminoMinimoEntre(camino, caminoMinimo);
				camino.RemoverArista(e);

			}
		
		}
			
	}

	grafo.Desvisitar(verticeActual);
}



void ObtenerElCaminoMinimoEntre(Camino& camino, Camino& caminoMinimo)
{
	if (caminoMinimo.Longitud() == 0) {

		caminoMinimo = camino;
	
	} else if (camino.PesoTotalEnW2() < caminoMinimo.PesoTotalEnW2()) {

		caminoMinimo = camino;

	}
}
