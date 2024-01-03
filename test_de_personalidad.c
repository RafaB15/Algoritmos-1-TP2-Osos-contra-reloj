#include <stdio.h>
#include <stdbool.h>
#include "test_de_personalidad.h"

#define POLAR 'I'
#define PANDA 'P'
#define PARDO 'G'

#define ANIME 'A'
#define LIMPIEZA 'L'
#define MUSICA_POP 'M'

#define FOCAS 'F'
#define BAMBU 'B'
#define PESCADO 'P'

#define PREGUNTA_CANAL 'C'
#define PREGUNTA_ALIMENTO 'X'
#define PREGUNTA_TORRE 'T'
#define PREGUNTA_GRITO 'S'

const int PUNTOS_LIMPIEZA = 1;
const int PUNTOS_ANIME = 2;
const int PUNTOS_MUSICA_POP = 3;

const int PUNTOS_FOCAS = 3;
const int PUNTOS_BAMBU = 6;
const int PUNTOS_PESCADO = 9;

const int OPCION_MINIMA = 1;
const int OPCION_MAXIMA = 18;

const int LIMITE_INFERIOR_POLAR = 5;
const int LIMITE_SUPERIOR_POLAR = 24;
const int LIMITE_INFERIOR_PANDA = 25;
const int LIMITE_SUPERIOR_PANDA = 43;


//Pre condición:  Recibe un char que represente la pregunta que se quiere imprimir por pantalla.
//Post condición: Imprime por pantalla la pregunta requerida.
void imprimir_pegunta(char tipo_de_pregunta){
	switch(tipo_de_pregunta){
		case PREGUNTA_CANAL:
		printf("Vas a ver televisión un rato, pones el canal de: Anime (A), Musica Pop (M), Limpieza (L).\n");
		break;
		case PREGUNTA_ALIMENTO:
		printf("Solo podes guardar un alimento en tu vianda: Bambú (B), Pescado (P), Focas (F).\n");
		break;
		case PREGUNTA_TORRE:
		printf("Te compras una torre con tus dos hermanos de 18 pisos. ¿En que piso te gustaría vivir?\n");
		break;
		case PREGUNTA_GRITO:
		printf("¡Oh, una rata! ¿Que tan fuerte gritas del 1 al 18? Siendo 1 no gritar y 18 desgarrarse la garganta.\n");
		break;
	}
}

//Pre condición:  Recibe un char que identifica el tipo de pregunta.
//Post condición: Imprime una advertencia que dependerá del tipo de pregunta.
void imprimir_mensaje_de_advertencia(char tipo_de_pregunta){
	if ((tipo_de_pregunta == PREGUNTA_CANAL) || (tipo_de_pregunta == PREGUNTA_ALIMENTO)){
		printf("Respuesta inválida. Por favor, ingrese una de las letras indicadas.\n");
	} else {
		printf("Respuesta inválida. Por favor ingrese ún número válido. \n");
	}
}

//Pre condición:  Recibe un char que represente el canal elegido.
//Post condición: Devuelve true si la respuesta es una de las opciones 
//				  válidas: ANIME, MUSICA_POP o LIMPIEZA. Devuelve false en caso 
//				  contrario.
bool verificar_respuesta_canal(char canal){
	return ((canal == ANIME) || (canal == MUSICA_POP) || (canal == LIMPIEZA));
}

//Pre condición:  Recibe un char que represente el alimento elegido.
//Post condición: Devuelve true si la respuesta es una de las opciones 
//				  válidas: FOCAS, BAMBU o PESCADO. Devuelve false en caso 
//				  contrario.
bool verificar_respuesta_alimento(char alimento){
	return ((alimento == FOCAS) || (alimento == BAMBU) || (alimento == PESCADO));
}

//Pre condición:  Recibe un int que represente el piso deseado o nivel de grito elegido.
//Post condición: Devuelve true si la respuesta es una de las opciones válidas: Números
//				  entre OPCION_MINIMA y OPCION_MAXIMA inclusive. Devuelve false en caso 
//				  contrario.
bool verificar_respuesta_entero(int respuesta){
	return ((respuesta >= OPCION_MINIMA) && (respuesta <= OPCION_MAXIMA));
}

//Pre condición:  Recibe una dirección de memoria.
//Post condición: Obtiene un char del usuario, verifica que este sea válido y lo almacena en la
//				  dirección de memoria proporcionada.
void obtener_respuesta_canal(char* canal){
	scanf(" %c", canal);
	while(!verificar_respuesta_canal(*canal)){
		imprimir_mensaje_de_advertencia(PREGUNTA_CANAL);
		imprimir_pegunta(PREGUNTA_CANAL);
		scanf(" %c", canal);
	}	
}

//Pre condición:  Recibe una dirección de memoria.
//Post condición: Obtiene un char del usuario, verifica que este sea válido y lo almacena en la
//				  dirección de memoria proporcionada.
void obtener_respuesta_alimento(char* alimento){
	scanf(" %c", alimento);
	while(!verificar_respuesta_alimento(*alimento)){
		imprimir_mensaje_de_advertencia(PREGUNTA_ALIMENTO);
		imprimir_pegunta(PREGUNTA_ALIMENTO);
		scanf(" %c", alimento);
	}	
}

