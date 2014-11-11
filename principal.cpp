#include "principal.h"

/*
 Primer parametro: algoritmo a correr
 Segundo parametro: 1 para tomar mediciones. 0 para correr normalmente.
*/
int main(int argc, char* argv[]) 
{
	string nombreDeArchivo = "mediciones de ";
	nombreDeArchivo += argv[0]+2;
	string algoritmoACorrer(argv[1]);
	string mediciones(argv[2]);
	bool hayQueTomarMediciones;
	
	if (mediciones == "1") {

		hayQueTomarMediciones = true;

	} else if (mediciones == "0") {

		hayQueTomarMediciones = false;

	} else {

		cerr << "Segundo parametro incorrecto" << endl;
		exit(1);

	}
	
	transform(algoritmoACorrer.begin(), algoritmoACorrer.end(), algoritmoACorrer.begin(), ::tolower); //paso todo a minusculas.
	
	while(cin.peek() != '0') {

		Grafo grafo;
		Vertice u;
		Vertice v;
		double K;
		
		CargarEntrada(grafo, u, v, K); //Deja el stdin apuntando a la próxima instancia.
		
		if (hayQueTomarMediciones)
			ResolverYTomarMedicionesDe(nombreDeArchivo, algoritmoACorrer, grafo, u, v, K);
		else
			ResolverNormalmente(algoritmoACorrer, grafo, u, v, K);

	}
}



void ResolverNormalmente(string& algoritmoACorrer, Grafo& grafo, Vertice u, Vertice v, double K) 
{
	Camino camino;
	
	if (algoritmoACorrer == "exacto")	{
		
		camino = Exacto(grafo, u, v, K);

	} else if (algoritmoACorrer == "goloso") {
		
		camino = Goloso(grafo, u, v, K);
		
	} else if (algoritmoACorrer == "local") {
		
		camino = BusquedaLocal(grafo, u, v, K);
		
	} else if (algoritmoACorrer == "grasp") {
		
		std::random_device rd;
		camino = Grasp(grafo, u, v, K, rd());

	} else {
		
		cerr << "Parametros incorrectos" << endl;
		exit(1);
		
	}
	
	ImprimirCamino(camino);
}


void ResolverYTomarMedicionesDe(string& nombreDeArchivo, string& algoritmoACorrer, Grafo& grafo, Vertice u, Vertice v, double K) 
{	
	ofstream mediciones;
	Camino camino;
	timespec startT, endT; //Para calcular el tiempo
	long nanosegundos = 0;
	time_t segundos = 0;
	unsigned int cantMediciones = 20;
	std::random_device rd;
	unsigned int semilla = rd();
	
	mediciones.open(nombreDeArchivo, ofstream::app);

	for (unsigned int medicion = 1; medicion <= cantMediciones; medicion++) {
	
		if (algoritmoACorrer == "exacto")	{

			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startT); //comienzo a medir tiempo
			camino = Exacto(grafo, u, v, K);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endT);	//termino de medir

		} else if (algoritmoACorrer == "goloso") {

			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startT); //comienzo a medir tiempo
			camino = Goloso(grafo, u, v, K);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endT); //termino de medir

		} else if (algoritmoACorrer == "local") {

			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startT); //comienzo a medir tiempo
			camino = BusquedaLocal(grafo, u, v, K);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endT); //termino de medir

		} else if (algoritmoACorrer == "grasp") {

			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startT); //comienzo a medir tiempo
			camino = Grasp(grafo, u, v, K, semilla);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endT); //termino de medir

		}  else if (algoritmoACorrer == "graspinf") {

			mediciones << grafo.CantidadDeVertices() << " " << grafo.CantidadDeAristas() << endl;
			camino = GraspInfinito(grafo, u, v, K, semilla, mediciones);
			mediciones.close();
			ImprimirCamino(camino);
			return; //Por si llega a terminar por algún motivo.

		} else {

			cerr << "Parametros incorrectos" << endl;
			exit(1);

		}
		
		timespec estaMedicion = diff(startT,endT);

		if (nanosegundos == 0)
			nanosegundos = estaMedicion.tv_nsec;
		else
			nanosegundos = min(estaMedicion.tv_nsec, nanosegundos);

		if (segundos == 0)
			segundos = estaMedicion.tv_sec;
		else
			segundos = min(estaMedicion.tv_sec, segundos);
	}
	
	ImprimirCamino(camino);

	mediciones << grafo.CantidadDeVertices() << " " << camino.PesoTotalEnW2() << " " 
			   << segundos << " " << nanosegundos << endl;
	mediciones.close();
}


void CargarEntrada(Grafo& grafo, Vertice& u, Vertice& v, double& K) 
{
	unsigned int cantVertices;
	unsigned int cantAristas;
	unsigned int numVerticeInicial;
	unsigned int numVerticeFinal;

	cin >> cantVertices >> cantAristas >> numVerticeInicial >> numVerticeFinal >> K;
	assert(cantVertices > 0 && "cantidad de vertices incorrecta");
	assert(cantAristas >= 0 && "cantidad de aristas incorrecta");
	assert(numVerticeInicial > 0 && numVerticeFinal > 0 && numVerticeFinal <= cantVertices && numVerticeInicial <= cantVertices && "vertices u,v incorrectos");
	assert(K > 0 && "K incorrecto");
	
	grafo = Grafo(cantVertices);
	u = Vertice(numVerticeInicial);
	v = Vertice(numVerticeFinal);
	
	for (unsigned int i = 1; i <= cantAristas; i++) {

		unsigned int v1;
		unsigned int v2;
		double peso1;
		double peso2;
		
		cin >> v1 >> v2 >> peso1 >> peso2;
		assert(0 < v1 && v1 <= cantVertices && 0 < v2 && v2 <= cantVertices && "aristas incorrectas");
		assert(peso1 >= 0 && peso2 >= 0 && "pesos incorrectos");
		
		Arista e = Arista(Vertice(v1), Vertice(v2), peso1, peso2);
		grafo.AgregarArista(e);

	}

	cin.ignore(1); //Ignora el newline a continuación, dejando como siguiente caracter el primer dato de la próxima instancia.
}	





timespec diff(timespec start, timespec end) 
{
    timespec temp;

    if ((end.tv_nsec-start.tv_nsec)<0) {

        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;

    } else {

        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
        
    }

    return temp;
}




void ImprimirCamino(Camino& caminoMinimo) 
{
	if (caminoMinimo.Longitud() != 0) {

		cout << caminoMinimo.PesoTotalEnW1() << " " << caminoMinimo.PesoTotalEnW2() << " " << caminoMinimo.Longitud() << " ";
		caminoMinimo.Mostrar();
		cout << endl;

	} else {

		caminoMinimo.Mostrar();

	}
}
