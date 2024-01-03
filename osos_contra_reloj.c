#include "osos_contra_reloj.h"
#include "utiles.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_OBSTACULOS 600
#define MAX_HERRAMIENTAS 600
#define MAX_COORDENADAS 600
#define MAX_ELEMENTOS 600

#define CUADRADO "\u2588"
#define VACIO  ' '

#define RESETEAR_COLOR     "\e[0m"
#define LETRA_NEGRA        "\e[30m"
#define LETRA_ROJA		   "\e[31m"
#define LETRA_VERDE_OSCURO "\e[32m"
#define LETRA_AMARILLA	   "\e[33m"
#define LETRA_AZUL		   "\e[34m"
#define LETRA_CYAN		   "\e[35m"
#define FONDO_BLANCO       "\e[47m"
#define	LETRA_GRIS		   "\e[90m"
#define FONDO_VERDE_CLARO  "\e[102m"

#define ARBOL  'A'
#define PIEDRA 'R'
#define KOALA  'K'
 
#define LINTERNA  'L'
#define VELA      'V'
#define BENGALA   'E'
#define PILA      'B'

#define POLAR 'I'
#define PARDO 'G'
#define PANDA 'P'
#define CHLOE 'C'

const int CANTIDAD_INICIAL_VELAS          = 4;
const int CANTIDAD_INICIAL_VELAS_POLAR    = 6;
const int CANTIDAD_INICIAL_BENGALAS_PANDA = 2;

const int MOVIMIENTOS_LINTERNA       = 10;
const int MOVIMIENTOS_LINTERNA_PARDO = 15;
const int MOVIMIENTOS_PILA			 = 5;
const int MOVIMIENTOS_VELA           = 5;
const int MOVIMIENTOS_BENGALA        = 3;

const int CANTIDAD_ARBOLES_MAPA = 350;
const int CANTIDAD_PIEDRAS_MAPA = 80;

const int CANTIDAD_INICIAL_KOALAS_MAPA = 1;

const int CANTIDAD_PILAS_MAPA    = 30;
const int CANTIDAD_VELAS_MAPA    = 30;
const int CANTIDAD_BENGALAS_MAPA = 10;

const int FILAS    = 20;
const int COLUMNAS = 30;

const int MAX_COMENTARIOS = 20;
const int MAX_LONGITUD_COMENTARIOS = 200;

const double TIEMPO_PERDIDO_ARBOL       = 1;
const double TIEMPO_PERDIDO_ARBOL_PARDO = 0.5;
const double TIEMPO_PERDIDO_PIEDRA      = 2;

#define MOVERSE_ARRIBA     'W'
#define MOVERSE_IZQUIERDA  'A'
#define MOVERSE_ABAJO      'S'
#define MOVERSE_DERECHA    'D'

#define VER_TIEMPO         'T'

//Pre condición:  --
//Post condición: Asigna a posicion_personaje una fila aleatoria y la columna 0, almacena la posición en el vector de coordenadas
//				  y aumenta el tope del vector en 1. 
void asignar_coordenadas_personaje(coordenada_t* posicion_personaje, coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas){
	posicion_personaje->fil = (rand() % FILAS);
	posicion_personaje->col = 0;

	coordenadas[0] = (*posicion_personaje);
	(*tope_coordenadas)++;
}

//Pre condición:  --
//Post condición: Asigna el tipo LINTERNA a un elemento del vector "mochila" del personaje y, dependiendo del tipo del
//				  personaje, le asigna a la linterna los movimientos restantes que le corresponden. Luego aumenta la 
//				  cantidad de elementos en 1.
void rellenar_mochila_linterna(personaje_t* personaje){
	personaje->mochila[personaje->cantidad_elementos].tipo = LINTERNA;
	if (personaje->tipo == PARDO)
		personaje->mochila[personaje->cantidad_elementos].movimientos_restantes = MOVIMIENTOS_LINTERNA_PARDO;
	else
		personaje->mochila[personaje->cantidad_elementos].movimientos_restantes = MOVIMIENTOS_LINTERNA;
	
	(personaje->cantidad_elementos)++;
}

//Pre condición:  --
//Post condición: Dependiendo del tipo del personaje, asigna el tipo VELA a una cantidad de elementos del vector mochila,
//				  les asigna la cantidad de movimientos restantes que les corresponde y aumenta la cantidad de elementos 
//				  en 1 por cada elemento agregado al vector.
void rellenar_mochila_velas(personaje_t* personaje){
	int velas_para_asignar = CANTIDAD_INICIAL_VELAS;
	if (personaje->tipo == POLAR)
		velas_para_asignar = CANTIDAD_INICIAL_VELAS_POLAR;

	for (int i = 0; i < velas_para_asignar; i++){
		personaje->mochila[personaje->cantidad_elementos].tipo = VELA;
		personaje->mochila[personaje->cantidad_elementos].movimientos_restantes = MOVIMIENTOS_VELA;
		(personaje->cantidad_elementos)++;
	}
}

//Pre condición:  --
//Post condición: Si el tipo del personaje es PANDA, asigna el tipo BENGALA a una cantidad correspondiente de elementos
//				  del vector mochila y les asigna los movimientos restantes que les corresponden. Luego aumenta la
// 				  cantidad de elementos en 1 por cada elemento agregado al vector.
void rellenar_mochila_bengalas(personaje_t* personaje){
	if (personaje->tipo == PANDA){
		for (int i = 0; i < CANTIDAD_INICIAL_BENGALAS_PANDA; i++){
			personaje->mochila[personaje->cantidad_elementos].tipo = BENGALA;
			personaje->mochila[personaje->cantidad_elementos].movimientos_restantes = MOVIMIENTOS_BENGALA;
			(personaje->cantidad_elementos)++;
		}
	}
}

