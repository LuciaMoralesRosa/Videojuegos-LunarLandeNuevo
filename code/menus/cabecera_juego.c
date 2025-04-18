#include "cabecera_juego.h"

#include "../dibujable.h"
#include "../palabra.h"
#include "../transformaciones.h"
#include "../../data/variables_globales.h"
#include "../../resources/caracteres.h"

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

void inicializar_cabecera(void) {
	float separacion_altura =  ALTURA_CARACTER_MAX + ALTURA_CARACTER_MAX / 2;
	float origen_x_izda = 20;
	float origen_x_izda_valores = 110;
	float origen_y = 20;
	score = crear_palabra_desde_cadena("SCORE", (struct Punto) {origen_x_izda, origen_y});
	score_valor = crear_palabra_desde_cadena("0000", (struct Punto) {origen_x_izda_valores, origen_y});
	time = crear_palabra_desde_cadena("TIME", (struct Punto) {origen_x_izda, origen_y + separacion_altura});
	time_valor = crear_palabra_desde_cadena("00:00", (struct Punto) {origen_x_izda_valores, origen_y + separacion_altura});
	fuel = crear_palabra_desde_cadena("FUEL", (struct Punto) {origen_x_izda, origen_y + separacion_altura * 2});
	fuel_valor = crear_palabra_desde_cadena("0000", (struct Punto) {origen_x_izda_valores, origen_y + separacion_altura * 2});

	origen_x_izda = tamano_inicial_pantalla_X + 100;
	origen_x_izda_valores = origen_x_izda + 270;
	altitude = crear_palabra_desde_cadena("ALTITUDE", (struct Punto) {origen_x_izda, origen_y});
	altitude_valor = crear_palabra_desde_cadena("0000", (struct Punto) {origen_x_izda_valores, origen_y});
	horizontal_speed = crear_palabra_desde_cadena("HORIZONTAL SPEED", (struct Punto) {origen_x_izda, origen_y + separacion_altura});
	horizontal_speed_valor = crear_palabra_desde_cadena("0000", (struct Punto) {origen_x_izda_valores, origen_y + separacion_altura});
	vertical_speed = crear_palabra_desde_cadena("VERTICAL SPEED", (struct Punto) {origen_x_izda, origen_y + separacion_altura * 2});
	vertical_speed_valor = crear_palabra_desde_cadena("0000", (struct Punto) {origen_x_izda_valores, origen_y + separacion_altura * 2});

	escalar_cabecera(factor_escalado);
	escalar_cabecera(0.7); // Reducir el tamaño de los dibujables
}

void escalar_cabecera(float factor) {
	escalar_palabra_en_escena_dados_ejes(score, factor, factor);
	escalar_palabra_en_escena_dados_ejes(score_valor, factor, factor);
	escalar_palabra_en_escena_dados_ejes(time, factor, factor);
	escalar_palabra_en_escena_dados_ejes(time_valor, factor, factor);
	escalar_palabra_en_escena_dados_ejes(fuel, factor, factor);
	escalar_palabra_en_escena_dados_ejes(fuel_valor, factor, factor);
	escalar_palabra_en_escena_dados_ejes(altitude, factor, factor);
	escalar_palabra_en_escena_dados_ejes(altitude_valor, factor, factor);
	escalar_palabra_en_escena_dados_ejes(horizontal_speed, factor, factor);
	escalar_palabra_en_escena_dados_ejes(horizontal_speed_valor, factor, factor);
	escalar_palabra_en_escena_dados_ejes(vertical_speed, factor, factor);
	escalar_palabra_en_escena_dados_ejes(vertical_speed_valor, factor, factor);
}

void dibujar_cabecera(HDC hdc){
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