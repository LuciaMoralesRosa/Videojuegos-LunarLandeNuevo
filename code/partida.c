#include "partida.h"
#include "gestor_colisiones.h"
#include "gestor_plataformas.h"
#include "gestor_zoom.h"
#include "terreno.h"

#include "../resources/nave.h"
#include "../resources/superficie_lunar.h"

#include "../data/variables_globales.h"
#include "../data/variables_juego.h"

#include "menus/cabecera_juego.h"
#include "menus/menu_aterrizaje.h"
#include "menus/menu_final_partida.h"



#define fuel_por_moneda 1500
#define masa_nave 1000

#define aterrizaje_perfecto_vel 0.5
#define aterrizaje_brusco_vel 1
#define aterrizaje_perfecto_rot 5
#define aterrizaje_brusco_rot 10

#define entrada_modo_zoom_nave 1
#define entrada_modo_zoom_terreno 1.7


int inicio = 0;

/**
 * @brief Enumeración para el estado de las fisicas
 * 
 * DESACTIVADAS: 0
 * ACTIVADAS: 1
 */
enum fisicas {
    DESACTIVADAS,
    ACTIVADAS
};

struct objetoFisico* nave = NULL;
struct Dibujable* motor_debil = NULL;
struct Dibujable* motor_medio = NULL;
struct Dibujable* motor_fuerte = NULL;
struct Dibujable* terreno = NULL;
struct Plataforma* plataformas_partida = NULL;
uint8_t numero_plataformas = 0;

static uint8_t fisicas = DESACTIVADAS;
static int traslacion_dibujables_por_borde_inferior = 0;
struct Punto origen_terreno = {0};

