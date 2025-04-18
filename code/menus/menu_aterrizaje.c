#include "menu_aterrizaje.h"

#include <stdio.h>

#include "auxiliares_menus.h"
#include "../palabra.h"
#include "../transformaciones.h"
#include "../../data/constantes.h"
#include "../../data/variables_globales.h"

const struct Punto origen_msj_1 = {100, tamano_inicial_pantalla_Y/2-50};
const struct Punto origen_msj_2 = {100, tamano_inicial_pantalla_Y/2-25};
const struct Punto origen_msj_3 = {100, tamano_inicial_pantalla_Y/2};
const struct Punto origen_msj_4 = {100, tamano_inicial_pantalla_Y/2+50};
const struct Punto origen_msj_5 = {100, tamano_inicial_pantalla_Y/2+200};

struct Palabra* primer_mensaje = {0};
struct Palabra* segundo_mensaje = {0};
struct Palabra* tercer_mensaje = {0};
struct Palabra* puntuacion_mensaje = {0};
struct Palabra* presione_espacio = {0};

int opcion_colision = 0;
int opcion_brusco = 0;
int opcion_perfecto = 0;

void generar_mensaje_aterrizaje(int puntuacion) {
	char buffer[5];
	char mensaje[12] = "XXXX POINTS";
	crear_cadena_dado_valor_4_digitos(puntuacion, buffer);
	mensaje[0] = buffer[0];
	mensaje[1] = buffer[1];
	mensaje[2] = buffer[2];
	mensaje[3] = buffer[3];
	puntuacion_mensaje = crear_palabra_desde_cadena(mensaje, origen_msj_4);
	presione_espacio = crear_palabra_desde_cadena("PRESIONE ESPACIO PARA SEGUIR JUGANDO", origen_msj_5);
	switch(tipo_aterrizaje) {
		case PERFECTO:
			switch (opcion_perfecto) {
			case 0:
				primer_mensaje = crear_palabra_desde_cadena("CONGRATULATIONS", origen_msj_1);
				segundo_mensaje = crear_palabra_desde_cadena("A PERFECT LANDING", origen_msj_2);
				tercer_mensaje = crear_palabra_desde_cadena("", origen_msj_3);
			break;
			default:
			break;
			}
			
		break;	
		case BRUSCO: 
			switch (opcion_brusco) {
			case 0:
				primer_mensaje = crear_palabra_desde_cadena("NOT QUITE GRACEFUL", origen_msj_1);
				segundo_mensaje = crear_palabra_desde_cadena("BUT HEY, YOU'RE ALIVE - I GUESS THAT COUNTS", origen_msj_2);
				tercer_mensaje = crear_palabra_desde_cadena("YOU PROBABLY SQUASHED SOME LUNARCITOS THO", origen_msj_3);
			break;
			
			default:
				primer_mensaje = crear_palabra_desde_cadena("ROUGH, BUT LANDED", origen_msj_1);
				segundo_mensaje = crear_palabra_desde_cadena("EVER HEARD OF GENTLE", origen_msj_2);
				tercer_mensaje = crear_palabra_desde_cadena("THERE GOES THE LANDING GEAR", origen_msj_3);
			break;
			}
		break;
		default: // colision
		switch(opcion_colision) {
			case 0:
				primer_mensaje = crear_palabra_desde_cadena("YOU CRASHED", origen_msj_1);
				segundo_mensaje = crear_palabra_desde_cadena("DID YOU EVEN READ THE MANUAL?", origen_msj_2);
				tercer_mensaje = crear_palabra_desde_cadena("TIP: SLOW DOWN BEFORE THE MOON DOES IT FOR YOU", origen_msj_3);
				opcion_colision++;
			break;
			case 1:
				primer_mensaje = crear_palabra_desde_cadena("STILL CRASHING?", origen_msj_1);
				segundo_mensaje = crear_palabra_desde_cadena("TRY USING THE BRAKE", origen_msj_2);
				tercer_mensaje = crear_palabra_desde_cadena("OR MAYBE JUST CLOSE YOUR EYES", origen_msj_3);
				opcion_colision++;
			break;
	
			default:
				primer_mensaje = crear_palabra_desde_cadena("SERIOUSLY?", origen_msj_1);
				segundo_mensaje = crear_palabra_desde_cadena("STOP DESTROYING THE MOON", origen_msj_2);
				tercer_mensaje = crear_palabra_desde_cadena("NASA IS NOT GONNA CALL YOU BACK", origen_msj_3);
				opcion_colision = 0;
			break;
		}
		break;
	}

	escalar_menu_aterrizaje(factor_escalado);
}

void dibujar_mensajes_aterrizaje(HDC hdc) {
	dibujar_palabra(primer_mensaje, hdc);
	dibujar_palabra(segundo_mensaje, hdc);
	dibujar_palabra(tercer_mensaje, hdc);
	dibujar_palabra(puntuacion_mensaje, hdc);
	dibujar_palabra(presione_espacio, hdc);
}

void escalar_menu_aterrizaje(float factor){
	escalar_palabra_en_escena_dados_ejes(primer_mensaje, factor, factor);
	escalar_palabra_en_escena_dados_ejes(segundo_mensaje, factor, factor);
	escalar_palabra_en_escena_dados_ejes(tercer_mensaje, factor, factor);
	escalar_palabra_en_escena_dados_ejes(puntuacion_mensaje, factor, factor);
	escalar_palabra_en_escena_dados_ejes(presione_espacio, factor, factor);
}


void borrar_menu_aterrizaje() {
	destruir_palabra(primer_mensaje);
	destruir_palabra(segundo_mensaje);
	destruir_palabra(tercer_mensaje);
	destruir_palabra(puntuacion_mensaje);
}