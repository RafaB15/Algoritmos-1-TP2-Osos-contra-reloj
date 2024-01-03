#include <stdio.h>
#include <time.h>
#include "test_de_personalidad.h"
#include "osos_contra_reloj.h"
#include "utiles.h"

//Pre condición:  Recibe un double que representa el tiempo total.
//Post condición: Imprimte un mensaje de victoria si el tiempo total es menor o igual a 120 y uno de derrota en caso contrario.
void imprimir_resultado(double tiempo_total){
	if (tiempo_total > 120){
		printf("\
						########  ######## ########  ########  ####  ######  ######## ######## \n \
						##     ## ##       ##     ## ##     ##  ##  ##    ##    ##    ##       \n \
						##     ## ##       ##     ## ##     ##  ##  ##          ##    ##       \n \
						########  ######   ########  ##     ##  ##   ######     ##    ######   \n \
						##        ##       ##   ##   ##     ##  ##        ##    ##    ##       \n \
						##        ##       ##    ##  ##     ##  ##  ##    ##    ##    ##       \n \
						##        ######## ##     ## ########  ####  ######     ##    ######## \n \
								 ¡NO ENCONTRASTE A CHLOE A TIEMPO!\n");
	} else if (tiempo_total <= 120){
		printf("\
						 ######      ###    ##    ##    ###     ######  ######## ######## \n \
						##    ##    ## ##   ###   ##   ## ##   ##    ##    ##    ##       \n \
						##         ##   ##  ####  ##  ##   ##  ##          ##    ##       \n \
						##   #### ##     ## ## ## ## ##     ##  ######     ##    ######   \n \
						##    ##  ######### ##  #### #########       ##    ##    ##       \n \
						##    ##  ##     ## ##   ### ##     ## ##    ##    ##    ##       \n \
 		 				 ######   ##     ## ##    ## ##     ##  ######     ##    ######## \n \
			        			       ¡ENCONTRASTE A CHLOE A TIEMPO!\n");
	}
}

//Pre condición:  Recibe una dirección de memoria a un elemento de tipo char.
//Post condición: Asigna el caracter que el usuario escriba al elemento cuya dirección de memoria fue proporcionada.
void asignar_jugada(char* jugada){
	printf("Por favor, ingrese su jugada: ");
	scanf(" %c", jugada);
	printf("\n");
}

//Pre condición:  Recibe dos direcciones de memoria. Una a un elemento de tipo juego_t y otra a un elemento de tipo char.
//Post condición: Repite el ciclo de recibir la jugada del ususario, realizar la jugada y mostrar el mapa actualizado hasta que
//				  se encuentre a Chloe.
void jugar_juego(juego_t* juego, char* jugada){
	while (!(estado_juego(*juego))){
		asignar_jugada(jugada);
		realizar_jugada(juego, *jugada);
		mostrar_juego(*juego);
	}
}

int main(){
	char personalidad_detectada = 0;
	char jugada = 0;
	juego_t juego;
	double tiempo_total;

	system("clear");
	srand((unsigned)time(NULL));

	test_de_personalidad(&personalidad_detectada);

	inicializar_juego(&juego, personalidad_detectada);
	mostrar_juego(juego);
	iniciar_cronometro();
	jugar_juego(&juego, &jugada);

	tiempo_total = detener_cronometro() + (juego.personaje.tiempo_perdido);
	
	imprimir_resultado(tiempo_total);
}