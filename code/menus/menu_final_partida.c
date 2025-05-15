#include "menu_final_partida.h"

#include "auxiliares_menus.h"
#include "../dibujable.h"
#include "../transformaciones.h"
#include "../../data/variables_globales.h"

// Palabras para almacenar los mensajes a mostrar
struct Palabra* mensaje_1 = {0};
struct Palabra* mensaje_puntuacion = {0};


void generar_mensaje_final_partida(int puntuacion) {
	mensaje_puntuacion = crear_mensaje_puntuacion(puntuacion, origen_msj_4);
	mensaje_1 = crear_palabra_desde_cadena("X-", origen_msj_1);
	
}

void dibujar_mensajes_final_partida() {
	dibujar_palabra(mensaje_1);
	dibujar_palabra(mensaje_puntuacion);
}



