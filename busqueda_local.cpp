#include "busqueda_local.h"


Camino BusquedaLocal(Grafo& grafo, Vertice u, Vertice v, double K, Camino caminoInicial)
{	
	//Esto va a pasar cuando corra la busqueda local sola. Probablemente cuando corra dentro de GRASP, caminoInicial no va a ser vacio.
	if (caminoInicial.Longitud() == 0) 
		caminoInicial = GenerarSolucionInicial(grafo, u, v, K); //O(mlogn)

	IterCamino it = caminoInicial.CrearIterador();	//Al inicio del camino.

	while (!it.Final()) {	//O(n³ + nmlogn)

		Vertice x = it.AristaActual().ObtenerExtremos().first; //obtengo el vertice izquierdo de la arista.
		Camino mejorCaminoMinimoEnVecindad = HallarMejorCaminoEnVecindadDeX(grafo, caminoInicial, x, it, K);

		if (mejorCaminoMinimoEnVecindad.Longitud() != 0)
			MejorarCaminoInicial(caminoInicial, mejorCaminoMinimoEnVecindad, it, x);
		else 
			it.Avanzar();

	}	

	return caminoInicial;

}



Camino GenerarSolucionInicial(Grafo& grafo, Vertice u, Vertice v, double K)
{
	Camino caminoInicial = grafo.HallarCaminoMinimoEntre(u, v, &Arista::PesoW2);

	if (caminoInicial.PesoTotalEnW1() <= K) {

		return caminoInicial;	//Es el optimo

	}

	caminoInicial = grafo.HallarCaminoMinimoEntre(u, v, &Arista::PesoW1);

	if (caminoInicial.PesoTotalEnW1() <= K)
		return caminoInicial;
	else
		return Camino();
}

//O(mlogn + n²)
Camino HallarMejorCaminoEnVecindadDeX(Grafo& grafo, const Camino& caminoInicial, const Vertice x, IterCamino& iter1, const double K)
{
	Camino mejorCaminoMinimoEnVecindad;
	IterCamino iter2 = iter1;
	vector<Vertice> arbolDeDistancias = grafo.HallarCaminosMinimosDesde(x, &Arista::PesoW2); //desde x al resto del grafo, en particular a todos los vertices del camino.
	//O(mlogn)
	while (!iter2.Final()) { //O(n²)

		Vertice y = iter2.AristaActual().ObtenerExtremos().second; //obtengo el vertice derecho de la arista.
		Camino caminoMinimoEntreXeY = grafo.ArmarCaminoEntre(x, y, arbolDeDistancias);
		double pesoW1DeLasAristasEntreXeY = caminoInicial.ObtenerPesoDeLasAristasEntre(x, y, &Arista::PesoW1);
		double pesoW2DeLasAristasEntreXeY = caminoInicial.ObtenerPesoDeLasAristasEntre(x, y, &Arista::PesoW2);
		//Chequeo que me quede un camino factible.
		if (caminoInicial.PesoTotalEnW1() + caminoMinimoEntreXeY.PesoTotalEnW1() - pesoW1DeLasAristasEntreXeY <= K
			&& caminoMinimoEntreXeY.PesoTotalEnW2() < pesoW2DeLasAristasEntreXeY)
			ObtenerElCaminoMinimoEntre(caminoMinimoEntreXeY, mejorCaminoMinimoEnVecindad);

		iter2.Avanzar();

	}

	return mejorCaminoMinimoEnVecindad;
}

//O(n)
void MejorarCaminoInicial(Camino& caminoInicial, Camino& mejorCaminoMinimoEnVecindad, IterCamino& iter1, const Vertice x)
{				
	IterCamino iterFinalCaminoMinimo = mejorCaminoMinimoEnVecindad.CrearIteradorAlFinal();
	Vertice finalCaminoMinimo = iterFinalCaminoMinimo.AristaActual().ObtenerExtremos().second;
	iter1.AvanzarHasta(finalCaminoMinimo);
	caminoInicial.EliminarAristasEntre(x, finalCaminoMinimo);
	caminoInicial.UnirCon(mejorCaminoMinimoEnVecindad);
}