//Pre condición:  Recibe un elemento de tipo coordenada_t inicializado.
//Post condición: Devuelve true si las coordenadas introducidas ya pertenecen al vector de coordenadas y devuelve false
//				  si este no es el caso. 
bool coordenadas_ocupadas(coordenada_t posibles_coordenadas, coordenada_t coordenadas[MAX_COORDENADAS], int tope_coordenadas){
	int i = 0;
	bool veredicto = false;
	while ((i < tope_coordenadas) && (!veredicto)){
		if ((posibles_coordenadas.fil == coordenadas[i].fil) && (posibles_coordenadas.col == coordenadas[i].col))
			veredicto = true;
		else 
			i++;
	}
	return veredicto;
}

//Pre condición:  -- 
//Post condición: Asigna a amiga_chloe coordenadas libres, las almacena en el vector de coordenadas y aumenta el tope del vector en 1.  
void asignar_coordenadas_chloe(coordenada_t* amiga_chloe, coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas){
	do{
		amiga_chloe->fil = (rand() % FILAS);
		amiga_chloe->col = (rand() % COLUMNAS); 
	} while (coordenadas_ocupadas(*amiga_chloe, coordenadas, *tope_coordenadas));
	coordenadas[*tope_coordenadas] = (*amiga_chloe);
	(*tope_coordenadas)++;
}

//Pre condición:  Recibe un tipo de elemento que puede ser ARBOL, PIEDRA o KOALA.
//Post condición: Asigna el tipo tipo_de_elemento a la cantidad correspondiente de elementos del vector y aumenta el tope
//				  del vector en 1 por cada elemento agregado a este.
void asignar_elementos_del_mapa_a_vector(elemento_del_mapa_t vector_de_elementos[MAX_ELEMENTOS], int* tope_vector, char tipo_de_elemento, int cantidad){
	for (int i = 0; i < cantidad; i++){
		vector_de_elementos[*tope_vector].tipo = tipo_de_elemento;
		(*tope_vector)++;
	}
}

//Pre condición:  --
//Post condición: Asigna coordenadas libres a todos los elementos del vector, agrega las coordenadas asignadas al vector de
//			      coordenadas y aumenta su tope en 1 por cada elemento cuyas coordenadas fueron añadidas.
void asignar_coordenadas_elementos_del_mapa(elemento_del_mapa_t vector_de_elementos[MAX_ELEMENTOS], int tope_vector, coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas){
	for (int i = 0; i < tope_vector; i++){
		do{
			vector_de_elementos[i].posicion.fil = (rand() % FILAS);
			vector_de_elementos[i].posicion.col = (rand() % COLUMNAS);
		} while (coordenadas_ocupadas(vector_de_elementos[i].posicion, coordenadas, *tope_coordenadas));
		coordenadas[*tope_coordenadas] = vector_de_elementos[i].posicion;
		(*tope_coordenadas)++;
	}
}

//Pre condición:  --
//Post condición: Asigna al campo visible de todos los elementos del vector el valor false.
void invisibilisar_vector_elementos_mapa(elemento_del_mapa_t vector_de_elementos[MAX_ELEMENTOS], int tope_vector){
	for (int i = 0; i < tope_vector; i++)
		vector_de_elementos[i].visible = false;
}

//Pre condición:  --
//Post condición: Asigna el valor false a todos lo campos del arreglo juego que se refieran a la visibilidad.
void invisibilisar_elementos(juego_t* juego){
	juego->chloe_visible = false;
	invisibilisar_vector_elementos_mapa(juego->obstaculos, juego->cantidad_obstaculos);
	invisibilisar_vector_elementos_mapa(juego->herramientas, juego->cantidad_herramientas);
}

void inicializar_juego(juego_t* juego, char tipo_personaje){
	coordenada_t coordenadas[MAX_COORDENADAS];
	int tope_coordenadas  = 0;

	juego->personaje.cantidad_elementos = 0;
	juego->cantidad_obstaculos          = 0;
	juego->cantidad_herramientas        = 0;
	
	juego->personaje.tipo = tipo_personaje;
	asignar_coordenadas_personaje(&(juego->personaje.posicion), coordenadas, &tope_coordenadas);

    rellenar_mochila_linterna(&(juego->personaje));
	rellenar_mochila_velas(&(juego->personaje));
	rellenar_mochila_bengalas(&(juego->personaje));

	juego->personaje.elemento_en_uso = -1;
	juego->personaje.tiempo_perdido  = 0; 
	juego->personaje.ultimo_movimiento = MOVERSE_DERECHA;

	asignar_coordenadas_chloe(&(juego->amiga_chloe), coordenadas, &tope_coordenadas);

	asignar_elementos_del_mapa_a_vector(juego->obstaculos, &(juego->cantidad_obstaculos), ARBOL, CANTIDAD_ARBOLES_MAPA);
	asignar_elementos_del_mapa_a_vector(juego->obstaculos, &(juego->cantidad_obstaculos), PIEDRA, CANTIDAD_PIEDRAS_MAPA);
	asignar_elementos_del_mapa_a_vector(juego->obstaculos, &(juego->cantidad_obstaculos), KOALA, CANTIDAD_INICIAL_KOALAS_MAPA);
	asignar_coordenadas_elementos_del_mapa(juego->obstaculos, juego->cantidad_obstaculos, coordenadas, &tope_coordenadas);

	asignar_elementos_del_mapa_a_vector(juego->herramientas, &(juego->cantidad_herramientas), PILA, CANTIDAD_PILAS_MAPA);
	asignar_elementos_del_mapa_a_vector(juego->herramientas, &(juego->cantidad_herramientas), VELA, CANTIDAD_VELAS_MAPA);
	asignar_elementos_del_mapa_a_vector(juego->herramientas, &(juego->cantidad_herramientas), BENGALA, CANTIDAD_BENGALAS_MAPA);
	asignar_coordenadas_elementos_del_mapa(juego->herramientas, juego->cantidad_herramientas, coordenadas, &tope_coordenadas);

	invisibilisar_elementos(juego);
}

