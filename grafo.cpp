#include "grafo.h"



/**************************************************************************************************************************/

Arista::Arista(Vertice v, Vertice u, double peso1, double peso2) 
{
	this->extremos.first = v;
	this->extremos.second = u;
	this->peso1 = peso1;
	this->peso2 = peso2;
}


double Arista::PesoW1() const 
{
	return peso1;
}


double Arista::PesoW2() const 
{
	return peso2;
}


pair<Vertice, Vertice> Arista::ObtenerExtremos() const 
{
	return extremos;
}


bool Arista::operator==(const Arista& arista) const 
{
	return (this->extremos == arista.extremos && this->peso1 == arista.peso1 && this->peso2 == arista.peso2);
}


bool Arista::operator!=(const Arista& arista) const
{
	return !(*this == arista);
}


void Arista::Mostrar() const 
{
	cout << "<" << extremos.first << ", " << extremos.second << "> ";
}


void Arista::Swap() 
{
	Vertice aux = extremos.first;
	extremos.first = extremos.second;
	extremos.second = aux;
}
/**************************************************************************************************************************/


Grafo::Grafo(unsigned int n) : grafo(n+1, list<Arista>()), visitado(n+1, false) 
{
	cantVertices = n;
	cantAristas = 0;
}


void Grafo::AgregarArista(Arista arista) 
{
	pair<Vertice, Vertice> extremos = arista.ObtenerExtremos();
	Vertice v = extremos.first;
	Vertice w = extremos.second;

	assert(0 < v && v <= cantVertices && 0 < w && w <= cantVertices && "aristas invalidas");

	grafo[v].push_back(arista);
	arista.Swap();
	grafo[w].push_back(arista);
	cantAristas++;
}


const list<Arista>& Grafo::AristasIncidentesA(const Vertice v) const 
{
	return grafo[v];
}


unsigned int Grafo::CantidadDeVertices() const {
	return cantVertices;
}


unsigned int Grafo::CantidadDeAristas() const 
{
	return cantAristas;
}


void Grafo::Visitar(Vertice v) 
{
	visitado[v] = true;
}
		

bool Grafo::FueVisitado(Vertice v) const 
{
	return visitado[v];
}
		

void Grafo::Desvisitar(Vertice v) 
{
	visitado[v] = false;
}



vector<Vertice> Grafo::HallarCaminosMinimosDesde(const Vertice u, double (Arista::*Peso)() const)
{
	return Dijkstra(u, Peso);
}



Camino Grafo::HallarCaminoMinimoEntre(const Vertice u, const Vertice v, double (Arista::*Peso)() const)
{
	vector<Vertice> predecesores = Dijkstra(u, Peso);

	return ArmarCaminoEntre(u, v, predecesores);
}


vector<Vertice> Grafo::Dijkstra(const Vertice u, double (Arista::*Peso)() const)
{
	vector<double> distancias(cantVertices+1);
	vector<unsigned int> predecesores(cantVertices+1);
	vector<VerticeYDistancia> Q;

	Inicializar(distancias, predecesores, Q, u);
	make_heap(Q.begin(), Q.end(), Comparacion());

	while (!Q.empty()) {

		VerticeYDistancia par = ObtenerMinimo(Q);
		const Vertice verticeActual = par.first;
		double distanciaAlVerticeActual = par.second;
		Visitar(verticeActual);

		for (auto arista : grafo[verticeActual]) {

			const Vertice vecino = arista.ObtenerExtremos().second;

			if (!FueVisitado(vecino)) {

				double distanciaAlVecino = distancias[vecino];

				if (distanciaAlVerticeActual + (arista.*Peso)() < distanciaAlVecino) {

					distancias[vecino] = distanciaAlVerticeActual + (arista.*Peso)();
					predecesores[vecino] = verticeActual;
					CambiarPrioridad(Q, vecino, distanciaAlVerticeActual + (arista.*Peso)());

				}

			}

		}

	}

	for (unsigned int i = 0; i < visitado.size(); i++)
		visitado[i] = false;

	return predecesores;
}


VerticeYDistancia Grafo::ObtenerMinimo(vector<VerticeYDistancia>& Q) const 
{
	pop_heap(Q.begin(), Q.end(), Comparacion());
	VerticeYDistancia res = Q.back();
	Q.pop_back();

	return res;
}


