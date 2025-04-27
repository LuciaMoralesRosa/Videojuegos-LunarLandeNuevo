#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#include "partida.h"
#include "gestor_colisiones.h"
#include "gestor_plataformas.h"
#include "gestor_zoom.h"
#include "terreno.h"
#include "fragmentacion_nave.h"

#include "../resources.h"
#include "../resources/nave.h"
#include "../resources/superficie_lunar.h"

#include "../data/variables_globales.h"
#include "../data/variables_juego.h"

#include "menus/cabecera_juego.h"
#include "menus/menu_aterrizaje.h"
#include "menus/menu_final_partida.h"

#pragma comment(lib, "winmm.lib")


#define fuel_por_moneda 250
#define masa_nave 1000

#define aterrizaje_perfecto_vel 0.5
#define aterrizaje_brusco_vel 1.5
#define aterrizaje_perfecto_rot 5
#define aterrizaje_brusco_rot 10

#define entrada_modo_zoom_nave 1
#define entrada_modo_zoom_terreno 2.2

#define MARCO_DESPLAZAMIENTO 150
#define MARCO_SUPERIOR 80
#define MARCO_INFERIOR 110
#define MARCO_TERRENO 150

SOCKET server_socket = INVALID_SOCKET;
SOCKET client_socket = INVALID_SOCKET;
int puerto = 8080;
int tiempo_episodio = 0;

int inicio = 0;

struct objetoFisico* nave = NULL;
struct Dibujable* motor_debil = NULL;
struct Dibujable* motor_medio = NULL;
struct Dibujable* motor_fuerte = NULL;
struct Dibujable* terreno_0 = NULL;
struct Dibujable* terreno_1 = NULL;
struct Plataforma* plataformas_0 = NULL;
struct Plataforma* plataformas_1 = NULL;
uint8_t numero_plataformas = 0;


static int traslacion_dibujables_por_borde_inferior = 0;
struct Punto posicion_nave_cuando_zoom = {0};
uint8_t nave_proxima_borde_inferior = 0;
float traslacion_horizontal_terreno_cuando_zoom = 0;
// Indica si la nave ha cruzado los marcos del terreno
uint8_t nave_ha_entrado_a_centro_terreno = 1;

// 1 si el terreno_0 auxiliar esta a la izquierda, 0 si el terreno_0 auxiliar esta a la derecha
static uint8_t terreno_auxiliar_en_izda = 1;
// 0 si el auxiliar es terreno_0, 1 si el auxiliar es terreno_1 
int terreno_auxiliar = 1;


// Variable para contabilizar la subida de la nave y controlar el desplazamiento
// vertical del terreno_0
float desplazamiento_superior = 0;

struct Punto pos_real_nave_al_terminar_el_zoom = {0};

