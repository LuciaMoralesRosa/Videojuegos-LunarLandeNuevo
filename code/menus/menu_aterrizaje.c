#include "menu_aterrizaje.h"

#include <stdio.h>

#include "auxiliares_menus.h"
#include "../transformaciones.h"
#include "../../data/variables_globales.h"

// Palabras que contienen los mensajes a mostrar
struct Palabra* primer_mensaje = {0};
struct Palabra* segundo_mensaje = {0};
struct Palabra* tercer_mensaje = {0};
struct Palabra* puntuacion_mensaje = {0};
struct Palabra* presione_espacio = {0};

// Contador para variar el mensaje al colisionar
int opcion_colision = 0;
// Contador para variar el mensaje al aterrizar bruscamente
int opcion_brusco = 0;
// Contador para variar el mensaje al aterrizar perfectamente
int opcion_perfecto = 0;

void generar_mensaje_aterrizaje(int puntuacion) {
	puntuacion_mensaje = crear_mensaje_puntuacion(puntuacion, origen_msj_4);
	presione_espacio = crear_palabra_desde_cadena("PRESIONE ESPACIO PARA SEGUIR JUGANDO", origen_msj_5);
	switch(tipo_aterrizaje) {
		case PERFECTO:
			switch (opcion_perfecto) {
			case 0:
				primer_mensaje = crear_palabra_desde_cadena("FELICIDADES", origen_msj_1);
				segundo_mensaje = crear_palabra_desde_cadena("HAS CONSEGUIDO UN ATERRIZAJE PERFECTO", origen_msj_2);
				tercer_mensaje = crear_palabra_desde_cadena(" ", origen_msj_3);
			break;
			default:
			break;
			}
			
		break;	
		case BRUSCO: 
			switch (opcion_brusco) {
			case 0:
				primer_mensaje = crear_palabra_desde_cadena("NO MUY ELEGANTE", origen_msj_1);
				segundo_mensaje = crear_palabra_desde_cadena("PERO EY, ESTAS VIVO - ESO ES LO CUENTA, SUPONGO", origen_msj_2);
				tercer_mensaje = crear_palabra_desde_cadena("PROBABLEMENTE CHAFASTE A ALGUNOS LUNARCITOS ESO SI", origen_msj_3);
			break;
			
			default:
				primer_mensaje = crear_palabra_desde_cadena("BRUSCO, PERO EN TIERRA", origen_msj_1);
				segundo_mensaje = crear_palabra_desde_cadena("AUNQUE DUDO QUE ALGUNA VEZ HAYAS ESCUCHADO LA PALABRA SUAVIDAD", origen_msj_2);
				tercer_mensaje = crear_palabra_desde_cadena("AH, ADIOS AL TREN DE ATERRIZAJE", origen_msj_3);
			break;
			}
		break;
		default: // colision
		switch(opcion_colision) {
			case 0:
				primer_mensaje = crear_palabra_desde_cadena("BIEN, YA SABES COMO NO HACERLO", origen_msj_1);
				segundo_mensaje = crear_palabra_desde_cadena("HABIAS LLEGADO A LEER EL MANUAL?", origen_msj_2);
				tercer_mensaje = crear_palabra_desde_cadena("CONSEJILLO: FRENA ANTES DE QUE LA LUNA TE HAGA HACERLO", origen_msj_3);
				opcion_colision++;
			break;
			case 1:
				primer_mensaje = crear_palabra_desde_cadena("TODAVIA CHOCANDO?", origen_msj_1);
				segundo_mensaje = crear_palabra_desde_cadena("INTENTA USAR EL FRENO", origen_msj_2);
				tercer_mensaje = crear_palabra_desde_cadena("O SIMPLEMENTE CIERRA LOS OJOS, DUDO QUE EL RESULTADO SEA PEOR", origen_msj_3);
				opcion_colision++;
			break;
	
			default:
				primer_mensaje = crear_palabra_desde_cadena("EN SERIO?", origen_msj_1);
				segundo_mensaje = crear_palabra_desde_cadena("DEJA DE DESTRUIR LA LUNA", origen_msj_2);
				tercer_mensaje = crear_palabra_desde_cadena("LA NASA VA A ACABAR PASANDOTE FACTURA A ESTE PASO", origen_msj_3);
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


void destruir_menu_aterrizaje() {
	destruir_palabra(primer_mensaje);
	destruir_palabra(segundo_mensaje);
	destruir_palabra(tercer_mensaje);
	destruir_palabra(puntuacion_mensaje);
	destruir_palabra(presione_espacio);
}