void escalar_nave_partida(float factor_x, float factor_y){
	escalar_dibujable_en_escena_dados_ejes(motor_fuerte, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes(motor_medio, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes(motor_debil, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes(nave->objeto, factor_x, factor_y);
}

void escalar_terreno_partida(float factor_x, float factor_y) {
	escalar_dibujable_en_escena_dados_ejes(terreno, factor_x, factor_y);
	for(uint8_t i = 0; i < numero_plataformas; i++) {
		escalar_dibujable_en_escena_dados_ejes(plataformas_partida[i].linea, factor_x, factor_y);
		for(uint8_t j = 0; j < plataformas_partida[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes(plataformas_partida[i].palabra->letras[j], factor_x, factor_y);
		}
	}
}

void escalar_terreno_partida_dado_punto(struct Punto punto, float factor_x, float factor_y){
	escalar_dibujable_en_escena_dados_ejes_y_punto(terreno, punto, factor_x, factor_y);
	for(uint8_t i = 0; i < numero_plataformas; i++) {
		escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_partida[i].linea, punto, factor_x, factor_y);
		for(uint8_t j = 0; j < plataformas_partida[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_partida[i].palabra->letras[j], punto, factor_x, factor_y);
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
	tipo_aterrizaje = COLISION;
	if(es_arista_aterrizable == 1){
		if(nave->velocidad[1] > -aterrizaje_perfecto_vel &&
			(aterrizaje_perfecto_vel > nave->velocidad[0] &&
			nave->velocidad[0] > -aterrizaje_perfecto_vel) &&
			(nave->rotacion < aterrizaje_perfecto_rot ||
			nave->rotacion > 360 - aterrizaje_perfecto_rot)) {
			// Aterrizaje perfecto
			printf("Aterrizaje perfecto\n");
			puntuacion = 50 * bonificador;
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
			puntuacion = 15 * bonificador;
			tipo_aterrizaje = BRUSCO;
		}
		else{
			// Colision
			printf("Colision\n");
			puntuacion = 5 * bonificador;
		}
	}
	else {
		// Colision
		printf("Colision\n");
		puntuacion = 5 * bonificador;
	}
	
	return puntuacion;
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
}

void gestionar_colisiones() {
	struct Arista arista_colision = (struct Arista){0};
	uint8_t bonificador = 1;
	uint8_t es_arista_aterrizable = 0;
	uint8_t se_produce_colision = 0;

	// Comprobar colision con el terreno
	if(hay_colision(nave->objeto, terreno, &arista_colision)){
		se_produce_colision = 1;
		es_arista_aterrizable = es_horizontal(arista_colision);
		if(es_arista_aterrizable == 1){

			// Si hay colision con el terreno -> evaluar si ha sido colision con plataforma
			for(uint8_t i = 0; i < numero_plataformas; i++) {
				if(hay_colision(nave->objeto, plataformas_partida[i].linea, &arista_colision)) {
					// La colision ha sido con una plataforma
					bonificador = plataformas_partida[i].bonificador;
					break;
				}
			}
		}
	}
	if(se_produce_colision == 1){
		// Determinar tipo de aterrizaje y puntos conseguidos
		uint16_t puntos_conseguidos = evaluar_aterrizaje(bonificador, es_arista_aterrizable);
		puntuacion_partida += puntos_conseguidos;
		printf("Has conseguido %d puntos en este aterrizaje\n", puntos_conseguidos);
		se_ha_aterrizado(puntos_conseguidos);
			
	}
}


void dibujar_escena(HDC hdc){
	dibujar_cabecera(hdc);
    dibujar_dibujable(hdc, nave -> objeto);	
	trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){20, 0});
	dibujar_dibujable_terreno(hdc, terreno);
	for(uint8_t i = 0; i < numero_plataformas; i++){
		dibujar_plataforma(hdc, plataformas_partida[i]);
	}
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


void manejar_instante_partida(){
    if(fisicas == ACTIVADAS) {
		struct Punto nueva_posicion = calcularFisicas(nave);
		if(modo_zoom == DESACTIVADO) {
			if(hay_arista_en_radio_activar_zoom(nave->objeto->origen, terreno)) {
				// Activar zoom
				modo_zoom = ACTIVADO;
				origen_terreno = nave->objeto->origen;
				escalar_terreno_partida_dado_punto(nave->objeto->origen, entrada_modo_zoom_terreno, entrada_modo_zoom_terreno);
				escalar_nave_partida(entrada_modo_zoom_nave, entrada_modo_zoom_nave);
				//escalar_terreno_partida(entrada_modo_zoom_terreno, entrada_modo_zoom_terreno);
			}
		}
		else if(modo_zoom == ACTIVADO) {
			if(no_hay_arista_en_radio_desactivar_zoom(nave->objeto->origen, terreno)) {
				modo_zoom = DESACTIVADO;
				trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){0, traslacion_dibujables_por_borde_inferior});
				trasladarDibujable(nave->objeto, (struct Punto){0, -traslacion_dibujables_por_borde_inferior});
				escalar_nave_partida(1/entrada_modo_zoom_nave, 1/entrada_modo_zoom_nave);
				escalar_terreno_partida_dado_punto(origen_terreno, 1/entrada_modo_zoom_terreno, 1/entrada_modo_zoom_terreno);
			}
		}
		if(modo_zoom == ACTIVADO){
			gestionar_colisiones();
			if(nave_proxima_a_borde_inferior(nave->objeto->origen)) {
				printf("Estoy proximo al borde inferior\n\n");
				traslacion_dibujables_por_borde_inferior = traslacion_dibujables_por_borde_inferior + nueva_posicion.y;
				struct Punto nueva_traslacion_nave = {nueva_posicion.x, 0};
				trasladarDibujable(nave->objeto, nueva_traslacion_nave);
				struct Punto traslacion_terreno = {0, -nueva_posicion.y};
				trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, traslacion_terreno);
			}
			else {
				trasladarDibujable(nave->objeto, nueva_posicion);
			}
		}
		else {
			trasladarDibujable(nave->objeto, nueva_posicion);
		}
	}
}

void inicializar_partida(){
	inicializar_cabecera();
    combustible = 0;
	terreno = crear_dibujable(&Terreno);
	plataformas_partida = generar_plataformas(&Terreno, &numero_plataformas);
	trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){0, 350});
}

void continuar_tras_aterrizaje_partida(){
	modo_zoom = DESACTIVADO;
	terreno = crear_dibujable(&Terreno);
	plataformas_partida = generar_plataformas(&Terreno, &numero_plataformas);
	trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){0, 350});
}

void anyadirMoneda(){
    combustible += fuel_por_moneda;
}

void insertar_monedas(int monedas) {
	combustible += monedas * fuel_por_moneda;
}

void comenzarPartida(){
    nave = (struct objetoFisico*)malloc(sizeof(struct objetoFisico));
    nave -> objeto = crear_dibujable(&Nave_Base);
    nave -> velocidad[0] = 0;
    nave -> velocidad[1] = 0;
    nave -> aceleracion[0] = 0;
    nave -> aceleracion[1] = 0;
    nave -> masa = masa_nave;
	nave -> rotacion = 0;
    trasladarDibujable(nave -> objeto, (struct Punto){50, 50});

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

void findeJuego(){
    cambiar_estado(PEDIR);
}