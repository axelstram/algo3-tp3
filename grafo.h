#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <assert.h>

using namespace std;


#ifndef VERTICE_H_
#define VERTICE_H_
typedef unsigned int Vertice;
#endif 


#ifndef ARISTA_H_
#define ARISTA_H_
class Arista {
	
	private:
		
		pair<Vertice, Vertice> extremos;
		double peso1;
		double peso2;
		
	public:
		
		Arista() {};
		Arista(Vertice v, Vertice u, double peso1, double peso2);
		double PesoW1() const;
		double PesoW2() const;
		pair<Vertice, Vertice> ObtenerExtremos() const;
		bool operator==(const Arista& arista) const;
		bool operator!=(const Arista& arista) const;
		void Swap();
		void Mostrar() const;

};
#endif



#ifndef GRAFO_H_
#define GRAFO_H_
class Camino;
typedef pair<Vertice, double> VerticeYDistancia;

class Grafo {

	private:

		class Comparacion {
			public:
				bool operator()(const VerticeYDistancia& p1, const VerticeYDistancia& p2) const { return p1.second > p2.second; };
		};

		vector<list<Arista>> grafo;
		unsigned int cantVertices;
		unsigned int cantAristas;
		vector<bool> visitado;
		
		void Inicializar(vector<double>& distancias, vector<unsigned int>& predecesores, vector<VerticeYDistancia>& Q, const Vertice u) const;
		void CambiarPrioridad(vector<VerticeYDistancia>& Q, const Vertice w, double nuevaDistancia) const;
		VerticeYDistancia ObtenerMinimo(vector<VerticeYDistancia>& Q) const;
		vector<Vertice> Dijkstra(const Vertice u, double (Arista::*Peso)() const);

	public:
		
		Grafo() {};
		Grafo(unsigned int n);
		void AgregarArista(Arista e);
		const list<Arista>& AristasIncidentesA(const Vertice v) const;
		unsigned int CantidadDeVertices() const;
		unsigned int CantidadDeAristas() const;
		void Visitar(Vertice v);
		bool FueVisitado(Vertice v) const;
		void Desvisitar(Vertice v);
		Arista HallarArista(Vertice w, Vertice v) const;
		Camino HallarCaminoMinimoEntre(const Vertice u, const Vertice v, double (Arista::*Peso)() const);
		vector<Vertice> HallarCaminosMinimosDesde(const Vertice u, double (Arista::*Peso)() const);
		Camino ArmarCaminoEntre(const Vertice u, const Vertice v, const vector<Vertice>& predecesores) const;
		void Mostrar() const;

};
#endif


#ifndef ITER_CAMINO_H
#define ITER_CAMINO_H
class IterCamino {
	
	private:
	
		Camino& camino;
		Vertice ultimoVertice;
		Arista aristaActual;
		bool final;
		IterCamino(Camino& c);
		IterCamino(Camino& c, Arista e);
		
	public:
		
		bool Final() const;
		Arista AristaActual() const;
		void Avanzar();
		void AvanzarHasta(const Vertice final);
		IterCamino operator=(IterCamino& otroIterador);

		friend class Camino;
};
#endif


#ifndef CAMINO_H_
#define CAMINO_H_
class Camino {

	friend class IterCamino;

	private:

		vector<list<Arista>> adyacencias;
		unsigned int longitud;
		Vertice verticeInicial;
		Arista ultimaArista;
		Vertice ultimoVertice;
		double pesoTotalW1;
		double pesoTotalW2;

	public:

		Camino();
		~Camino() {};
		Camino(const Grafo& grafo);
		void AgregarArista(Arista arista);
		void RemoverArista(Arista arista);
		unsigned int Longitud() const;
		double PesoTotalEnW1() const;
		double PesoTotalEnW2() const;
		void UnirCon(const Camino& otroCamino);
		void UnirSinCiclos(const Camino& otroCamino);
		void EliminarAristasEntre(const Vertice verticeInicial, const Vertice verticeFinal);
		double ObtenerPesoDeLasAristasEntre(const Vertice x, const Vertice y, double (Arista::*Peso)() const) const;
		IterCamino CrearIterador();
		IterCamino CrearIteradorEn(Arista e);
		IterCamino CrearIteradorAlFinal();
		void Mostrar() const;
		bool enCamino(const Vertice) const;
		
};
#endif