int estado_juego(juego_t juego){
	if ((juego.amiga_chloe.fil == juego.personaje.posicion.fil) && (juego.amiga_chloe.col == juego.personaje.posicion.col))
		return (-1);
	else
		return (0);
}

//Pre condición:  Recibe posicion_personaje incializada, un vector con todos sus elementos inicializados y un tope válido.
//Post condición: Asigna al elemento tipo char que se encuentra en la dirección de memoria proporconada el tipo de elemento que 
//				  le corresponde comparando la posicion_personaje con elementos del vector. Si ninguno cumple la condición el
//				  tipo_de_elemento será VACIO.
void encontrar_tipo_elemento(char* tipo_de_elemento, coordenada_t posicion_personaje, elemento_del_mapa_t vector_de_elementos[MAX_ELEMENTOS], int tope_vector){
	int i = 0;
	(*tipo_de_elemento) = VACIO;
	while((i < tope_vector) && i != (-1)){
		if((posicion_personaje.fil == vector_de_elementos[i].posicion.fil) && (posicion_personaje.col == vector_de_elementos[i].posicion.col)){
			(*tipo_de_elemento) = vector_de_elementos[i].tipo;
			i = (-1);
		} else {
			i++;
		}
	}
}

//Pre condición:  Recibe un elemento de tipo juego_t con todas sus estructuras válidas.
//Post condición: Asigna al elemento tipo char que se encuentra en la dirección de memoria proporcionada el tipo de elemento
//				  que le corresponde.
void asignar_tipo_elemento(char* tipo_de_elemento, juego_t juego){
	encontrar_tipo_elemento(tipo_de_elemento, juego.personaje.posicion, juego.obstaculos, juego.cantidad_obstaculos); 
	if ((*tipo_de_elemento) == VACIO)
		encontrar_tipo_elemento(tipo_de_elemento, juego.personaje.posicion, juego.herramientas, juego.cantidad_herramientas);
}

//Pre condición:  Recibe un vector de tipo elemento_del_mapa_t de tamaño MAX_ELEMENTOS, un elemento de tipo int que
//				  representa el tope de este vector, un vector de tipo coordenada_t de tamaño MAX_COORDENADAS y un
//				  elemento de tipo int que representa el tope de este.
//Post condición: Introduce las coordenadas almacenadas en el primer vector en el segundo vector y aumenta el tope de
//				  este en 1 por cada elemnto agregado.
void cargar_coordenadas(elemento_del_mapa_t vector_de_elementos[MAX_ELEMENTOS], int tope_vector, coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas){
	for (int i = 0; i < tope_vector; i++){
		coordenadas[*tope_coordenadas] = vector_de_elementos[i].posicion;
		(*tope_coordenadas)++;
	}
}

//Pre condición:  Recibe una dirección de memoria a un elemento de tipo juego_t con todas sus estructuras válidas.
//Post condicion: Introduce todas las coordenadas almacenadas en juego al vector de coordenadas y aumenta el tope en 1 por cada
//				  elemento agregado.
void rellenar_vector_coordenadas(juego_t* juego, coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas){
	coordenadas[*tope_coordenadas] = juego->personaje.posicion;
	(*tope_coordenadas)++;
	coordenadas[*tope_coordenadas] = juego->amiga_chloe;
	(*tope_coordenadas)++;
	cargar_coordenadas(juego->obstaculos, juego->cantidad_obstaculos, coordenadas, tope_coordenadas);
	cargar_coordenadas(juego->herramientas, juego->cantidad_herramientas, coordenadas, tope_coordenadas);
}

//Pre condición:  Recibe un tipo_personaje que puede ser PARDO, POLAR o PANDA.
//Post condición: Si el tipo del personaje es PARDO, aumenta en 0.5 el tiempo perdido, caso contrario, lo hace en 1.
void consecuencias_arbol(char tipo_personaje, double* tiempo_perdido){
	if (tipo_personaje == PARDO)
		(*tiempo_perdido) += TIEMPO_PERDIDO_ARBOL_PARDO;
	else
		(*tiempo_perdido) += TIEMPO_PERDIDO_ARBOL;
}

//Pre condición:  Recibe un tipo_personaje que puede ser PARDO, POLAR o PANDA.
//Post condición: Si el tipo del personaje no es POLAR, aumenta en 2 el tiempo perdido. 
void consecuencias_piedra(char tipo_personaje, double* tiempo_perdido){
	if (tipo_personaje != POLAR)
		(*tiempo_perdido) += TIEMPO_PERDIDO_PIEDRA;
}


