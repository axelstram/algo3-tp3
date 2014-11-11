#include "grasp.h" 



Camino Grasp(Grafo& grafo, const Vertice u, const Vertice v, const double K, const unsigned int semilla, const unsigned int cotaIteraciones) {
	//Inicializo los vectores con -1 para representar distancias infinitas.
	vector<double> tablaDistanciasW1(grafo.CantidadDeVertices()+1, -1);
	vector<double> tablaDistanciasW2(grafo.CantidadDeVertices()+1, -1);
	InicializarCaminosMinimos(grafo, v, tablaDistanciasW1, tablaDistanciasW2);

	if (NoHayCamino(u, tablaDistanciasW1, K))
		return Camino();

	//Inicializo el mersenne engine
	std::mt19937 gen(semilla);
	//while (no mejoro en tantas iteraciones)
	Camino mejorCamino = CaminoRandomGoloso(grafo, u, v, K, tablaDistanciasW1, tablaDistanciasW2, gen);
	mejorCamino = BusquedaLocal(grafo, u, v, K, mejorCamino);
	unsigned int iteracionesSinCambios = 0;
	while (iteracionesSinCambios < cotaIteraciones) {
		Camino caminoActual = CaminoRandomGoloso(grafo, u, v, K, tablaDistanciasW1, tablaDistanciasW2, gen);
		caminoActual = BusquedaLocal(grafo, u, v, K, caminoActual);
		if (mejorCamino.PesoTotalEnW2() > caminoActual.PesoTotalEnW2()) {
			mejorCamino = caminoActual;
			iteracionesSinCambios = 0;
		}
		else iteracionesSinCambios++;
	}
	return mejorCamino;
}

Camino GraspInfinito(Grafo& grafo, const Vertice u, const Vertice v, const double K, const unsigned int semilla, std::ostream& os) {
	vector<double> tablaDistanciasW1(grafo.CantidadDeVertices()+1, std::numeric_limits<double>::infinity());
	vector<double> tablaDistanciasW2(grafo.CantidadDeVertices()+1, std::numeric_limits<double>::infinity());
	InicializarCaminosMinimos(grafo, v, tablaDistanciasW1, tablaDistanciasW2);

	if (NoHayCamino(u, tablaDistanciasW1, K))
		return Camino();

	//Inicializo el mersenne engine
	std::mt19937 gen(semilla);
	//while (no mejoro en tantas iteraciones)
	Camino mejorCamino = CaminoRandomGoloso(grafo, u, v, K, tablaDistanciasW1, tablaDistanciasW2, gen);
	mejorCamino = BusquedaLocal(grafo, u, v, K, mejorCamino);
	unsigned int iteracionesSinCambios = 0, iteraciones = 0;
	while (true) {
		os << iteraciones << " " << iteracionesSinCambios << " " << mejorCamino.PesoTotalEnW2() << endl;
		Camino caminoActual = CaminoRandomGoloso(grafo, u, v, K, tablaDistanciasW1, tablaDistanciasW2, gen);
		caminoActual = BusquedaLocal(grafo, u, v, K, caminoActual);
		if (mejorCamino.PesoTotalEnW2() > caminoActual.PesoTotalEnW2()) {
			mejorCamino = caminoActual;
			iteracionesSinCambios = 0;
		}
		else iteracionesSinCambios++;
		iteraciones++;
	}
	return mejorCamino; //En principio nunca va a llegar acá.
}

// generador de caminos aleatorios greedy
Camino CaminoRandomGoloso(Grafo& grafo, const Vertice inicio, const Vertice destino, const double K, const vector<double>& tablaDistanciasW1, const vector<double>& tablaDistanciasW2, std::mt19937& gen) {
	Camino caminoRandom(grafo);
	Vertice verticeActual = inicio;

	//Con esto decidimos cuánto restringimos la lista de opciones.
	#define Fraccion_de_opciones 3
	double kRestante = K;
	while (verticeActual != destino) {
		list<Arista> aristas = FiltrarInfactibles(grafo.AristasIncidentesA(verticeActual), verticeActual, kRestante, caminoRandom, tablaDistanciasW1);
		unsigned int cantOpciones = aristas.size();
		if (cantOpciones == 0) {
			//extiendo el camino
			Camino extension = grafo.HallarCaminoMinimoEntre(verticeActual, destino, &Arista::PesoW1);
			caminoRandom.UnirSinCiclos(extension);
			return caminoRandom; //Si no hay un camino entre los vertices, devuelve vacío.
		}

		vector<AristaYPotencial> mejoresAristas = Ordenar(aristas, tablaDistanciasW2, verticeActual);
		
		unsigned int maxOpciones = grafo.CantidadDeVertices() / Fraccion_de_opciones;
		if (cantOpciones > maxOpciones) { // este numero puede depender de la cantidad de nodos del grafo, o ser constante
			//mejoresAristas.resize(maxOpciones);
			cantOpciones = maxOpciones;
		}
    	std::uniform_int_distribution<> dis(0, cantOpciones-1);
		unsigned int numeroDeArista = dis(gen);
		Arista& aristaElegida = mejoresAristas[numeroDeArista].first;
		caminoRandom.AgregarArista(aristaElegida);
		verticeActual = VerticeDeDestino(aristaElegida, verticeActual);
		kRestante = kRestante - aristaElegida.PesoW1();
	}
	return caminoRandom;
}

