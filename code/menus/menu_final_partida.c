#include "menu_final_partida.h"

#include "auxiliares_menus.h"
#include "../dibujable.h"
#include "../transformaciones.h"
#include "../../data/variables_globales.h"

// Palabras para almacenar los mensajes a mostrar
struct Palabra* mensaje_1 = {0};
struct Palabra* mensaje_2 = {0};
struct Palabra* mensaje_3 = {0};
struct Palabra* mensaje_puntuacion = {0};


void generar_mensaje_final_partida(int puntuacion) {
	mensaje_puntuacion = crear_mensaje_puntuacion(puntuacion, origen_msj_4);
	mensaje_1 = crear_palabra_desde_cadena("LA AVENTURA TERMINA AQUI", origen_msj_1);
	mensaje_2 = crear_palabra_desde_cadena("TE HAS QUEDADO SIN COMBUSTIBLE", origen_msj_2);
	mensaje_3 = crear_palabra_desde_cadena("PULSA ESPACIO PARA COMENZAR OTRA AVENTURA", (struct Punto){100, tam_ventana_y - 200*factor_escalado});

	escalar_menu_final_partida(factor_escalado);
}

void dibujar_mensajes_final_partida(HDC hdc) {
	dibujar_palabra(mensaje_1, hdc);
	dibujar_palabra(mensaje_2, hdc);
	dibujar_palabra(mensaje_3, hdc);
	dibujar_palabra(mensaje_puntuacion, hdc);
}


void escalar_menu_final_partida(float factor){
	escalar_palabra_en_escena_dados_ejes(mensaje_1, factor, factor);
	escalar_palabra_en_escena_dados_ejes(mensaje_2, factor, factor);
	escalar_palabra_en_escena_dados_ejes(mensaje_3, factor, factor);
	escalar_palabra_en_escena_dados_ejes(mensaje_puntuacion, factor, factor);
}


void destruir_menu_final_partida() {
	destruir_palabra(mensaje_1);
	destruir_palabra(mensaje_2);
	destruir_palabra(mensaje_3);
	destruir_palabra(mensaje_puntuacion);
}