void escalar_nave_partida(float factor_x, float factor_y){
	escalar_dibujable_en_escena_dados_ejes(motor_fuerte, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes(motor_medio, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes(motor_debil, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes(nave->objeto, factor_x, factor_y);
}

void escalar_terreno_partida(float factor_x, float factor_y) {
	escalar_dibujable_en_escena_dados_ejes(terreno_0, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes(terreno_1, factor_x, factor_y);
	for(uint8_t i = 0; i < numero_plataformas; i++) {
		escalar_dibujable_en_escena_dados_ejes(plataformas_0[i].linea, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes(plataformas_1[i].linea, factor_x, factor_y);
		for(uint8_t j = 0; j < plataformas_0[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes(plataformas_0[i].palabra->letras[j], factor_x, factor_y);
		}
		for(uint8_t j = 0; j < plataformas_1[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes(plataformas_1[i].palabra->letras[j], factor_x, factor_y);
		}
	}
}

void escalar_terreno_partida_dado_punto(struct Punto punto, float factor_x, float factor_y){
	escalar_dibujable_en_escena_dados_ejes_y_punto(terreno_0, punto, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes_y_punto(terreno_1, punto, factor_x, factor_y);
	for(uint8_t i = 0; i < numero_plataformas; i++) {
		escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_0[i].linea, punto, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_1[i].linea, punto, factor_x, factor_y);
		for(uint8_t j = 0; j < plataformas_0[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_0[i].palabra->letras[j], punto, factor_x, factor_y);
		}
		for(uint8_t j = 0; j < plataformas_1[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_1[i].palabra->letras[j], punto, factor_x, factor_y);
		}
	}
}

void escalar_escena_partida(float factor_x, float factor_y){
	if(inicio == 1) {
		escalar_nave_partida(factor_x, factor_y);
		escalar_terreno_partida(factor_x, factor_y);
	}
}

uint16_t evaluar_aterrizaje(uint8_t bonificador, uint8_t es_arista_aterrizable){
	uint16_t puntuacion = 0;
	float velocidad_vertical_nave = nave->velocidad[1];
	float velocidad_horizontal_nave = nave->velocidad[0];
	if(velocidad_vertical_nave < 0) {
		// Si la velocidad vertical es negativa, se toma el valor absoluto
		velocidad_vertical_nave = -velocidad_vertical_nave;
	}
	if(velocidad_horizontal_nave < 0) {
		// Si la velocidad horizontal es negativa, se toma el valor absoluto
		velocidad_horizontal_nave = -velocidad_horizontal_nave;
	}
	velocidad_vertical_nave += 0.01; // Evitar division por cero
	velocidad_horizontal_nave += 0.01; // Evitar division por cero
	float velocidad_nave = velocidad_horizontal_nave * velocidad_vertical_nave;
	tipo_aterrizaje = COLISION;
	if(es_arista_aterrizable == 1){
		if(nave->velocidad[1] > -aterrizaje_perfecto_vel &&
			(aterrizaje_perfecto_vel > nave->velocidad[0] &&
			nave->velocidad[0] > -aterrizaje_perfecto_vel) &&
			(nave->rotacion < aterrizaje_perfecto_rot ||
			nave->rotacion > 360 - aterrizaje_perfecto_rot)) {
			// Aterrizaje perfecto
			
			printf("Aterrizaje perfecto\n");
			puntuacion = (50 * bonificador * 10) / velocidad_nave;
			combustible += 50;
			tipo_aterrizaje = PERFECTO;
		}
		else if(nave->velocidad[1] > -aterrizaje_brusco_vel &&
			(aterrizaje_brusco_vel > nave->velocidad[0] &&
			nave->velocidad[0] > -aterrizaje_brusco_vel) &&
			(nave->rotacion < aterrizaje_brusco_rot ||
			nave->rotacion > 360 - aterrizaje_brusco_rot)) {
			// Aterrizaje brusco
			
			printf("Aterrizaje brusco\n");
			puntuacion = 15 * bonificador * 10 / velocidad_nave;
			tipo_aterrizaje = BRUSCO;
		}
		else{
			// Colision
			printf("Colision\n");
			puntuacion = 5 * bonificador * 10 / velocidad_nave;
		}
	}
	else {
		// Colision
		printf("Colision\n");
		puntuacion = bonificador * 10 / velocidad_nave;
	}
	
	return puntuacion;
}

void iniciar_nueva_partida_tras_aterrizar_ia(){
	printf("DEBUG: Se ha aterrizado - Se va a continuar tras aterrizaje\n");
	continuar_tras_aterrizaje_partida();
	estado_actual = ESTADO_JUEGO;
	printf("DEBUG: Se ha aterrizado - Estado actual = JUEGO\n");
	comenzarPartida();
	escalar_escena_partida(factor_escalado, factor_escalado);
	printf("DEBUG: Se ha aterrizado - Se ha comenzado la partida y escalado la escena\n");
}

void se_ha_aterrizado(uint16_t puntos){
	nave->velocidad[0] = 0;
	nave->velocidad[1] = 0;
	nave->aceleracion[0] = 0;
	nave->aceleracion[1] = 0;
	fisicas = DESACTIVADAS;
	actualizar_puntuacion_cabecera();
	generar_mensaje_aterrizaje(puntos);
	estado_actual = ESTADO_ATERRIZAJE;
	enviar_puntuacion_final(puntos);
	iniciar_nueva_partida_tras_aterrizar_ia();
}

void gestionar_colisiones() {
	struct Arista arista_colision = (struct Arista){0};
	uint8_t bonificador = 1;
	uint8_t es_arista_aterrizable = 0;
	uint8_t colision_detectada = 0;

	// Comprobar colision con el primer terreno
	if(hay_colision(nave->objeto, terreno_0, &arista_colision)){
		colision_detectada = 1;
		es_arista_aterrizable = es_horizontal(arista_colision);
		if(es_arista_aterrizable == 1){
			// Si hay colision con el terreno_0 -> evaluar si ha sido colision con plataforma
			for(uint8_t i = 0; i < numero_plataformas; i++) {
				if(hay_colision(nave->objeto, plataformas_0[i].linea, &arista_colision)) {
					// La colision ha sido con una plataforma
					bonificador = plataformas_0[i].bonificador;
					break;
				}
			}
		}
	}
	// Si no hay con el primero, comprobar colison con el segundo terreno
	else if(hay_colision(nave->objeto, terreno_1, &arista_colision)){
		colision_detectada = 1;
		es_arista_aterrizable = es_horizontal(arista_colision);
		if(es_arista_aterrizable == 1){
			// Si hay colision con el terreno_0 -> evaluar si ha sido colision con plataforma
			for(uint8_t i = 0; i < numero_plataformas; i++) {
				if(hay_colision(nave->objeto, plataformas_1[i].linea, &arista_colision)) {
					// La colision ha sido con una plataforma
					bonificador = plataformas_1[i].bonificador;
					break;
				}
			}
		}
	}
	if(colision_detectada == 1) {
		// Determinar tipo de aterrizaje y puntos conseguidos
		uint16_t puntos_conseguidos = evaluar_aterrizaje(bonificador, es_arista_aterrizable);
		puntuacion_partida += puntos_conseguidos;
		printf("Has conseguido %d puntos en este aterrizaje\n", puntos_conseguidos);
		se_ha_aterrizado(puntos_conseguidos);
		tiempo_episodio = 0;
	}
	else if (tiempo_episodio >= 2000){
		// Cortar el episodio si no hay colision y ha pasado mucho tiempo (BUG)
		evaluar_aterrizaje(0, 0);
		printf("Has conseguido %d puntos en este aterrizaje\n", 0);
		se_ha_aterrizado(0);
		tiempo_episodio = 0;
	}
}

void pasar_tiempo_episodio(){
	tiempo_episodio++;
}

void dibujar_escena(HDC hdc){
	dibujar_cabecera(hdc);
    dibujar_dibujable(hdc, nave -> objeto);	
	dibujar_superficie_lunar(hdc, terreno_0, plataformas_0, numero_plataformas);
	dibujar_superficie_lunar(hdc, terreno_1, plataformas_1, numero_plataformas);
	switch(obtener_propulsor()){
		case 1:
			colocar_dibujable(motor_debil, nave -> objeto -> origen);
			dibujar_dibujable(hdc, motor_debil);
			break;
		case 2:
			colocar_dibujable(motor_medio, nave -> objeto -> origen);
			dibujar_dibujable(hdc, motor_medio);
			break;
		case 3:
			colocar_dibujable(motor_fuerte, nave -> objeto -> origen);
			dibujar_dibujable(hdc, motor_fuerte);
			break;
		default:
			break;
	}
	
}

void rotar_nave(uint8_t direccion){
	rotarDibujable(nave -> objeto, direccion);
	rotarDibujable(motor_debil, direccion);
	rotarDibujable(motor_medio, direccion);
	rotarDibujable(motor_fuerte, direccion);
}

// Traslacion del dibujable de la nave teniendo en cuenta el marco de desplazamiento
struct Punto gestionar_posicion_nave_marcos(struct Punto traslacion_fisicas, struct Punto posicion_provisional_tras_fisicas) {
	struct Punto punto_return = traslacion_fisicas;
	if((posicion_provisional_tras_fisicas.x < MARCO_DESPLAZAMIENTO * factor_escalado) && (posicion_provisional_tras_fisicas.y < MARCO_SUPERIOR * factor_escalado)) {
		// Ir hacia arriba y derecha
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){-traslacion_fisicas.x, -traslacion_fisicas.y});
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){-traslacion_fisicas.x, -traslacion_fisicas.y});
		desplazamiento_superior = desplazamiento_superior + traslacion_fisicas.y;
		punto_return = (struct Punto) {0,0};
	}
	else if(posicion_provisional_tras_fisicas.x > (tamano_inicial_pantalla_X - MARCO_DESPLAZAMIENTO) * factor_escalado && posicion_provisional_tras_fisicas.y < MARCO_SUPERIOR * factor_escalado) {
		// Ir hacia arriba e izquierda
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){-traslacion_fisicas.x, -traslacion_fisicas.y});
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){-traslacion_fisicas.x, -traslacion_fisicas.y});
		desplazamiento_superior = desplazamiento_superior + traslacion_fisicas.y;
		punto_return = (struct Punto) {0,0};
	}
	else if(posicion_provisional_tras_fisicas.x < MARCO_DESPLAZAMIENTO * factor_escalado) {
		// Ir solo hacia la derecha
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){-traslacion_fisicas.x, 0});
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){-traslacion_fisicas.x, 0});
		punto_return = (struct Punto) {0, traslacion_fisicas.y};
		if(modo_zoom == ACTIVADO) {
			traslacion_horizontal_terreno_cuando_zoom = traslacion_horizontal_terreno_cuando_zoom + traslacion_fisicas.x;	
		}
	}
	else if(posicion_provisional_tras_fisicas.x > (tamano_inicial_pantalla_X - MARCO_DESPLAZAMIENTO) * factor_escalado) {
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){-traslacion_fisicas.x, 0});
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){-traslacion_fisicas.x, 0});
		punto_return = (struct Punto){0, traslacion_fisicas.y};
		if(modo_zoom == ACTIVADO) {
			traslacion_horizontal_terreno_cuando_zoom = traslacion_horizontal_terreno_cuando_zoom + traslacion_fisicas.x;	
		}
	}
	else if(posicion_provisional_tras_fisicas.y < MARCO_SUPERIOR * factor_escalado) {
		desplazamiento_superior = desplazamiento_superior + traslacion_fisicas.y;
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){0, -traslacion_fisicas.y});
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){0, -traslacion_fisicas.y});
		punto_return = (struct Punto){punto_return.x, 0};
	}
	if(modo_zoom == DESACTIVADO && desplazamiento_superior < 0 && (posicion_provisional_tras_fisicas.y > MARCO_INFERIOR * factor_escalado)) {
		desplazamiento_superior = desplazamiento_superior + traslacion_fisicas.y;
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){0, -traslacion_fisicas.y});
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){0, -traslacion_fisicas.y});
		punto_return = (struct Punto){punto_return.x, 0};
	}
	return punto_return;
}