//Pre condición:  Recibe un vector de coordenadas inicializado.
//Post condición: Devuelve true si no hay elementos en el vector con coordenadas en la columna ingresada y false en caso contrario.
bool hay_espacio_col(coordenada_t coordenadas[MAX_COORDENADAS], int tope_coordenadas, int columna){
	int contador = 0;
	for (int j = 0; j < tope_coordenadas; j++){
		if (coordenadas[j].col == columna)
			contador++;
	}
	return (contador < FILAS);
}

//Pre condición:  Recibe una posición del personaje válida y un vector de coordenadas incializado.
//Post condición: Asigna al personaje una posición que no esté en la primera columna con espacios libres del vector de coordenadas.
void echar_personaje(coordenada_t* posicion_personaje, coordenada_t coordenadas[MAX_COORDENADAS], int tope_coordenadas, int columna){
	if (columna >= COLUMNAS)
		return;

	if (hay_espacio_col(coordenadas, tope_coordenadas, columna)){
		do{
			posicion_personaje->fil = (rand() % FILAS);
			posicion_personaje->col = columna;
		} while (coordenadas_ocupadas(*posicion_personaje, coordenadas, tope_coordenadas));
	} else {
		columna++;
		echar_personaje(posicion_personaje, coordenadas, tope_coordenadas, columna);
	}
}

//Pre condición:  Recibe una dirección de memoria a un elemento de tipo juego_t con todas sus estructuras válidas.
//Post condición: Mueve de posición al personaje a un lugar vacío en la primera columna con espacios libres.
void consecuencias_koala(juego_t* juego){
	coordenada_t coordenadas[MAX_COORDENADAS];
	int tope_coordenadas = 0;
	rellenar_vector_coordenadas(juego, coordenadas, &tope_coordenadas);
	echar_personaje(&(juego->personaje.posicion), coordenadas, tope_coordenadas, 0);
}

//Pre condición:  --
//Post condición: Agrega un elemento tipo KOALA al vector de obstáculos, le asigna coordenadas libres y aumenta el tope del vctor en 1.
void posicionar_nuevo_koala(elemento_del_mapa_t obstaculos[MAX_OBSTACULOS], int* cantidad_obstaculos, coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas){
	obstaculos[*cantidad_obstaculos].tipo = KOALA;
	do{
		obstaculos[*cantidad_obstaculos].posicion.fil = (rand() % FILAS);
		obstaculos[*cantidad_obstaculos].posicion.col = (rand() % COLUMNAS);
	} while (coordenadas_ocupadas(obstaculos[*cantidad_obstaculos].posicion, coordenadas, *tope_coordenadas));
	(*cantidad_obstaculos)++;
}

//Pre condición:  Recibe una dirección de memoria a un elemento de tipo juego_t con todas sus estructuras válidas.
//Post condición: Agrega un elemento tipo Koala al vector de obstáculos en juego y le asigna coordenadas libres, siempre y cuando a
//				  la matriz le quede espacio.
void aparece_koala(juego_t* juego){
	coordenada_t coordenadas[MAX_COORDENADAS];
	int tope_coordenadas = 0;
	rellenar_vector_coordenadas(juego, coordenadas, &tope_coordenadas);
	if (tope_coordenadas < MAX_ELEMENTOS)
		posicionar_nuevo_koala(juego->obstaculos, &(juego->cantidad_obstaculos), coordenadas, &tope_coordenadas);
}

//Pre condición:  Recibe una dirección de memoria a un elemento de tipo juego_t.
//Post condición: Elimina el elemento del vector herramientas que tenga la misma posición que el personaje.
void eliminar_herramienta_mapa(elemento_del_mapa_t herramientas[MAX_HERRAMIENTAS], int* cantidad_herramientas, coordenada_t posicion_personaje){
	int posicion_elemento;
	for (int i = 0; i < (*cantidad_herramientas); i++){
		if ((posicion_personaje.fil == herramientas[i].posicion.fil) && (posicion_personaje.col == herramientas[i].posicion.col)){
			posicion_elemento = i;
		}
	}
	herramientas[posicion_elemento] = herramientas[(*cantidad_herramientas) - 1];
	(*cantidad_herramientas)--;
}

//Pre condición:  Recibe una dirección de memoria a un elemento de tipo juego_t con todas sus estructuras válidas y un tipo de 
//				  elemento que puede ser PILA, VELA O BENGALA.
//Post condición: Si el personaje se encontró con una PILA agrega 5 movimientos a la linterna y si se encontró con una VELA o una 
//				  BENGALA las agrega a la mochila. En ambos casos el elemento es eliminado del mapa.
void recoger_elemento(juego_t* juego, char tipo_de_elemento, int movimientos_a_asignar){
	if (tipo_de_elemento == PILA){
		(juego->personaje.mochila[0].movimientos_restantes) += movimientos_a_asignar;
	} else {
		juego->personaje.mochila[juego->personaje.cantidad_elementos].tipo = tipo_de_elemento;
		juego->personaje.mochila[juego->personaje.cantidad_elementos].movimientos_restantes = movimientos_a_asignar;
		(juego->personaje.cantidad_elementos)++;
	}
	eliminar_herramienta_mapa(juego->herramientas, &(juego->cantidad_herramientas), juego->personaje.posicion);
}