void Grafo::CambiarPrioridad(vector<VerticeYDistancia>& Q, const Vertice w, double nuevaDistancia) const 
{
	for (auto& par : Q) {

		const Vertice x = par.first;

		if (w == x)
			par.second = nuevaDistancia;

	}

	make_heap(Q.begin(), Q.end(), Comparacion());
}


void Grafo::Inicializar(vector<double>& distancias, vector<unsigned int>& predecesores, vector<VerticeYDistancia>& Q, const Vertice u) const 
{
	for (unsigned int i = 1; i < distancias.size(); i++) {

		if (i == u) {

			distancias[i] = 0;

		} else {

			distancias[i] = numeric_limits<double>::infinity();

		}

		predecesores[i] = 0;

		Q.push_back(make_pair(Vertice(i), distancias[i]));

	}
}


Camino Grafo::ArmarCaminoEntre(const Vertice u, const Vertice v, const vector<Vertice>& predecesores) const 
{
	Camino camino(*this);
	list<Arista> aristas;

	Vertice w = predecesores[v];
	Vertice actual = v;

	if (w == 0 || u == 0 || v == 0)
		return Camino();	//No existe camino, devuelvo camino vacio.

	while (actual != u) {

		Arista e = HallarArista(w,actual);
		aristas.push_front(e);
		actual = w;
		w = predecesores[w];

	}

	for (auto e : aristas)
		camino.AgregarArista(e);

	return camino;
}



Arista Grafo::HallarArista(Vertice w, Vertice v) const 
{
	Arista res;
	
	for (auto arista : grafo[w]) {

		if (arista.ObtenerExtremos().first == w && arista.ObtenerExtremos().second == v)
			res = arista;

	}
	
	return res;
}



void Grafo::Mostrar() const 
{
	int numVertice = 0;

	for (auto listaAristas : grafo) {

		if (numVertice == 0) {

			numVertice++;
			continue;

		}

		cout << "v" << numVertice << ": ";

		for (auto arista : listaAristas)
			arista.Mostrar();

		cout << endl;
		numVertice++;

	}
}


/**************************************************************************************************************************/

Camino::Camino() : adyacencias(0, list<Arista>()) 
{ 
	longitud = 0;
	pesoTotalW1 = 0;
	pesoTotalW2 = 0;
}



Camino::Camino(const Grafo& G) : adyacencias(G.CantidadDeVertices()+1, list<Arista>()) 
{
	longitud = 0;
	pesoTotalW1 = 0;
	pesoTotalW2 = 0;
}


void Camino::AgregarArista(Arista arista) 
{
	pair<Vertice, Vertice> extremos = arista.ObtenerExtremos();
	Vertice v = extremos.first;
	Vertice w = extremos.second;
	
	assert(0 < v && v <= adyacencias.size() && 0 < w && w <= adyacencias.size() && "aristas invalidas");
	
	if (longitud == 0)
		verticeInicial = v;
		
	ultimaArista = arista;
	ultimoVertice = w;
		
	adyacencias[v].push_back(arista);
	longitud++;
	pesoTotalW1 += arista.PesoW1();
	pesoTotalW2 += arista.PesoW2();
}


void Camino::RemoverArista(Arista arista) 
{	
	pair<Vertice, Vertice> extremos = arista.ObtenerExtremos();
	Vertice v = extremos.first;
	Vertice w = extremos.second;

	assert(0 < v && v <= adyacencias.size() && 0 < w && w <= adyacencias.size() && "aristas invalidas");

	adyacencias[v].remove(arista);
	longitud--;
	pesoTotalW1 -= arista.PesoW1();
	pesoTotalW2 -= arista.PesoW2();
}


unsigned int Camino::Longitud() const 
{
	return longitud;
}


double Camino::PesoTotalEnW1() const 
{
	return pesoTotalW1;
}


double Camino::PesoTotalEnW2() const 
{
	return pesoTotalW2;
}