int establecer_terreno_auxiliar(int n_terreno) {
	int nave_en_terreno = (int)(pos_real_nave_x / tamano_inicial_pantalla_X*factor_escalado) % 2;
	if(pos_real_nave_x < 0) {
		n_terreno--;
	} 
	int terreno_auxiliar_inicial = terreno_auxiliar;
	if(pos_real_nave_x > 0) {
		if(nave_en_terreno == 0) {
			// La nave esta en el terreno original, ie, terreno 0
			terreno_auxiliar = 1;
		}
		else {
			terreno_auxiliar = 0;
		}
	}
	else {
		if(nave_en_terreno == 0) {
			// La nave esta en el terreno original, ie, terreno 0
			terreno_auxiliar = 0;
		}
		else {
			terreno_auxiliar = 1;
		}
	}
	if(terreno_auxiliar != terreno_auxiliar_inicial) {
		terreno_auxiliar_en_izda = terreno_auxiliar_en_izda == 0 ? 1 : 0;
		nave_ha_entrado_a_centro_terreno = 0;
		//printf("\n\nTerreno auxiliar a cambiado a terreno_%d\n\n", terreno_auxiliar);
	}
	//printf("Estoy en terreno %d, mi terreno auxiliar es %d y esta en %d\n", n_terreno, terreno_auxiliar, terreno_auxiliar_en_izda);
	return n_terreno;
}


