#include "auxiliares_menus.h"

#include "../../data/constantes.h"

// Posiciones de los mensajes
const struct Punto origen_msj_1 = {100, tamano_inicial_pantalla_Y/2-60};
const struct Punto origen_msj_2 = {100, tamano_inicial_pantalla_Y/2-30};
const struct Punto origen_msj_3 = {100, tamano_inicial_pantalla_Y/2};
const struct Punto origen_msj_4 = {100, tamano_inicial_pantalla_Y/2+60};
const struct Punto origen_msj_5 = {100, tamano_inicial_pantalla_Y/2+200};


void crear_cadena_dado_valor_4_digitos(int valor, char* buffer) {
	buffer[0] = '0' + (valor / 1000);			// Mil
    buffer[1] = '0' + ((valor / 100) % 10);		// Centena
    buffer[2] = '0' + ((valor / 10) % 10);		// Decena
    buffer[3] = '0' + (valor % 10);			// Unidad
	buffer[4] = '\0';							// Terminador nulo para cadena
}


void crear_cadena_dado_valor_3_digitos(int valor, char* buffer) {
	buffer[0] = '0';
    buffer[1] = '0' + (valor / 100);		// Centena
    buffer[2] = '0' + ((valor / 10) % 10);		// Decena
    buffer[3] = '0' + (valor % 10);			// Unidad
	buffer[4] = '\0';							// Terminador nulo para cadena
}

struct Palabra* crear_mensaje_puntuacion(int puntuacion, struct Punto origen) {
    char mensaje[12] = "XXXX POINTS";
    char buffer[5];
	crear_cadena_dado_valor_4_digitos(puntuacion, buffer);
	mensaje[0] = buffer[0];
	mensaje[1] = buffer[1];
	mensaje[2] = buffer[2];
	mensaje[3] = buffer[3];
	return crear_palabra_desde_cadena(mensaje, origen);
    
}