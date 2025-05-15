#include "menu_aterrizaje.h"

#include <stdio.h>

#include "auxiliares_menus.h"
#include "../transformaciones.h"
#include "../../data/variables_globales.h"

// Palabras que contienen los mensajes a mostrar
struct Palabra* primer_mensaje = {0};
struct Palabra* puntuacion_mensaje = {0};

// Contador para variar el mensaje al colisionar
int opcion_colision = 0;
// Contador para variar el mensaje al aterrizar bruscamente
int opcion_brusco = 0;
// Contador para variar el mensaje al aterrizar perfectamente
int opcion_perfecto = 0;

void generar_mensaje_aterrizaje(int puntuacion) {
	puntuacion_mensaje = crear_mensaje_puntuacion(puntuacion, origen_msj_4);
	switch(tipo_aterrizaje) {
		case PERFECTO:
			primer_mensaje = crear_palabra_desde_cadena("P", origen_msj_1);	
		break;	
		case BRUSCO: 
			primer_mensaje = crear_palabra_desde_cadena("X", origen_msj_1);
		break;
		default: // colision
			primer_mensaje = crear_palabra_desde_cadena("C", origen_msj_1);
		break;
	}
}

void dibujar_mensajes_aterrizaje() {
	dibujar_palabra(primer_mensaje);
	dibujar_palabra(puntuacion_mensaje);
}