void comprobar_si_nave_entra_a_centro_izda(int n_terreno) {
	float marco_derecho = (tamano_inicial_pantalla_X * (n_terreno + 1) - MARCO_TERRENO) * factor_escalado;
	float marco_izquierdo = (tamano_inicial_pantalla_X * n_terreno + MARCO_TERRENO) * factor_escalado;
	if(pos_real_nave_x * factor_escalado < marco_derecho && pos_real_nave_x * factor_escalado > marco_izquierdo) {
		nave_ha_entrado_a_centro_terreno = 1;
	}
}

void comprobar_si_nave_entra_a_centro_dcha(int n_terreno) {
	float marco_derecho = (tamano_inicial_pantalla_X * (n_terreno + 1) - MARCO_TERRENO) * factor_escalado;
	float marco_izquierdo = (tamano_inicial_pantalla_X * n_terreno + MARCO_TERRENO) * factor_escalado;
	if(pos_real_nave_x * factor_escalado < marco_derecho && pos_real_nave_x * factor_escalado > marco_izquierdo) {
		nave_ha_entrado_a_centro_terreno = 1;
	}
}

void activar_zoom() {			
	posicion_nave_cuando_zoom = nave->objeto->origen;
	escalar_terreno_partida_dado_punto(posicion_nave_cuando_zoom, entrada_modo_zoom_terreno, entrada_modo_zoom_terreno);
	escalar_nave_partida(entrada_modo_zoom_nave, entrada_modo_zoom_nave);
	traslacion_horizontal_terreno_cuando_zoom = 0;
	pos_real_nave_al_terminar_el_zoom = (struct Punto) {0,0};
}

void desactivar_zoom() {
	trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){traslacion_horizontal_terreno_cuando_zoom, traslacion_dibujables_por_borde_inferior});
	trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){traslacion_horizontal_terreno_cuando_zoom, traslacion_dibujables_por_borde_inferior});
	printf("Dibujable punto de entrada (%f, %f)\n", posicion_nave_cuando_zoom.x, posicion_nave_cuando_zoom.y);
	printf("Dibujable traslacion durante el zoom (%f, %f)\n", pos_real_nave_al_terminar_el_zoom.x, pos_real_nave_al_terminar_el_zoom.y);
	struct Punto traslacion_real_reescalada = (struct Punto) {
		pos_real_nave_al_terminar_el_zoom.x / entrada_modo_zoom_terreno,
		pos_real_nave_al_terminar_el_zoom.y / entrada_modo_zoom_terreno
	};
	struct Punto punto_real_con_traslacion = (struct Punto) {traslacion_real_reescalada.x + posicion_nave_cuando_zoom.x, traslacion_real_reescalada.y + posicion_nave_cuando_zoom.y};
	//trasladarDibujable(nave->objeto, (struct Punto){0, -traslacion_dibujables_por_borde_inferior});
	colocar_dibujable(nave->objeto, punto_real_con_traslacion);
	printf("Dibujable colocado en posicion (%f, %f)\n", nave->objeto->origen.x, nave->objeto->origen.y);
	escalar_nave_partida(1/entrada_modo_zoom_nave, 1/entrada_modo_zoom_nave);
	escalar_terreno_partida_dado_punto(posicion_nave_cuando_zoom, 1/entrada_modo_zoom_terreno, 1/entrada_modo_zoom_terreno);
	//trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){traslacion_horizontal_terreno_cuando_zoom / entrada_modo_zoom_terreno, 0});
	//trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){traslacion_horizontal_terreno_cuando_zoom / entrada_modo_zoom_terreno, 0});
}