//Pre condición:  Recibe una dirección de memoria a un elemento de tipo juego_t con todas sus estructuras válidas.
//Post condición: Dependiendo del tipo de elemento con el que se encuentra el usuario, se efectúan distintos resultados.
void toparse_elemento(juego_t* juego){
	char tipo_de_elemento;
	asignar_tipo_elemento(&tipo_de_elemento, *juego);

	switch(tipo_de_elemento){
		case ARBOL:
		consecuencias_arbol(juego->personaje.tipo, &(juego->personaje.tiempo_perdido));
		break;
		case PIEDRA:
		consecuencias_piedra(juego->personaje.tipo, &(juego->personaje.tiempo_perdido));
		break;
		case KOALA:
		consecuencias_koala(juego);
		break;
		case PILA:
		recoger_elemento(juego, PILA, MOVIMIENTOS_PILA);
		break;
		case VELA:
		recoger_elemento(juego, VELA, MOVIMIENTOS_VELA);
		break;
		case BENGALA:
		recoger_elemento(juego, BENGALA, MOVIMIENTOS_BENGALA);
		break;
	}
}

//Pre condición:  Recibe la posición del prsonaje incializada y una jugada que puede ser MOVERSE_ARRIBA, MOVERSE_ABAJO,
//				  MOVERSE_IZQUIERDA O MOVERSE_DERECHA.
//Post condición: Devuelve true si la jugada resulta en llevar al personaje fuera de límites y false en caso contrario.
bool es_movimiento_limite(coordenada_t posicion_personaje, char jugada){
	switch(jugada){
		case MOVERSE_ARRIBA:
		return (posicion_personaje.fil == 0);
		break;
		case MOVERSE_ABAJO:
		return (posicion_personaje.fil == 19);
		break;
		case MOVERSE_IZQUIERDA:
		return (posicion_personaje.col == 0);
		break;
		case MOVERSE_DERECHA:
		return (posicion_personaje.col == 29);
		break;
		default:
		return false;
		break;
	}
}

//Pre condición:  Recibe una dirección de memoria a un elemento de tipo juego con todas sus estructuras válidas y una jugada que 
//				  puede ser MOVERSE_ARRIBA, MOVERSE_ABAJO, MOVERSE_IZQUIERDA O MOVERSE_DERECHA.
//Post condición: Cambia la posición del personaje dependiendo de la jugada.
void mover_personaje(juego_t* juego, char jugada){
	juego->personaje.ultimo_movimiento = jugada;
	
	if (es_movimiento_limite(juego->personaje.posicion, jugada))
		return;

	switch(jugada){
		case MOVERSE_ARRIBA:
		(juego->personaje.posicion.fil)--;
		break;
		case MOVERSE_ABAJO:
		(juego->personaje.posicion.fil)++;
		break;
		case MOVERSE_IZQUIERDA:
		(juego->personaje.posicion.col)--;
		break;
		case MOVERSE_DERECHA:
		(juego->personaje.posicion.col)++;
		break;
	}

	toparse_elemento(juego);
}

//Pre condición:  Recibe una dirección de memoria al personaje con todas sus estructuras válidas y una herramienta_requerida que puede
//				  ser VELA, BENGALA o LINTERNA.
//Post condición: Cambia el elemento en uso dependiendo del tipo de herramienta que se requiere de acuerdo a la situación.
void buscar_herramienta(personaje_t* personaje, char herramienta_requerida){
	int i = 0;
	if (personaje->mochila[personaje->elemento_en_uso].tipo == BENGALA){
		return;
	}
		
	if ((personaje->elemento_en_uso != (-1)) && (personaje->mochila[personaje->elemento_en_uso].tipo == herramienta_requerida)){
		personaje->elemento_en_uso = (-1);
		return;
	}

	personaje->elemento_en_uso = (-1);
	while ((i < (personaje->cantidad_elementos)) && ((personaje->elemento_en_uso) == (-1))){
		if ((personaje->mochila[i].tipo == herramienta_requerida) && (personaje->mochila[i].movimientos_restantes > 0))
			personaje->elemento_en_uso = i;
		else
			i++;
	}
}

//Pre condición:  Recibe las posiciones del personaje y de un elemento inicializadas, así como un ultimo_movimiento que puede ser
//				  MOVERSE_ARRIBA, MOVERSE_ABAJO, MOVERSE_DERECHA o MOVERSE_IZQUIERDA.
//Post condición: Devuelve true si se cumplen las condicones, dependientes del último movimiento, y false en el caso contrario.
bool cumple_condicion_linterna(coordenada_t posicion_personaje, coordenada_t posicion_elemento, char ultimo_movimiento){
	switch(ultimo_movimiento){
		case MOVERSE_ARRIBA:
		return ((posicion_elemento.col == posicion_personaje.col) && (posicion_elemento.fil < posicion_personaje.fil));
		break;
		case MOVERSE_ABAJO:
		return ((posicion_elemento.col == posicion_personaje.col) && (posicion_elemento.fil > posicion_personaje.fil));
		break;
		case MOVERSE_IZQUIERDA:
		return ((posicion_elemento.col < posicion_personaje.col) && (posicion_elemento.fil == posicion_personaje.fil));
		break;
		case MOVERSE_DERECHA:
		return ((posicion_elemento.col > posicion_personaje.col) && (posicion_elemento.fil == posicion_personaje.fil));
		break;
		default:
		return false;
		break;
	}
}

