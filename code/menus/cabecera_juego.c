#include "cabecera_juego.h"

#include "../dibujable.h"
#include "../palabra.h"
#include "../transformaciones.h"
#include "../../data/variables_globales.h"
#include "../../data/variables_juego.h"
#include "../../resources/caracteres.h"

#define escalado_cabecera 0.7

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

int minutos = 0;
int segundos = 0;

void crear_cadena_dado_valor(int valor, char* buffer) {
	buffer[0] = '0' + (valor / 1000);			// Mil
    buffer[1] = '0' + ((valor / 100) % 10);		// Centena
    buffer[2] = '0' + ((valor / 10) % 10);		// Decena
    buffer[3] = '0' + (valor % 10);			// Unidad
	buffer[4] = '\0';							// Terminador nulo para cadena
}


void crear_cadena_dado_valor_2(int valor, char* buffer) {
	buffer[0] = '0';
    buffer[1] = '0' + (valor / 100);		// Centena
    buffer[2] = '0' + ((valor / 10) % 10);		// Decena
    buffer[3] = '0' + (valor % 10);			// Unidad
	buffer[4] = '\0';							// Terminador nulo para cadena
}


void escalado_inicial(struct Palabra* palabra){
	escalar_palabra_en_escena_dados_ejes(palabra, factor_escalado, factor_escalado);
	escalar_palabra_en_escena_dados_ejes(palabra, escalado_cabecera, escalado_cabecera);
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
	buffer[2] = ':';
	buffer[3] = '0' + ((segundos / 10) % 10);     // Decena
    buffer[4] = '0' + (segundos % 10);            // Unidad
	buffer[5] = '\0';                          // Terminador nulo para cadena
	time_valor = crear_palabra_desde_cadena(buffer, time_valor->origen);
	escalado_inicial(time_valor);
}

void actualizar_puntuacion_cabecera(void) {
	char buffer[5];
	crear_cadena_dado_valor(puntuacion_partida, buffer);
	score_valor = crear_palabra_desde_cadena(buffer, score_valor->origen); 
	escalado_inicial(score_valor);

}

void actualizar_dibujables(void) {
	char buffer[5];
	
	crear_cadena_dado_valor(combustible, buffer);
	fuel_valor = crear_palabra_desde_cadena(buffer, fuel_valor->origen); 
	escalado_inicial(fuel_valor);

	int valor = (int)(velocidad_horizontal* 10);
	if(valor < 0) {
		valor = valor * -1;
		crear_cadena_dado_valor_2(valor, buffer);
		buffer[0] = '-';
	}
	else{
		crear_cadena_dado_valor_2(valor, buffer);
		buffer[0] = '0';
	}
	horizontal_speed_valor = crear_palabra_desde_cadena(buffer, horizontal_speed_valor->origen); 
	escalado_inicial(horizontal_speed_valor);

	valor = (int)(velocidad_vertical * 10);
	if(valor < 0) {
		valor = valor * -1;
		crear_cadena_dado_valor_2(valor, buffer);
		buffer[0] = '-';
	}
	else{
		crear_cadena_dado_valor_2(valor, buffer);
		buffer[0] = '0';
	}
	vertical_speed_valor = crear_palabra_desde_cadena(buffer, vertical_speed_valor->origen); 
	escalado_inicial(vertical_speed_valor);

	int valor_altitud = tamano_inicial_pantalla_Y - altitud;
	crear_cadena_dado_valor(valor_altitud, buffer);
	altitude_valor = crear_palabra_desde_cadena(buffer, altitude_valor->origen); 
	escalado_inicial(altitude_valor);
	
}


void inicializar_cabecera(void) {
	minutos = 0;
	segundos = 0;

	char buffer[5];

	float separacion_altura =  ALTURA_CARACTER_MAX + ALTURA_CARACTER_MAX / 2;
	float origen_x_izda = 20;
	float origen_x_izda_valores = 110;
	float origen_y = 20;
	score = crear_palabra_desde_cadena("SCORE", (struct Punto) {origen_x_izda, origen_y});
	score_valor = crear_palabra_desde_cadena("0000", (struct Punto) {origen_x_izda_valores, origen_y});
	time = crear_palabra_desde_cadena("TIME", (struct Punto) {origen_x_izda, origen_y + separacion_altura});
	time_valor = crear_palabra_desde_cadena("00:00", (struct Punto) {origen_x_izda_valores, origen_y + separacion_altura});
	fuel = crear_palabra_desde_cadena("FUEL", (struct Punto) {origen_x_izda, origen_y + separacion_altura * 2});
	crear_cadena_dado_valor(combustible, buffer);
	fuel_valor = crear_palabra_desde_cadena(buffer, (struct Punto) {origen_x_izda_valores, origen_y + separacion_altura * 2});

	origen_x_izda = tamano_inicial_pantalla_X + 100;
	origen_x_izda_valores = origen_x_izda + 270;
	altitude = crear_palabra_desde_cadena("ALTITUDE", (struct Punto) {origen_x_izda, origen_y});
	altitude_valor = crear_palabra_desde_cadena("0000", (struct Punto) {origen_x_izda_valores, origen_y});
	horizontal_speed = crear_palabra_desde_cadena("HORIZONTAL SPEED", (struct Punto) {origen_x_izda, origen_y + separacion_altura});
	horizontal_speed_valor = crear_palabra_desde_cadena("0000", (struct Punto) {origen_x_izda_valores, origen_y + separacion_altura});
	vertical_speed = crear_palabra_desde_cadena("VERTICAL SPEED", (struct Punto) {origen_x_izda, origen_y + separacion_altura * 2});
	vertical_speed_valor = crear_palabra_desde_cadena("0000", (struct Punto) {origen_x_izda_valores, origen_y + separacion_altura * 2});

	escalar_cabecera(factor_escalado);
	escalar_cabecera(escalado_cabecera); // Reducir el tamaño de los dibujables
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