void desactivar_zoom_terreno(int i) {
	float f = 1/entrada_modo_zoom_terreno;
	if(i == 0) {
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){0, traslacion_dibujables_por_borde_inferior});
		escalar_dibujable_en_escena_dados_ejes_y_punto(terreno_0, posicion_nave_cuando_zoom, f, f);
		for(uint8_t i = 0; i < numero_plataformas; i++) {
			escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_0[i].linea, posicion_nave_cuando_zoom, f, f);
			for(uint8_t j = 0; j < plataformas_0[i].palabra->num_letras; j++){
				escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_0[i].palabra->letras[j], posicion_nave_cuando_zoom, f, f);
			}
		}
	} 
	else {
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){0, traslacion_dibujables_por_borde_inferior});
		escalar_dibujable_en_escena_dados_ejes_y_punto(terreno_1, posicion_nave_cuando_zoom, f, f);
		for(uint8_t i = 0; i < numero_plataformas; i++) {
			escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_1[i].linea, posicion_nave_cuando_zoom, f, f);
			for(uint8_t j = 0; j < plataformas_1[i].palabra->num_letras; j++){
				escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_1[i].palabra->letras[j], posicion_nave_cuando_zoom, f, f);
			}
		}
	}
}

void reactivar_zoom_terreno(int i) {
	float f = entrada_modo_zoom_terreno;
	if(i == 0) {
		escalar_dibujable_en_escena_dados_ejes_y_punto(terreno_0, posicion_nave_cuando_zoom, entrada_modo_zoom_terreno, entrada_modo_zoom_terreno);
		for(uint8_t i = 0; i < numero_plataformas; i++) {
			escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_0[i].linea, posicion_nave_cuando_zoom, f, f);
			for(uint8_t j = 0; j < plataformas_0[i].palabra->num_letras; j++){
				escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_0[i].palabra->letras[j], posicion_nave_cuando_zoom, f, f);
			}
		}
	}
	else {
		escalar_dibujable_en_escena_dados_ejes_y_punto(terreno_1, posicion_nave_cuando_zoom, entrada_modo_zoom_terreno, entrada_modo_zoom_terreno);
		for(uint8_t i = 0; i < numero_plataformas; i++) {
			escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_1[i].linea, posicion_nave_cuando_zoom, f, f);
			for(uint8_t j = 0; j < plataformas_1[i].palabra->num_letras; j++){
				escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_1[i].palabra->letras[j], posicion_nave_cuando_zoom, f, f);
			}
		}
	}
}