//Pre condición:  Recibe un char que represente el tipo de pregunta y una dirección de memoria.
//Post condición: Obtiene un int del usuario, verifica que este sea válido y lo almacena en la
//				  dirección de memoria proporcionada.
void obtener_respuesta_entero(char tipo_de_pregunta, int* respuesta){
	scanf("%i", respuesta);
	while(!verificar_respuesta_entero(*respuesta)){
		imprimir_mensaje_de_advertencia(tipo_de_pregunta);
		imprimir_pegunta(tipo_de_pregunta);
		scanf("%i", respuesta);
	}	
}

//Pre condición:  Recibe un char que represente el canal elegido.
//Post condición: Devuelve un int con la cantidad de puntos que le corresponden al char ingresado.
int obtener_puntos_canal(char canal_elegido){
	int puntos = 0;
	switch (canal_elegido){
		case LIMPIEZA:
		puntos = PUNTOS_LIMPIEZA;
		break;
		case ANIME:
		puntos = PUNTOS_ANIME;
		break;
		case MUSICA_POP:
		puntos = PUNTOS_MUSICA_POP;
		break;
	}
	return puntos;
}

//Pre condición:  Recibe un char que represente el alimento elegido.
//Post condición: Devuelve un int con la cantidad de puntos que le corresponden al char ingresado.
int obtener_puntos_alimento(char alimento_elegido){
	int puntos = 0;
	switch (alimento_elegido){
		case FOCAS:
		puntos = PUNTOS_FOCAS;
		break;
		case BAMBU:
		puntos = PUNTOS_BAMBU;
		break;
		case PESCADO:
		puntos = PUNTOS_PESCADO;
		break;
	}
	return puntos;
}

//Pre condición:  Recibe un int que represente los puntos obtenidos por el canal elegido, 
//				  un int que represente los puntos obtenidos por el alimento elegido,
//				  un int que represente el piso elegido y un int que represente el nivel
//				  de grito elegido.
//Post condición: Devuelve un int que represente el puntaje total obtenido.   
int obtener_puntaje_total(int puntos_canal, int puntos_alimento, int piso, int grito){
	return ((puntos_canal * puntos_alimento) + piso + grito);
}

//Pre condición:  Recibe un int que represente el puntaje obtenido y una dirección de memoria.
//Post condición: Almacena un char que represente a uno de los osos en la dirección de memoria proporcionada.
void obtener_personalidad(int puntaje_obtenido, char* personalidad){
	if ((puntaje_obtenido >= LIMITE_INFERIOR_POLAR) && (puntaje_obtenido <= LIMITE_SUPERIOR_POLAR)){
		*personalidad = POLAR;
	} else if ((puntaje_obtenido >= LIMITE_INFERIOR_PANDA) && (puntaje_obtenido <= LIMITE_SUPERIOR_PANDA)){
		*personalidad = PANDA;
	} else {
		*personalidad = PARDO;
	}
}

//Pre condición:  Recibe un char que represente a un oso.
//Post condición: Imprime por pantalla el resultado del test.
void imprimir_personalidad(char personalidad){
	switch (personalidad){
		case POLAR:
		printf("¡Tu personalidad escandalosa es - Polar (I) -!\n");
		break;
		case PANDA:
		printf("¡Tu personalidad escandalosa es - Panda (P) -!\n");
		break;
		case PARDO:
		printf("¡Tu personalidad escandalosa es - Pardo (G) -!\n");
		break;
	}
}

//Pre condición:  Recibe una dirección de memoria.
//Post condición: Después de hacerle preguntas al usuario, almacena
//				  una caracter que simboliza a Polar, Panda o Pardo 
//				  dentro de la dirección de memoria proporcionada.
void test_de_personalidad(char* personalidad_detectada){
	char canal = 0;
	char alimento = 0;
	int piso = 0;
	int grito = 0;

	int puntos_canal = 0;
	int puntos_alimento = 0;
	int puntaje_total = 0;

	imprimir_pegunta(PREGUNTA_CANAL);
	obtener_respuesta_canal(&canal);

	imprimir_pegunta(PREGUNTA_ALIMENTO);
	obtener_respuesta_alimento(&alimento);

	imprimir_pegunta(PREGUNTA_TORRE);
	obtener_respuesta_entero(PREGUNTA_TORRE, &piso);

	imprimir_pegunta(PREGUNTA_GRITO);
	obtener_respuesta_entero(PREGUNTA_GRITO, &grito);

	puntos_canal = obtener_puntos_canal(canal);
	puntos_alimento = obtener_puntos_alimento(alimento);
	puntaje_total = obtener_puntaje_total(puntos_canal, puntos_alimento, piso, grito);

	obtener_personalidad(puntaje_total, personalidad_detectada);
	imprimir_personalidad(*personalidad_detectada);
}