#include "auxiliares_menus.h"

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