void gestionar_posicion_nave_terreno() {
	int n_terreno = (int)(pos_real_nave_x / tamano_inicial_pantalla_X*factor_escalado);
	n_terreno = establecer_terreno_auxiliar(n_terreno);

	if(pos_real_nave_x < 0) {
		// La nave esta en terrenos a la izquierda del original
		if(nave_ha_entrado_a_centro_terreno == 1) {
			// La nave ya ha entrado al centro de este terreno
			if((tamano_inicial_pantalla_X * (n_terreno + 1) - MARCO_TERRENO) * factor_escalado < pos_real_nave_x * factor_escalado) {
				// la nave ha pasado el limite derecho de su terreno y quiere cruzar al terreno anterior (ie, a la dcha)
				if(terreno_auxiliar_en_izda == 1) {
					// El terreno auxiliar esta a la izquierda -> moverlo a la derecha
					if(terreno_auxiliar == 1) {
						if(modo_zoom == ACTIVADO) {
							printf("ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((2)*tamano_inicial_pantalla_X) * factor_escalado*entrada_modo_zoom_terreno);
							trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado * entrada_modo_zoom_terreno, 0});
						}
						else {
							printf("NO ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((2)*tamano_inicial_pantalla_X) * factor_escalado);
							trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
						}
					}
					else {
						if(modo_zoom == ACTIVADO) {
							printf("ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((2)*tamano_inicial_pantalla_X) * factor_escalado*entrada_modo_zoom_terreno);
							trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado * entrada_modo_zoom_terreno, 0});
						}
						else {
							printf("NO ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((2)*tamano_inicial_pantalla_X) * factor_escalado);
							trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
						}
					}
					terreno_auxiliar_en_izda = 0; // terreno_auxiliar en la derecha
				}
			}
			else if((tamano_inicial_pantalla_X * n_terreno + MARCO_TERRENO) * factor_escalado > pos_real_nave_x * factor_escalado) {
				// La nave ha pasado el limite izquierdo de su terreno y quiere cruzar al terreno siguiente (ie, a la izda)
				if(terreno_auxiliar_en_izda == 0) {
					// El terreno auxiliar esta a la derecha -> moverlo a la izda
					if(terreno_auxiliar == 1) {
						if(modo_zoom == ACTIVADO) {
							printf("ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((-2)*tamano_inicial_pantalla_X) * factor_escalado*entrada_modo_zoom_terreno);
							trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado * entrada_modo_zoom_terreno, 0});
						}
						else {
							printf("NO ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((-2)*tamano_inicial_pantalla_X) * factor_escalado);
							trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
						}
					}
					else {
						if(modo_zoom == ACTIVADO) {
							printf("ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((-2)*tamano_inicial_pantalla_X) * factor_escalado*entrada_modo_zoom_terreno);
							trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado*entrada_modo_zoom_terreno, 0});
						}
						else {
							printf("NO ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((-2)*tamano_inicial_pantalla_X) * factor_escalado);
							trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
						}
					}
					terreno_auxiliar_en_izda = 1; // terreno_auxiliar en la derecha
				}
			}
		}
		else {
			comprobar_si_nave_entra_a_centro_izda(n_terreno);
		}
	}
	else {
		// La nave esta en el terreno original o en terrenos a su derecha
		if(nave_ha_entrado_a_centro_terreno == 1) {
			if(pos_real_nave_x * factor_escalado < (tamano_inicial_pantalla_X * n_terreno + MARCO_TERRENO) * factor_escalado) {
				// La nave ha pasado el limite izquierdo de su terreno y quiere cruzar al terreno anterior (ie, a la izda)
				if(terreno_auxiliar_en_izda == 0) {
					// El terreno auxiliar esta a la derecha -> moverlo a la izda
					if(terreno_auxiliar == 1) {
						if(modo_zoom == ACTIVADO) {
							printf("ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((-2)*tamano_inicial_pantalla_X) * factor_escalado);
							desactivar_zoom();
							trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
							activar_zoom();
						}
						else {
							printf("NO ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((-2)*tamano_inicial_pantalla_X) * factor_escalado);
							trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
						}
					}
					else {
						if(modo_zoom == ACTIVADO) {
							printf("ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((-2)*tamano_inicial_pantalla_X) * factor_escalado);
							desactivar_zoom();
							trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
							activar_zoom();
						}
						else {
							printf("NO ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((-2)*tamano_inicial_pantalla_X) * factor_escalado);
							trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
						}
					}
					terreno_auxiliar_en_izda = 1; // terreno_auxiliar en la derecha
				}
			}
			else if(pos_real_nave_x * factor_escalado > (tamano_inicial_pantalla_X * (n_terreno + 1) - MARCO_TERRENO) * factor_escalado) {
				// La nave ha pasado el limite derecho de su terreno y quiere cruzar al terreno siguiente (ie, a la dcha)
				if(terreno_auxiliar_en_izda == 1) {
					// El terreno auxiliar esta a la izquierda -> moverlo a la derecha
					if(terreno_auxiliar == 1) {
						if(modo_zoom == ACTIVADO) {
							printf("ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((2)*tamano_inicial_pantalla_X) * factor_escalado);
							desactivar_zoom();
							trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
							activar_zoom();
						}
						else {
							printf("NO ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((2)*tamano_inicial_pantalla_X) * factor_escalado);
							trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
						}
					}
					else {
						if(modo_zoom == ACTIVADO) {
							printf("ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((2)*tamano_inicial_pantalla_X) * factor_escalado);
							desactivar_zoom();
							trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
							activar_zoom();
						}
						else {
							printf("NO ZOOM - El terreno se deberia mover %f\n\n\n\n\n\n", ((2)*tamano_inicial_pantalla_X) * factor_escalado);
							trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
						}
					}
					terreno_auxiliar_en_izda = 0; // terreno_auxiliar en la derecha
				}
			}
		}
		else {
			comprobar_si_nave_entra_a_centro_dcha(n_terreno);
		}
	}
}

void gestionar_zoom_aterrizaje(struct Punto traslacion_tras_marcos, struct Punto traslacion_real) {
	if(modo_zoom == DESACTIVADO) {
		if(hay_arista_en_radio_activar_zoom(nave->objeto->origen, terreno_0) || hay_arista_en_radio_activar_zoom(nave->objeto->origen, terreno_1)) {
			//printf("Estoy activando el zoom desde el gestor de zoom\n");
			modo_zoom = ACTIVADO;
			//posicion_nave_cuando_zoom = nave->objeto->origen;
			pos_real_nave_al_terminar_el_zoom = (struct Punto) {0,0};
			activar_zoom();
		}
	}
	else if(modo_zoom == ACTIVADO) {
		if(no_hay_arista_en_radio_desactivar_zoom(nave->objeto->origen, terreno_0) && no_hay_arista_en_radio_desactivar_zoom(nave->objeto->origen, terreno_1)) {
			//printf("Se ha desactivado desde else if en gestionar zoom\n");
			modo_zoom = DESACTIVADO;
			desactivar_zoom();
		}
	}
	if(modo_zoom == ACTIVADO){
		//printf("DEBUG: Modo zoom activado\n");
		gestionar_colisiones();
		pos_real_nave_x = pos_real_nave_x + traslacion_real.x / entrada_modo_zoom_terreno;
		pos_real_nave_y = pos_real_nave_y + traslacion_real.y / entrada_modo_zoom_terreno;
		pos_real_nave_al_terminar_el_zoom = (struct Punto) {pos_real_nave_al_terminar_el_zoom.x + traslacion_real.x, pos_real_nave_al_terminar_el_zoom.y + traslacion_real.y};
		nave_proxima_borde_inferior = nave_proxima_a_borde_inferior(nave->objeto->origen);
		if(nave_proxima_borde_inferior) {
			//printf("Estamos en zoom cerca del borde inferior\n");
			traslacion_dibujables_por_borde_inferior = traslacion_dibujables_por_borde_inferior + traslacion_tras_marcos.y;
			struct Punto nueva_traslacion_tras_marcos = {traslacion_tras_marcos.x, 0};
			trasladarDibujable(nave->objeto, nueva_traslacion_tras_marcos);
			struct Punto traslacion_terreno = {0, -traslacion_tras_marcos.y};
			trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, traslacion_terreno);
			trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, traslacion_terreno);
		}
		else {
			trasladarDibujable(nave->objeto, traslacion_tras_marcos);
		}
	}
	else {
		trasladarDibujable(nave->objeto, traslacion_tras_marcos);
		pos_real_nave_x = pos_real_nave_x + traslacion_real.x;
		pos_real_nave_y = pos_real_nave_y + traslacion_real.y;
	}
}

void manejar_instante_partida(){
	if(fisicas == ACTIVADAS) {
		struct Punto traslacion_calculada_con_fisicas = calcularFisicas(nave);

		// Obtener el punto donde estaria la nave aplicando la traslacion obtenida con las fisicas
		struct Punto posicion_provisional_tras_fisicas = nave->objeto->origen;
		trasladar_punto(&posicion_provisional_tras_fisicas, traslacion_calculada_con_fisicas);
		
		// Traslacion de la nave al tener en cuenta los bordes
		struct Punto traslacion_tras_marcos = gestionar_posicion_nave_marcos(traslacion_calculada_con_fisicas, posicion_provisional_tras_fisicas);
		gestionar_zoom_aterrizaje(traslacion_tras_marcos, traslacion_calculada_con_fisicas);

		// Una vez obtenida la posicion final de la nave, se gestiona el auxiliar
		gestionar_posicion_nave_terreno();
		enviar_estado_partida();
	}
}

void inicializar_partida(){
	inicializar_cabecera();
    combustible = 0;
	terreno_0 = crear_dibujable(&Terreno);
	terreno_1 = crear_dibujable(&Terreno);
	generar_plataformas(&plataformas_0, &plataformas_1, &Terreno, terreno_1->origen, &numero_plataformas);
	trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){0, 350});
	trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){-tamano_inicial_pantalla_X, 350});
}

void continuar_tras_aterrizaje_partida(){
	combustible = fuel_por_moneda;
	nave_ha_entrado_a_centro_terreno = 1;
	terreno_auxiliar_en_izda = 1;
	terreno_auxiliar = 1;	
	pos_real_nave_x = MARCO_DESPLAZAMIENTO + 1;
	pos_real_nave_y = MARCO_SUPERIOR + 1;

	modo_zoom = DESACTIVADO;
	terreno_0 = crear_dibujable(&Terreno);
	terreno_1 = crear_dibujable(&Terreno);
	printf("DEBUG: Continuar tras aterrizaje - Terrenos creados\n");
	generar_plataformas(&plataformas_0, &plataformas_1, &Terreno, terreno_1->origen, &numero_plataformas);
	printf("DEBUG: Continuar tras aterrizaje - Plataformas generadas\n");
	trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){0, 350});
	trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){-tamano_inicial_pantalla_X, 350});
	printf("DEBUG: Continuar tras aterrizaje - Terrenos trasladados\n");
}