bool NoHayCamino(const Vertice v, const vector<double>& tablaDistancias, const double& kRestante) {
	assert(tablaDistancias.size() > v && "Vertice invalido en NoHayCamino");
	if (tablaDistancias[v] == std::numeric_limits<double>::infinity()) return true;
	else return (tablaDistancias[v] > kRestante);
}

void InicializarCaminosMinimos(Grafo& grafo, const Vertice destino, vector<double>& tablaDistanciasW1, vector<double>& tablaDistanciasW2) {
	//tablaDistancias tiene que tener en la posición i el valor de peso total del camino mínimo para el vértice i al destino.
	vector<Vertice> caminosW1 = grafo.HallarCaminosMinimosDesde(destino, &Arista::PesoW1);
	vector<Vertice> caminosW2 = grafo.HallarCaminosMinimosDesde(destino, &Arista::PesoW2);
	for (unsigned int i = 1; i <= grafo.CantidadDeVertices(); i++) {
		Camino miCaminoW1 = grafo.ArmarCaminoEntre(destino, i, caminosW1);
		Camino miCaminoW2 = grafo.ArmarCaminoEntre(destino, i, caminosW2);
		if (miCaminoW1.Longitud() != 0) { //Si no existe camino, le dejo un valor que representa infinito (-1).
			tablaDistanciasW1[i] = miCaminoW1.PesoTotalEnW1();
			tablaDistanciasW2[i] = miCaminoW2.PesoTotalEnW2();
		}
	}
	//Queda un caso especial: el nodo de destino. Le asigno 0 en ambas tablas para guardar coherencia.
	tablaDistanciasW1[destino] = 0;
	tablaDistanciasW2[destino] = 0;
}

list<Arista> FiltrarInfactibles(const list<Arista>& aristas, const Vertice verticeActual, const double kRestante, const Camino& miCamino, const vector<double>& tablaDistanciasW1) {	
	list<Arista> res;
	for (list<Arista>::const_iterator it = aristas.begin(); it != aristas.end(); it++) {
		double pesoOpcion = it->PesoW1();
		Vertice verticeDeOpcion = VerticeDeDestino(*it, verticeActual);
		bool opcionNoValida = miCamino.enCamino(verticeDeOpcion) || NoHayCamino(verticeDeOpcion, tablaDistanciasW1, kRestante-pesoOpcion);
		if (!opcionNoValida)
			res.push_back(*it);
	}
	return res;
}

//pre: el vértice pasado por argumento es un vértice de la arista.
Vertice VerticeDeDestino(const Arista& arista, const Vertice verticeDePartida) {
	pair<Vertice, Vertice> par = arista.ObtenerExtremos();
	if (par.first == verticeDePartida)
		return par.second;
	else return par.first;
}

vector<AristaYPotencial> Ordenar(const list<Arista>& aristas, const vector<double>& tablaDistancias, const Vertice verticeActual) {
	vector<AristaYPotencial> res(aristas.size());
	unsigned int i = 0;
	for (list<Arista>::const_iterator it = aristas.begin(); it != aristas.end(); it++, i++) {
		AristaYPotencial par;
		par.first = *it;
		Vertice verticeDeOpcion = VerticeDeDestino(*it, verticeActual);
		par.second = tablaDistancias[verticeDeOpcion];
		res[i] = par;
	}
	sort(res.begin(), res.end(), CompararPotenciales);
	return res;
}

bool CompararPotenciales(const AristaYPotencial& a1, const AristaYPotencial& a2) {
	return a1.second < a2.second;
}
