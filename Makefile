CC=g++
CFLAGS=-c -Wall -Werror -pedantic -O2 -std=c++0x
#-ggdb
LDFLAGS=-lrt
SOURCES=principal.cpp busqueda_local.cpp grasp.cpp exacto.cpp grafo.cpp goloso.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=tp

all: heuristicas generador inversamente_propocionales pesos_W1_mitad_nulos pesos_W1_todos_nulos random_ciclos patologicos_local patologicos_goloso

heuristicas: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

ejemplos_random.o:
	$(CC) $(CFLAGS) Tests/ejemplos_random.cpp -o Tests/$@

ejemplos_inversamente_proporcionales.o:
	$(CC) $(CFLAGS) Tests/ejemplos_inversamente_proporcionales.cpp -o Tests/$@

ejemplos_pesos_W1_mitad_nulos.o:
	$(CC) $(CFLAGS) Tests/ejemplos_pesos_W1_mitad_nulos.cpp -o Tests/$@

ejemplos_pesos_W1_todos_nulos.o:
	$(CC) $(CFLAGS) Tests/ejemplos_pesos_W1_todos_nulos.cpp -o Tests/$@

ejemplos_random_ciclos.o:
	$(CC) $(CFLAGS) Tests/ejemplos_random_ciclos.cpp -o Tests/$@

ejemplos_patologicos_local.o:
	$(CC) $(CFLAGS) Tests/ejemplos_patologicos_local.cpp -o Tests/$@

ejemplos_patologicos_goloso.o:
	$(CC) $(CFLAGS) Tests/ejemplos_patologicos_goloso.cpp -o Tests/$@

generador: $(OBJECTS) ejemplos_random.o
	$(CC) Tests/ejemplos_random.o grafo.o -o Tests/$@

inversamente_propocionales: $(OBJECTS) ejemplos_inversamente_proporcionales.o
	$(CC) Tests/ejemplos_inversamente_proporcionales.o grafo.o -o Tests/$@

pesos_W1_mitad_nulos: $(OBJECTS) ejemplos_pesos_W1_mitad_nulos.o
	$(CC) Tests/ejemplos_pesos_W1_mitad_nulos.o grafo.o -o Tests/$@

pesos_W1_todos_nulos: $(OBJECTS) ejemplos_pesos_W1_todos_nulos.o
	$(CC) Tests/ejemplos_pesos_W1_todos_nulos.o grafo.o -o Tests/$@

random_ciclos: $(OBJECTS) ejemplos_random_ciclos.o
	$(CC) Tests/ejemplos_random_ciclos.o grafo.o -o Tests/$@
	
	
patologicos_local: $(OBJECTS) ejemplos_patologicos_local.o
	$(CC) Tests/ejemplos_patologicos_local.o grafo.o -o Tests/$@
	
patologicos_goloso: $(OBJECTS) ejemplos_patologicos_goloso.o
	$(CC) Tests/ejemplos_patologicos_goloso.o grafo.o -o Tests/$@

clean_heu:
	rm -f *.o
	rm -f $(EXECUTABLE)

clean_test:
	rm -f Tests/*.o
	rm -f Tests/generador

clean:
	rm -f *.o Tests/*.o
	rm -f $(EXECUTABLE) Tests/generador
	rm -f $(EXECUTABLE) Tests/inversamente_proporcionales
	rm -f $(EXECUTABLE) Tests/pesos_W1_mitad_nulos
	rm -f $(EXECUTABLE) Tests/pesos_W1_todos_nulos
	rm -f $(EXECUTABLE) Tests/random_ciclos
	rm -f $(EXECUTABLE) Tests/patologicos_local
	rm -f $(EXECUTABLE) Tests/patologicos_goloso