void anyadirMoneda(){
    combustible += fuel_por_moneda;
}

void insertar_monedas(int monedas) {
	combustible += monedas * fuel_por_moneda;
}

void comenzarPartida(){
	combustible = fuel_por_moneda;
    nave = (struct objetoFisico*)malloc(sizeof(struct objetoFisico));
    nave -> objeto = crear_dibujable(&Nave_Base);
    nave -> velocidad[0] = 2;
    nave -> velocidad[1] = 0;
    nave -> aceleracion[0] = 0;
    nave -> aceleracion[1] = 0;
    nave -> masa = masa_nave;
	nave -> rotacion = 0;
    colocar_dibujable(nave -> objeto, (struct Punto){MARCO_DESPLAZAMIENTO + 1, MARCO_SUPERIOR + 1});
	pos_real_nave_x = MARCO_DESPLAZAMIENTO + 1;
	pos_real_nave_y = MARCO_SUPERIOR + 1;

	motor_debil = crear_dibujable(&Nave_Propulsion_Minima);
	motor_medio = crear_dibujable(&Nave_Propulsion_Media);
	motor_fuerte = crear_dibujable(&Nave_Propulsion_Maxima);

    fisicas = ACTIVADAS;
	inicio = 1;
	printf("Combustible inicial: %d\n", combustible);
}