void Camino::UnirCon(const Camino& otroCamino) 
{
	Vertice v = otroCamino.verticeInicial;
	Arista temp = ultimaArista;
	
	for (unsigned int i = 1; i <= otroCamino.longitud; i++) {
		
		Arista e = otroCamino.adyacencias[v].front();
		AgregarArista(e);
		v = e.ObtenerExtremos().second;
		
	}
	
	if (otroCamino.ultimaArista.ObtenerExtremos().second == this->verticeInicial) {
	
		this->verticeInicial = otroCamino.verticeInicial;
		this->ultimaArista = temp;
	
	} else {
	
		ultimoVertice = otroCamino.ultimoVertice;
	
	}
}

//pre: destino es parte del camino unido.
void Camino::UnirSinCiclos(const Camino& otroCamino)
{
	UnirCon(otroCamino);

	Camino caminoSinCiclos;
	caminoSinCiclos.adyacencias = vector<list<Arista> >(otroCamino.adyacencias.size());
	caminoSinCiclos.verticeInicial = verticeInicial;

	Vertice v = caminoSinCiclos.verticeInicial;
	bool llegueAlFinal = longitud == 0; //Sólo vale si es un camino vacío.
	
	while (!llegueAlFinal) {
		Arista e = adyacencias[v].back();
		caminoSinCiclos.AgregarArista(e);
		v = e.ObtenerExtremos().second;
		llegueAlFinal = (v == ultimoVertice);
	}

	*this = caminoSinCiclos;
}

void Camino::EliminarAristasEntre(const Vertice inicio, const Vertice final)
{
	Vertice verticeActual = inicio;

	while (verticeActual != final) {

		Arista e = adyacencias[verticeActual].front();
		Vertice proximoVertice = e.ObtenerExtremos().second;

		if (verticeActual == verticeInicial)
			verticeInicial = proximoVertice;

		verticeActual = proximoVertice;
		RemoverArista(e);

	}
}


double Camino::ObtenerPesoDeLasAristasEntre(const Vertice x, const Vertice y, double (Arista::*Peso)() const) const
{
	Vertice v = x;
	double peso = 0;

	while (v != y) {

		Arista e = adyacencias[v].front();
		v = e.ObtenerExtremos().second;
		peso += (e.*Peso)();

	}

	return peso;
}


IterCamino Camino::CrearIterador() 
{
	return IterCamino(*this);
}


IterCamino Camino::CrearIteradorEn(Arista e) 
{
	return IterCamino(*this, e);
}


IterCamino Camino::CrearIteradorAlFinal()
{
	return IterCamino(*this, ultimaArista);
}

void Camino::Mostrar() const 
{	
	Vertice v = verticeInicial;
	
	if (longitud == 0) {

		cout << "no" << endl;
		return;
		
	}
	
	cout << v << " ";

	for (unsigned int i = 0; i < longitud; i++) {
		
		Arista arista = adyacencias[v].front();
		Vertice u = arista.ObtenerExtremos().second;

		cout << u << " ";
		v = u;
		
	}
}


bool Camino::enCamino(const Vertice v) const
{	
	return (adyacencias[v].size() > 0);
}

/**************************************************************************************************************************/

IterCamino::IterCamino(Camino& c) : camino(c) 
{	
	ultimoVertice = c.ultimoVertice;
	final = (c.Longitud() == 0);
	if (!final)
		aristaActual = c.adyacencias[c.verticeInicial].front(); //primera arista del camino
}


IterCamino::IterCamino(Camino& c, Arista e) : camino(c) 
{	
	ultimoVertice = c.adyacencias.size();
	aristaActual = e;
	final = false;
}


bool IterCamino::Final() const 
{
	return final;
}


Arista IterCamino::AristaActual() const 
{
	assert(!final && "Acceso invalido en iterador de camino.");
	return aristaActual;
}

//pre: no es el final.
void IterCamino::Avanzar() 
{
	Vertice verticeSiguiente = aristaActual.ObtenerExtremos().second;
	if (verticeSiguiente != ultimoVertice)
		aristaActual = camino.adyacencias[verticeSiguiente].front();
	else final = true;
}


void IterCamino::AvanzarHasta(const Vertice final)
{
	while (aristaActual.ObtenerExtremos().first != final) {

		if (!Final())
			Avanzar();
		else
			return;
		
	}
}


IterCamino IterCamino::operator=(IterCamino& otroIterador) 
{
	this->camino = otroIterador.camino;
	this->aristaActual = otroIterador.aristaActual;
	
	return *this;
}

