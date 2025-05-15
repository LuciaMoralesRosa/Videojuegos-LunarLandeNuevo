#include "cabecera_juego.h"

#include "auxiliares_menus.h"
#include "../dibujable.h"
#include "../palabra.h"
#include "../transformaciones.h"
#include "../../data/variables_globales.h"
#include "../../data/variables_juego.h"
#include "../../resources/caracteres.h"

// Escalado inicial de la cabecera (para que se vea a un tamaño acorde de la pantalla)
// Si se cambia el valor por defecto (0.7), se deben actualizar las posciones de
// las palabras dadas en inicializar_cabecera()
#define escalado_cabecera 0.7
#define MULTIPLICADOR_VELOCIDADES 10
// Palabras para las variables mostradas por cabecera
struct Palabra* score = {0};
struct Palabra* score_valor = {0};
struct Palabra* time = {0};
struct Palabra* time_valor = {0};
struct Palabra* fuel = {0};
struct Palabra* fuel_valor = {0};
struct Palabra* altitude = {0};
struct Palabra* altitude_valor = {0};
struct Palabra* horizontal_speed = {0};
struct Palabra* horizontal_speed_valor = {0};
struct Palabra* vertical_speed = {0};
struct Palabra* vertical_speed_valor = {0};

// Contador de minutos trasncurridos en la partida
int minutos = 0;
// Contador de segundos trasncurridos en la partida
int segundos = 0;



// ----------------------------- METODOS PRIVADOS ------------------------------


/**
 * @brief Actualiza los valores numéricos de combustible, altitud y velocidades.
 * 
 * Convierte los valores a texto, los recrea como palabras y les aplica escalado
 * para su correcta visualización en la cabecera.
 */
void actualizar_dibujables(void) {
	char buffer[5];
	if(combustible >= 30000) {
		crear_cadena_dado_valor_4_digitos(9999, buffer);
	}
	else if(combustible < 30000) {
		crear_cadena_dado_valor_4_digitos(combustible / 3, buffer);
	}
	else if (combustible < 3000) {
		crear_cadena_dado_valor_3_digitos(combustible / 3, buffer);
	}
	fuel_valor = crear_palabra_desde_cadena(buffer, fuel_valor -> origen); 

	int valor = (int)(velocidad_horizontal* MULTIPLICADOR_VELOCIDADES);
	if(valor < 0) {
		valor = valor * -1;
		crear_cadena_dado_valor_3_digitos(valor, buffer);
		buffer[0] = '-';
	}
	else{
		crear_cadena_dado_valor_3_digitos(valor, buffer);
		buffer[0] = '0';
	}
	horizontal_speed_valor = crear_palabra_desde_cadena(buffer, horizontal_speed_valor -> origen); 

	valor = (int)(velocidad_vertical * MULTIPLICADOR_VELOCIDADES);
	if(valor < 0) {
		valor = valor * -1;
		crear_cadena_dado_valor_3_digitos(valor, buffer);
		buffer[0] = '-';
	}
	else{
		crear_cadena_dado_valor_3_digitos(valor, buffer);
		buffer[0] = '0';
	}
	vertical_speed_valor = crear_palabra_desde_cadena(buffer, vertical_speed_valor -> origen); 

	int valor_altitud = tamano_inicial_pantalla_Y - pos_real_nave_y - 130;
	crear_cadena_dado_valor_4_digitos(valor_altitud, buffer);
	altitude_valor = crear_palabra_desde_cadena(buffer, altitude_valor -> origen); 
}


// ----------------------------- METODOS PUBLICOS ------------------------------
void inicializar_cabecera(void) {
	minutos = 0;
	segundos = 0;

	char buffer[5];

	float separacion_altura =  ALTURA_CARACTER_MAX + ALTURA_CARACTER_MAX / 2;
	float origen_x_izda = 30;
	float origen_x_izda_valores = 60;
	float origen_y = 30;
	score = crear_palabra_desde_cadena("P", (struct Punto) {origen_x_izda, origen_y});
	score_valor = crear_palabra_desde_cadena("0000", (struct Punto) {origen_x_izda_valores, origen_y});
	time = crear_palabra_desde_cadena("T", (struct Punto) {origen_x_izda, origen_y + separacion_altura});
	time_valor = crear_palabra_desde_cadena("00:00", (struct Punto) {origen_x_izda_valores, origen_y + separacion_altura});
	fuel = crear_palabra_desde_cadena("C", (struct Punto) {origen_x_izda, origen_y + separacion_altura * 2});
	if(combustible < 10000) {
		crear_cadena_dado_valor_4_digitos(combustible / 3, buffer);
	}
	else if (combustible < 1000) {
		crear_cadena_dado_valor_3_digitos(combustible / 3, buffer);
	}
	fuel_valor = crear_palabra_desde_cadena(buffer, (struct Punto) {origen_x_izda_valores, origen_y + separacion_altura * 2});

	origen_x_izda = tamano_inicial_pantalla_X - 130;
	origen_x_izda_valores = origen_x_izda + 50;
	altitude = crear_palabra_desde_cadena("A", (struct Punto) {origen_x_izda, origen_y});
	altitude_valor = crear_palabra_desde_cadena("0000", (struct Punto) {origen_x_izda_valores, origen_y});
	horizontal_speed = crear_palabra_desde_cadena("VH", (struct Punto) {origen_x_izda, origen_y + separacion_altura});
	horizontal_speed_valor = crear_palabra_desde_cadena("0000", (struct Punto) {origen_x_izda_valores, origen_y + separacion_altura});
	vertical_speed = crear_palabra_desde_cadena("VV", (struct Punto) {origen_x_izda, origen_y + separacion_altura * 2});
	vertical_speed_valor = crear_palabra_desde_cadena("0000", (struct Punto) {origen_x_izda_valores, origen_y + separacion_altura * 2});

}



void dibujar_cabecera(HDC hdc){
	actualizar_dibujables();
	dibujar_palabra(score, hdc);
	dibujar_palabra(score_valor, hdc);
	dibujar_palabra(time, hdc);
	dibujar_palabra(time_valor, hdc);
	dibujar_palabra(fuel, hdc);
	dibujar_palabra(fuel_valor, hdc);
	dibujar_palabra(altitude, hdc);
	dibujar_palabra(altitude_valor, hdc);
	dibujar_palabra(horizontal_speed, hdc);
	dibujar_palabra(horizontal_speed_valor, hdc);
	dibujar_palabra(vertical_speed, hdc);
	dibujar_palabra(vertical_speed_valor, hdc);
}

void actualizar_segundos_cabecera(void) {
	if(segundos == 59) {
		segundos = 0;
		minutos++;
	}
	else {
		segundos++;
	}
	char buffer[6];
	buffer[0] = '0' + ((minutos / 10) % 10);     // Decena
    buffer[1] = '0' + (minutos % 10);            // Unidad
	buffer[2] = ' ';
	buffer[3] = '0' + ((segundos / 10) % 10);     // Decena
    buffer[4] = '0' + (segundos % 10);            // Unidad
	buffer[5] = '\0';                          // Terminador nulo para cadena
	time_valor = crear_palabra_desde_cadena(buffer, time_valor -> origen);
}

void actualizar_puntuacion_cabecera(void) {
	char buffer[5];
	crear_cadena_dado_valor_4_digitos(puntuacion_partida, buffer);
	score_valor = crear_palabra_desde_cadena(buffer, score_valor -> origen); 
}