void finalizarPartida(){
    destruir_objeto_fisico(nave);
    nave -> objeto = NULL;
    fisicas = DESACTIVADAS;
}

void iniciar_socket_servidor() {
    WSADATA wsaData;
    struct sockaddr_in server_addr;

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Error al iniciar Winsock\n");
        exit(EXIT_FAILURE);
    }

    // Crear socket
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        printf("Error al crear el socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Configurar dirección
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(puerto);

    // Asociar socket
    if (bind(server_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Error en bind: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Escuchar
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Error en listen: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Servidor esperando conexión en el puerto %d...\n", puerto);

    // Aceptar conexión entrante
    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == INVALID_SOCKET) {
        printf("Error en accept: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("¡Conexión establecida!\n");
}

void recibir_accion_ia(){
    // Creamos un buffer para recibir los datos
    int input_buffer[5];  // Array para almacenar los 5 estados de las teclas
    
    int len = recibir_datos((char *)input_buffer, sizeof(input_buffer));  // Función para recibir los datos
	if (len == sizeof(input_buffer)) {
		for (int i = 0; i < 5; i++) {
			if(i != MONEDA) {
				if(input_buffer[i] == 1) pulsar_tecla(i);
				else levantar_tecla(i);
			}
		}
	}
}

#pragma pack(push, 1) // Alinear la estructura a 1 byte
typedef struct {
    float velocidad[2];
    float aceleracion[2];
    float rotacion;
	float combustible;
    float posicion_nave[2];
    float puntos_terreno[280];
    float puntos_plataformas[8];
	float valor_plataformas[4];
} EstadoPartida;
#pragma pack(pop) // Restaurar alineación anterior

void enviar_estado_partida(){
	EstadoPartida estado_partida;
	
	// Definimos los datos del estado
	estado_partida.velocidad[0] = nave -> velocidad[0];
	estado_partida.velocidad[1] = nave -> velocidad[1];

	estado_partida.aceleracion[0] = nave -> aceleracion[0];
	estado_partida.aceleracion[1] = nave -> aceleracion[1];

	estado_partida.rotacion = nave -> rotacion;

	estado_partida.combustible = combustible;

	estado_partida.posicion_nave[0] = nave -> objeto -> origen.x;
	estado_partida.posicion_nave[1] = nave -> objeto -> origen.y;

	for(int i = 0; i < 140; i++){
		estado_partida.puntos_terreno[i] = terreno_0->puntos[i].x;
		estado_partida.puntos_terreno[i + 140] = terreno_0->puntos[i].y;
	}

	for(int i = 0; i < 4; i++){
		estado_partida.puntos_plataformas[i] = plataformas_0[i].linea -> puntos[0].x;
		estado_partida.puntos_plataformas[i + 4] = plataformas_0[i].linea -> puntos[0].y;
		estado_partida.valor_plataformas[i] = plataformas_0[i].bonificador;
	}

	// Creamos un buffer para enviar los datos
	uint8_t buffer[sizeof(estado_partida)];

	memcpy(buffer, &estado_partida, sizeof(estado_partida));

	// Ahora enviamos el buffer con todos los datos (esto depende de tu función de envío de datos)
	int len = enviar_datos((char *)buffer, sizeof(buffer));  // Esta es tu función para enviar los datos

	if (len != sizeof(buffer)) {
		printf("Error al enviar los datos de estado de la partida.\n");
	}
}

int enviar_datos(char *buffer, int len) {
	int total_enviados = 0;
	while (total_enviados < len) {
		int enviados = send(client_socket, buffer + total_enviados, len - total_enviados, 0);
		if (enviados == SOCKET_ERROR) {
			printf("Error al enviar datos: %d\n", WSAGetLastError());
			return -1;
		}
		total_enviados += enviados;
	}
	return total_enviados;
}

void enviar_puntuacion_final(float puntuacion_final) {
    int len = sizeof(puntuacion_final);
    if (enviar_datos((char *)&puntuacion_final, len) != len) {
        printf("Error al enviar la puntuación final.\n");
    } else {
        printf("Puntuación final enviada: %.2f\n", puntuacion_final);
    }
	Sleep(20);
}

int recibir_datos(char *buffer, int len) {
	int total_recibidos = 0;
	while (total_recibidos < len) {
		int recibidos = recv(client_socket, buffer + total_recibidos, len - total_recibidos, 0);
		if (recibidos == SOCKET_ERROR) {
			printf("Error al recibir datos: %d\n", WSAGetLastError());
			return -1;
		}
		if (recibidos == 0) break; // conexión cerrada
		total_recibidos += recibidos;
	}
	return total_recibidos;
}

void cerrar_socket() {
	closesocket(client_socket);
	closesocket(server_socket);
	WSACleanup();
}