//Pre condición:  Recibe las coordenadas del personaje ya inicializadas, un vector de elementos, todos con coordenadas válidas y un último movimiento
//				  que puede ser MOVERSE_ARRIBA, MOVERSE_ABAJO, MOVERSE_DERECHA o MOVERSE_IZQUIERDA.
//Post condición: Si se cumplen las condiciones para algún elemento del vector, asigna el valor true al campo visible de este.
void activar_linterna_vector_elementos(coordenada_t posicion_personaje, elemento_del_mapa_t vector_de_elementos[MAX_ELEMENTOS], int tope_vector, char ultimo_movimiento){
	for (int i = 0; i < tope_vector; i++){
		if (cumple_condicion_linterna(posicion_personaje, vector_de_elementos[i].posicion, ultimo_movimiento)){
			vector_de_elementos[i].visible = true;
		}
	}
}

//Pre condición:  Recibe una dirección de memoria a un elemento de tipo juego_t con todas sus estructuras válidas.
//Post condición: Asigan el valor true a todos lo elementos con campos relacionados a la visibilidad que cumplan las condiciones necesarias.
void activar_linterna(juego_t* juego){
	if (cumple_condicion_linterna(juego->personaje.posicion, juego->amiga_chloe, juego->personaje.ultimo_movimiento))
		juego->chloe_visible = true;

	activar_linterna_vector_elementos(juego->personaje.posicion, juego->obstaculos, juego->cantidad_obstaculos, juego->personaje.ultimo_movimiento);
	activar_linterna_vector_elementos(juego->personaje.posicion, juego->herramientas, juego->cantidad_herramientas, juego->personaje.ultimo_movimiento);
}

//Pre condición:  Recibe las coordenadas del personaje y de un elemento, ambas inicializadas.
//Post condición: Devuelve true si las coordenadas introducidas cumplen con las condiciones establecidas y false en caso contrario.
bool cumple_condicion_vela(coordenada_t posicion_personaje, coordenada_t posicion_elemento){
	return ((abs(posicion_personaje.fil - posicion_elemento.fil) <= 1) && (abs(posicion_personaje.col - posicion_elemento.col) <= 1));
}

//Pre condición:  Recibe las coordenadas del personaje ya inicializadas y un vector de elementos, todos con coordenadas válidas
//Post condición: Si se cumplen las condiciones para algún elemento del vector, asigna el valor true al campo visible de este.
void activar_vela_vector_elementos(coordenada_t posicion_personaje, elemento_del_mapa_t vector_de_elementos[MAX_ELEMENTOS], int tope_vector){
	for (int i = 0; i < tope_vector; i++){
		if (cumple_condicion_vela(posicion_personaje, vector_de_elementos[i].posicion)){
			vector_de_elementos[i].visible = true;
		}
	}
}

//Pre condición:  Recibe una dirección de memoria a un elemento de tipo juego_t con todas sus estructuras válidas.
//Post condición: Asigna el valor true a todos lo elementos con campos relacionados a la visibilidad que cumplan las condiciones necesarias.
void activar_vela(juego_t* juego){
	if (cumple_condicion_vela(juego->personaje.posicion, juego->amiga_chloe))
		juego->chloe_visible = true;

	activar_vela_vector_elementos(juego->personaje.posicion, juego->obstaculos, juego->cantidad_obstaculos);
	activar_vela_vector_elementos(juego->personaje.posicion, juego->herramientas, juego->cantidad_herramientas);
}

//Pre condición:  --
//Post condición: Asigna coordenadas aleatorias al elemento.
void generar_coordenadas_aleatorias(coordenada_t* centro_bengala){
	centro_bengala->fil = (rand() % FILAS);
	centro_bengala->col = (rand() % COLUMNAS);
}

//Pre condición:  Recibe la posición del centro de la bengala y de un elemento, ambas inicializadas.
//Post condición: Devuelve true si las coordenadas introducidas cumplen con las condiciones establecidas y false en caso contrario.
bool cumple_condicion_bengala(coordenada_t centro_bengala, coordenada_t posicion_elemento){
	return ((abs(centro_bengala.fil - posicion_elemento.fil) + abs(centro_bengala.col - posicion_elemento.col)) <= 3);
}

//Pre condición:  Recibe las coordenadas del centro de la bengala ya inicializadas y un vector de elementos, todos con coordenadas válidas
//Post condición: Si se cumplen las condiciones para algún elemento del vector, asigna el valor true al campo visible de este.
void activar_bengala_vector_elementos(coordenada_t centro_bengala, elemento_del_mapa_t vector_de_elementos[MAX_ELEMENTOS], int tope_vector){
	for (int i = 0; i < tope_vector; i++){
		if (cumple_condicion_bengala(centro_bengala, vector_de_elementos[i].posicion))
			vector_de_elementos[i].visible = true;
	}
}

//Pre condición:  Recibe una dirección de memoria a un elemento de tipo juego_t con todas sus estructuras válidas.
//Post condición: Asigna el valor true a todos lo elementos con campos relacionados a la visibilidad que cumplan las condiciones necesarias.
void activar_bengala(juego_t* juego){
	coordenada_t centro_bengala;
	generar_coordenadas_aleatorias(&centro_bengala);
	if (cumple_condicion_bengala(centro_bengala, juego->amiga_chloe))
		juego->chloe_visible = true;

	activar_bengala_vector_elementos(centro_bengala, juego->obstaculos, juego->cantidad_obstaculos);
	activar_bengala_vector_elementos(centro_bengala, juego->herramientas, juego->cantidad_herramientas);
}

