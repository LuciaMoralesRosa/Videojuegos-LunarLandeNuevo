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

#define MARCO_ZOOM 80
#define MARCO_SUPERIOR 80
#define MARCO_INFERIOR 110
#define MARCO_TERRENO 90



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
struct Dibujable* terreno_0 = NULL;
struct Dibujable* terreno_1 = NULL;
struct Plataforma* plataformas_1 = NULL;
struct Plataforma* plataformas_2 = NULL;
uint8_t numero_plataformas = 0;


static uint8_t fisicas = DESACTIVADAS;
static int traslacion_dibujables_por_borde_inferior = 0;
struct Punto posicion_nave_cuando_zoom = {0};


// 1 si el terreno_0 auxiliar esta a la izquierda, 0 si el terreno_0 auxiliar esta a la derecha
static uint8_t terreno_auxiliar_en_izda = 1;
// 0 si es terreno_0, 1 si es terreno_1
int terreno_a_colocar = 1;
// 0 si el auxiliar es terreno_0, 1 si el auxiliar es terreno_1 
int terreno_auxiliar = 1;


// Variable para contabilizar la subida de la nave y controlar el desplazamiento
// vertical del terreno_0
float desplazamiento_superior = 0;

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
		escalar_dibujable_en_escena_dados_ejes(plataformas_1[i].linea, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes(plataformas_2[i].linea, factor_x, factor_y);
		for(uint8_t j = 0; j < plataformas_1[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes(plataformas_1[i].palabra->letras[j], factor_x, factor_y);
		}
		for(uint8_t j = 0; j < plataformas_2[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes(plataformas_2[i].palabra->letras[j], factor_x, factor_y);
		}
	}
}

void escalar_terreno_partida_dado_punto(struct Punto punto, float factor_x, float factor_y){
	escalar_dibujable_en_escena_dados_ejes_y_punto(terreno_0, punto, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes_y_punto(terreno_1, punto, factor_x, factor_y);
	for(uint8_t i = 0; i < numero_plataformas; i++) {
		escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_1[i].linea, punto, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_2[i].linea, punto, factor_x, factor_y);
		for(uint8_t j = 0; j < plataformas_1[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_1[i].palabra->letras[j], punto, factor_x, factor_y);
		}
		for(uint8_t j = 0; j < plataformas_2[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_2[i].palabra->letras[j], punto, factor_x, factor_y);
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

	// Comprobar colision con el terreno_0
	if(hay_colision(nave->objeto, terreno_0, &arista_colision) || hay_colision(nave->objeto, terreno_1, &arista_colision)){
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
	dibujar_superficie_lunar(hdc, terreno_0, plataformas_1, numero_plataformas);
	dibujar_superficie_lunar(hdc, terreno_1, plataformas_2, numero_plataformas);
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


struct Punto gestionar_posicion_nave_marcos(struct Punto traslacion_nave, struct Punto posicion_provisional) {
	if((posicion_provisional.x < MARCO_ZOOM * factor_escalado) && (posicion_provisional.y < MARCO_SUPERIOR * factor_escalado)) {
		// Ir hacia arriba y derecha
		trasladar_superficie_lunar(terreno_0, plataformas_1, numero_plataformas, (struct Punto){-traslacion_nave.x, -traslacion_nave.y});
		trasladar_superficie_lunar(terreno_1, plataformas_2, numero_plataformas, (struct Punto){-traslacion_nave.x, -traslacion_nave.y});
		desplazamiento_superior = desplazamiento_superior + traslacion_nave.y;
		return (struct Punto){0, 0};
	}
	else if(posicion_provisional.x > (tamano_inicial_pantalla_X - MARCO_ZOOM) * factor_escalado && posicion_provisional.y < MARCO_SUPERIOR * factor_escalado) {
		// Ir hacia arriba e izquierda
		trasladar_superficie_lunar(terreno_0, plataformas_1, numero_plataformas, (struct Punto){-traslacion_nave.x, -traslacion_nave.y});
		trasladar_superficie_lunar(terreno_1, plataformas_2, numero_plataformas, (struct Punto){-traslacion_nave.x, -traslacion_nave.y});
		desplazamiento_superior = desplazamiento_superior + traslacion_nave.y;
		return (struct Punto){0, 0};
	}
	else if(posicion_provisional.x < MARCO_ZOOM * factor_escalado) {
		trasladar_superficie_lunar(terreno_0, plataformas_1, numero_plataformas, (struct Punto){-traslacion_nave.x, 0});
		trasladar_superficie_lunar(terreno_1, plataformas_2, numero_plataformas, (struct Punto){-traslacion_nave.x, 0});
		return (struct Punto){0, traslacion_nave.y};
	}
	else if(posicion_provisional.x > (tamano_inicial_pantalla_X - MARCO_ZOOM) * factor_escalado) {
		trasladar_superficie_lunar(terreno_0, plataformas_1, numero_plataformas, (struct Punto){-traslacion_nave.x, 0});
		trasladar_superficie_lunar(terreno_1, plataformas_2, numero_plataformas, (struct Punto){-traslacion_nave.x, 0});
		return (struct Punto){0, traslacion_nave.y};
	}
	else if(posicion_provisional.y < MARCO_SUPERIOR * factor_escalado) {
		desplazamiento_superior = desplazamiento_superior + traslacion_nave.y;
		trasladar_superficie_lunar(terreno_0, plataformas_1, numero_plataformas, (struct Punto){0, -traslacion_nave.y});
		trasladar_superficie_lunar(terreno_1, plataformas_2, numero_plataformas, (struct Punto){0, -traslacion_nave.y});
		return (struct Punto){traslacion_nave.x, 0};
	}
	else if(modo_zoom == DESACTIVADO && desplazamiento_superior < 0 && (posicion_provisional.y > MARCO_INFERIOR * factor_escalado)) {
		desplazamiento_superior = desplazamiento_superior + traslacion_nave.y;

		trasladar_superficie_lunar(terreno_0, plataformas_1, numero_plataformas, (struct Punto){0, -traslacion_nave.y});
		trasladar_superficie_lunar(terreno_1, plataformas_2, numero_plataformas, (struct Punto){0, -traslacion_nave.y});
		return (struct Punto){traslacion_nave.x, 0};
	}
	return traslacion_nave;
}

void gestionar_posicion_nave_terreno(struct Punto posicion_provisional) {
	int nave_en_terreno = (int)(pos_real_nave_x / tamano_inicial_pantalla_X*factor_escalado) % 2;
	int n_terreno = (int)(pos_real_nave_x / tamano_inicial_pantalla_X*factor_escalado);
	if(pos_real_nave_x < 0) {
		n_terreno--;
	} 
	//printf("Estoy en el terreno %d\n", n_terreno);
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
	}
	//printf("mi terreno auxiliar es el terreno %d\n", terreno_auxiliar);

	if(posicion_provisional.x < MARCO_TERRENO * factor_escalado) {
		// Si la nave esta proxima al lado izquierdo -> asegurar terreno auxilizar colocado al lado izquierdo
		if(terreno_auxiliar_en_izda == 0) {
			printf("\tMoviendo terreno 0 a izquierda\n\n");
			// Terreno no esta en la izquierda -> hay que colocarlo en la izquierda
			terreno_auxiliar_en_izda = 1;
			if(terreno_auxiliar == 0) {
				// Debo mover el terreno_0 original
				printf("\tMoviendo terreno 0 a izquierda\n\n");
				trasladar_superficie_lunar(terreno_0, plataformas_1, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
			}
			else {
				// Debo mover el terreno_1
				printf("\tMoviendo terreno 1 a izquierda\n");
				trasladar_superficie_lunar(terreno_1, plataformas_2, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
				printf("Hemos trasladado el terreno \n");
			}

		}
	}
	else if(posicion_provisional.x > (tamano_inicial_pantalla_X - MARCO_TERRENO) * factor_escalado) {
		// Si la nave esta proxima al lado derecho -> asegurar un terreno_0 colocado al lado derecho
		if(terreno_auxiliar_en_izda == 1) {
			// Terreno no en derecha -> hay que colocarlo en la derecha
			terreno_auxiliar_en_izda = 0;
			if(terreno_auxiliar == 0) {
				// Debo mover el terreno_0 original
				trasladar_superficie_lunar(terreno_0, plataformas_1, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
			}
			else {
				// Debo mover el terreno_1
				trasladar_superficie_lunar(terreno_1, plataformas_2, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
			}
		}
	}
}


void gestionar_zoom_aterrizaje(struct Punto traslacion_nave) {
	if(modo_zoom == DESACTIVADO) {
		if(hay_arista_en_radio_activar_zoom(nave->objeto->origen, terreno_0) || hay_arista_en_radio_activar_zoom(nave->objeto->origen, terreno_1)) {
			modo_zoom = ACTIVADO;
			posicion_nave_cuando_zoom = nave->objeto->origen;
			escalar_terreno_partida_dado_punto(nave->objeto->origen, entrada_modo_zoom_terreno, entrada_modo_zoom_terreno);
			escalar_nave_partida(entrada_modo_zoom_nave, entrada_modo_zoom_nave);
		}
	}
	else if(modo_zoom == ACTIVADO) {
		if(no_hay_arista_en_radio_desactivar_zoom(nave->objeto->origen, terreno_0) && no_hay_arista_en_radio_desactivar_zoom(nave->objeto->origen, terreno_1)) {
			modo_zoom = DESACTIVADO;
			trasladar_superficie_lunar(terreno_0, plataformas_1, numero_plataformas, (struct Punto){0, traslacion_dibujables_por_borde_inferior});
			trasladar_superficie_lunar(terreno_1, plataformas_2, numero_plataformas, (struct Punto){0, traslacion_dibujables_por_borde_inferior});
			trasladarDibujable(nave->objeto, (struct Punto){0, -traslacion_dibujables_por_borde_inferior});
			escalar_nave_partida(1/entrada_modo_zoom_nave, 1/entrada_modo_zoom_nave);
			escalar_terreno_partida_dado_punto(posicion_nave_cuando_zoom, 1/entrada_modo_zoom_terreno, 1/entrada_modo_zoom_terreno);
		}
	}
	if(modo_zoom == ACTIVADO){
		gestionar_colisiones();
		if(nave_proxima_a_borde_inferior(nave->objeto->origen)) {
			traslacion_dibujables_por_borde_inferior = traslacion_dibujables_por_borde_inferior + traslacion_nave.y;
			struct Punto nueva_traslacion_nave = {traslacion_nave.x, 0};
			trasladarDibujable(nave->objeto, nueva_traslacion_nave);
			struct Punto traslacion_terreno = {0, -traslacion_nave.y};
			trasladar_superficie_lunar(terreno_0, plataformas_1, numero_plataformas, traslacion_terreno);
			trasladar_superficie_lunar(terreno_1, plataformas_2, numero_plataformas, traslacion_terreno);
		}
		else {
			trasladarDibujable(nave->objeto, traslacion_nave);
		}
	}
	else {
		trasladarDibujable(nave->objeto, traslacion_nave);
	}
}

void manejar_instante_partida(){
    if(fisicas == ACTIVADAS) {
		struct Punto posible_traslacion_nave = calcularFisicas(nave);
		pos_real_nave_x += posible_traslacion_nave.x * factor_escalado;
		struct Punto posicion_provisional = nave->objeto->origen;
		trasladar_punto(&posicion_provisional, posible_traslacion_nave);
		struct Punto traslacion_nave = gestionar_posicion_nave_marcos(posible_traslacion_nave, posicion_provisional);
		gestionar_posicion_nave_terreno(posicion_provisional);
		gestionar_zoom_aterrizaje(traslacion_nave);
	}
}

void inicializar_partida(){
	inicializar_cabecera();
    combustible = 0;
	terreno_0 = crear_dibujable(&Terreno);
	terreno_1 = crear_dibujable(&Terreno);
	generar_plataformas(&plataformas_1, &plataformas_2, &Terreno, terreno_1->origen, &numero_plataformas);
	printf("bonificiador = %d", plataformas_1[0].bonificador);
	trasladar_superficie_lunar(terreno_0, plataformas_1, numero_plataformas, (struct Punto){0, 350});
	trasladar_superficie_lunar(terreno_1, plataformas_2, numero_plataformas, (struct Punto){-tamano_inicial_pantalla_X, 350});
	printf("Final inicializando\n\n");
}

void continuar_tras_aterrizaje_partida(){
	modo_zoom = DESACTIVADO;
	terreno_0 = crear_dibujable(&Terreno);
	terreno_1 = crear_dibujable(&Terreno);
	generar_plataformas(&plataformas_1, &plataformas_2, &Terreno, terreno_1->origen, &numero_plataformas);
	trasladar_superficie_lunar(terreno_0, plataformas_1, numero_plataformas, (struct Punto){0, 350});
	trasladar_superficie_lunar(terreno_1, plataformas_2, numero_plataformas, (struct Punto){-tamano_inicial_pantalla_X, 350});
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
    trasladarDibujable(nave -> objeto, (struct Punto){valor_inicial_nave_x, 80});
	pos_real_nave_x = valor_inicial_nave_x;

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