//Pre condición:  Recibe una dirección de memoria de un elemento de tipo juego_t con todas sus estructuras válidas y una jugada
//				  que puede ser LINTERNA, VELA o BENGALA.
//Post condición: Dependiendo del tipo del elemento en uso, activa la herramienta correspondiente.
void usar_herramienta(juego_t* juego, char jugada){
	char tipo_elemento_en_uso = (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo);
	
	switch(tipo_elemento_en_uso){
		case LINTERNA:
		activar_linterna(juego);
		break;
		case VELA:
		activar_vela(juego);
		break;
		case BENGALA:
		activar_bengala(juego);
		break;
	}
	
	if ((!es_movimiento_limite(juego->personaje.posicion, jugada)) || ((es_movimiento_limite(juego->personaje.posicion, jugada)) && (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == BENGALA))) {
		(juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes)--;
	}

}

//Pre condición:  --
//Post condición: Imprime un mensaje con la suma del tiempo perdido y el tiempo transcurrido desde que empezó la partida.
void mostrar_tiempo_actual(double tiempo_perdido){
	double tiempo_transcurrido = tiempo_actual() + tiempo_perdido;
	printf("Han trascurrido %.1f segundos hasta ahora.\n", tiempo_transcurrido);
}

bool es_jugada_valida(char jugada){
	return ((jugada == MOVERSE_ARRIBA) || (jugada == MOVERSE_ABAJO) || (jugada == MOVERSE_IZQUIERDA) || (jugada == MOVERSE_DERECHA) || (jugada == LINTERNA) || (jugada == VELA) || (jugada == BENGALA) || (jugada == VER_TIEMPO));
}
void realizar_jugada(juego_t* juego, char jugada){
	if (!es_jugada_valida(jugada)){
		printf("Por favor, ingrese una de las jugadas permitidas (W, A, S, D, L, V, E, T).\n");
		return;
	}

	invisibilisar_elementos(juego);

	if ((juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes) == 0)
		juego->personaje.elemento_en_uso = (-1);

	if ((jugada == LINTERNA) && (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo != LINTERNA) && (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo != BENGALA) && (juego->personaje.mochila[0].movimientos_restantes > 0))
		aparece_koala(juego);

	if ((jugada == MOVERSE_ARRIBA) || (jugada == MOVERSE_DERECHA) || (jugada == MOVERSE_ABAJO) || (jugada == MOVERSE_IZQUIERDA))
		mover_personaje(juego, jugada);
	else if ((jugada == LINTERNA) || (jugada == VELA) || (jugada == BENGALA))
		buscar_herramienta(&(juego->personaje), jugada);
	else if (jugada == VER_TIEMPO)
		mostrar_tiempo_actual(juego->personaje.tiempo_perdido);

	if (juego->personaje.elemento_en_uso != (-1))
		usar_herramienta(juego, jugada);

	if((juego->personaje.tipo == PANDA) && (juego->personaje.tiempo_perdido >= 30))
		juego->chloe_visible = true;
}


//Pre condición:  --
//Post condición: Imprime una fila de CUADRADOs.
void imprimr_fila_de_separacion(){
	for(int i = 0; i < ((COLUMNAS * 4) + 1); i++){
		printf(LETRA_VERDE_OSCURO "%s" RESETEAR_COLOR, CUADRADO);
	}
	printf("\n");
}

//Pre condición:  Recibe una matriz mapa_del_juego con elementos válidos (A,R,K,V,B,E,I,G,P,C) y una matriz de comentarios
//				  inicializada.
//Post condición: Imprime las matrices.
void imprimir_matriz_juego(char mapa_del_juego[FILAS][COLUMNAS], char comentarios[MAX_COMENTARIOS][MAX_LONGITUD_COMENTARIOS]){
	imprimr_fila_de_separacion();
	for (int i = 0; i < FILAS; i++){
		for (int j = 0; j < COLUMNAS; j++){

			printf(LETRA_VERDE_OSCURO "%s" RESETEAR_COLOR, CUADRADO);
			
			if (mapa_del_juego[i][j] == VACIO)
				printf(FONDO_VERDE_CLARO " %c " RESETEAR_COLOR, mapa_del_juego[i][j]);
			else if ((mapa_del_juego[i][j] == PARDO) || (mapa_del_juego[i][j] == PANDA) || (mapa_del_juego[i][j] == POLAR))
				printf(FONDO_BLANCO LETRA_NEGRA " %c " RESETEAR_COLOR, mapa_del_juego[i][j]);
			else if (mapa_del_juego[i][j] == ARBOL)
				printf(FONDO_BLANCO LETRA_VERDE_OSCURO " %c " RESETEAR_COLOR, mapa_del_juego[i][j]);
			else if (mapa_del_juego[i][j] == PIEDRA)
				printf(FONDO_BLANCO LETRA_GRIS " %c " RESETEAR_COLOR, mapa_del_juego[i][j]);
			else if (mapa_del_juego[i][j] == KOALA)
				printf(FONDO_BLANCO LETRA_CYAN " %c " RESETEAR_COLOR, mapa_del_juego[i][j]);
			else if (mapa_del_juego[i][j] == VELA)
				printf(FONDO_BLANCO LETRA_AZUL " %c " RESETEAR_COLOR, mapa_del_juego[i][j]);
			else if (mapa_del_juego[i][j] == BENGALA)
				printf(FONDO_BLANCO LETRA_ROJA " %c " RESETEAR_COLOR, mapa_del_juego[i][j]);
			else if (mapa_del_juego[i][j] == PILA)
				printf(FONDO_BLANCO LETRA_AMARILLA " %c " RESETEAR_COLOR, mapa_del_juego[i][j]);
			else if (mapa_del_juego[i][j] == CHLOE)
				printf(FONDO_BLANCO LETRA_NEGRA " %c " RESETEAR_COLOR, mapa_del_juego[i][j]);
		}
		printf(LETRA_VERDE_OSCURO "%s" RESETEAR_COLOR, CUADRADO);
		printf("%s\n", comentarios[i]);
		imprimr_fila_de_separacion();
	}
}

//Pre condición:  --
//Post condición: Asigna a cada valor de la matriz un espacio vacío. 
void inicializar_matriz_mapa(char mapa_del_juego[FILAS][COLUMNAS]){
	for (int i = 0; i < FILAS; i++){
		for (int j = 0; j < COLUMNAS; j++){
			mapa_del_juego[i][j] = VACIO;
		}
	}
}

//Pre condición:  Recibe un vector de elementos del mapa con todos sus elementos inicializados.
//Post condición: Asigna el tipo de un elemento del vector a la matriz en caso de que este sea visible.
void posicionar_elementos_del_mapa(elemento_del_mapa_t vector_de_elementos[MAX_ELEMENTOS], int tope_vector, char mapa_del_juego[FILAS][COLUMNAS]){
	for (int i = 0; i < tope_vector; i++){
		if (vector_de_elementos[i].visible)
			mapa_del_juego[vector_de_elementos[i].posicion.fil][vector_de_elementos[i].posicion.col] = vector_de_elementos[i].tipo;
	}
}

//Pre condición:  Recibe un tipo de personaje que puede ser PARDO, PANDA o POLAR.
//Post condición: Asigna a cada vector un string con un comentario.
void rellenar_matriz_comentarios(char comentarios[MAX_COMENTARIOS][MAX_LONGITUD_COMENTARIOS], char tipo_personaje){
	strcpy(comentarios[0], "		¡ENCONTRÁ A CHLOE!");
	strcpy(comentarios[1], "  Tenés 120 segundos para encontrar a Chloe (C)");
	strcpy(comentarios[2], "  OBSTÁCULOS QUE TE ENCONTRARÁS: ");
	
	if (tipo_personaje == PARDO)
		strcpy(comentarios[3], "  ARBOL (A)    -->  Te hace perder 0.5 segundos");
	else
		strcpy(comentarios[3], "  ARBOL (A)    -->  Te hace perder 1 segundo");

	if (tipo_personaje == POLAR)
		strcpy(comentarios[4], "  PIEDRA (R)   -->  No te hace perder segundos");
	else
		strcpy(comentarios[4], "  PIEDRA (R)   -->  Te hace perder 2 segundos");

	strcpy(comentarios[5], "  KOALA (K)    -->  Te devuelve a la primera");
	strcpy(comentarios[6], "                    columna");
	strcpy(comentarios[7], "  HERRAMIENTAS QUE PODÉS USAR:");
	strcpy(comentarios[8], "  LINTERNA (L) -->  Te deja ver todos los");
	strcpy(comentarios[9], "                    elementos delante tuyo");
	strcpy(comentarios[10], "                    Al prenderla aparece un");
	strcpy(comentarios[11], "                    nuevo Koala en el mapa");
	strcpy(comentarios[12], "  VELA (V)     -->  Te deja ver todos los");
	strcpy(comentarios[13], "                    elementos al lado tuyo");
	strcpy(comentarios[14], "  BENGALA (E)  -->  Ilumina un sector al azar");
	strcpy(comentarios[15], "                    cada vez que te muevas");
	strcpy(comentarios[16], "  En el mapa podrás encontrar VELAS (V) y");
	strcpy(comentarios[17], "  BENGALAS (E) que se agregarán a tu mochila");
	strcpy(comentarios[18], "  También te podrás encontrar BETERIAS (B) que");
	strcpy(comentarios[19], "  le darán 5 movimientos más a tu linterna");
}

void mostrar_juego(juego_t juego){
	char mapa_del_juego[FILAS][COLUMNAS];
	char comentarios[MAX_COMENTARIOS][MAX_LONGITUD_COMENTARIOS];

	rellenar_matriz_comentarios(comentarios, juego.personaje.tipo);

	inicializar_matriz_mapa(mapa_del_juego);
	if (juego.chloe_visible) mapa_del_juego[juego.amiga_chloe.fil][juego.amiga_chloe.col] = CHLOE;
	posicionar_elementos_del_mapa(juego.obstaculos, juego.cantidad_obstaculos, mapa_del_juego);
	posicionar_elementos_del_mapa(juego.herramientas, juego.cantidad_herramientas, mapa_del_juego);
	mapa_del_juego[juego.personaje.posicion.fil][juego.personaje.posicion.col] = juego.personaje.tipo;
	imprimir_matriz_juego(mapa_del_juego, comentarios);
}