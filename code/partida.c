#include "partida.h"
#include "plataformas.h"
#include "terreno.h"

#include "fragmentacion_nave.h"
#include "gestores_partida/gestor_colisiones.h"
#include "gestores_partida/gestor_zoom.h"
#include "gestores_partida/elementos_partida.h"
#include "gestores_partida/gestor_escalado_partida.h"

#include "../resources.h"
#include "../resources/nave.h"
#include "../resources/superficie_lunar.h"

#include "../data/variables_globales.h"
#include "../data/variables_juego.h"

#include "menus/cabecera_juego.h"
#include "menus/menu_aterrizaje.h"
#include "menus/menu_final_partida.h"

#pragma comment(lib, "winmm.lib")

#define fuel_por_moneda 1500
#define masa_nave 1000

#define aterrizaje_perfecto_vel 0.5
#define aterrizaje_brusco_vel 1.5
#define aterrizaje_perfecto_rot 5
#define aterrizaje_brusco_rot 10



#define MARCO_DESPLAZAMIENTO 150
#define MARCO_SUPERIOR 80
#define MARCO_INFERIOR 110
#define MARCO_TERRENO 90




// Indica si la nave ha cruzado los marcos del terreno
uint8_t nave_ha_entrado_a_centro_terreno = 1;

// 1 si el terreno_0 auxiliar esta a la izquierda, 0 si el terreno_0 auxiliar esta a la derecha
static uint8_t terreno_auxiliar_en_izda = 1;
// 0 si el auxiliar es terreno_0, 1 si el auxiliar es terreno_1 
int terreno_auxiliar = 1;


// Variable para contabilizar la subida de la nave y controlar el desplazamiento
// vertical del terreno_0
float desplazamiento_superior = 0;

void escalar_escena_partida(float factor_x, float factor_y) {
	escalar_escena_gestor(factor_x, factor_y);
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
			PlaySound(MAKEINTRESOURCE(IDR_SOUND_PERFECT), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
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
			PlaySound(MAKEINTRESOURCE(IDR_SOUND_FORCED), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
			printf("Aterrizaje brusco\n");
			puntuacion = 15 * bonificador;
			tipo_aterrizaje = BRUSCO;
		}
		else{
			// Colision
			printf("Colision\n");
			PlaySound(MAKEINTRESOURCE(IDR_SOUND_COLISION), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
			gestionar_nave_fragmentada(nave->velocidad[0], nave->velocidad[1], nave->objeto->origen);
			puntuacion = 5 * bonificador;
		}
	}
	else {
		// Colision
		printf("Colision\n");
		PlaySound(MAKEINTRESOURCE(IDR_SOUND_COLISION), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
		gestionar_nave_fragmentada(nave->velocidad[0], nave->velocidad[1], nave->objeto->origen);
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



void dibujar_escena(HDC hdc){
	dibujar_cabecera(hdc);
    dibujar_dibujable(hdc, nave -> objeto);	
	dibujar_superficie_lunar(hdc, terreno_0, plataformas_0, numero_plataformas);
	dibujar_superficie_lunar(hdc, terreno_1, plataformas_1, numero_plataformas);
	if(combustible > combustible_motor) {
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
}

void rotar_nave(uint8_t direccion){
	rotarDibujable(nave -> objeto, direccion);
	rotarDibujable(motor_debil, direccion);
	rotarDibujable(motor_medio, direccion);
	rotarDibujable(motor_fuerte, direccion);
}


struct Punto gestionar_posicion_nave_marcos(struct Punto traslacion_nave, struct Punto posicion_provisional_tras_fisicas) {
	struct Punto punto_return = traslacion_nave;
	if((posicion_provisional_tras_fisicas.x < MARCO_DESPLAZAMIENTO * factor_escalado) && (posicion_provisional_tras_fisicas.y < MARCO_SUPERIOR * factor_escalado)) {
		// Ir hacia arriba y derecha
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){-traslacion_nave.x, -traslacion_nave.y});
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){-traslacion_nave.x, -traslacion_nave.y});
		desplazamiento_superior = desplazamiento_superior + traslacion_nave.y;
		punto_return = (struct Punto) {0,0};
	}
	else if(posicion_provisional_tras_fisicas.x > (tamano_inicial_pantalla_X - MARCO_DESPLAZAMIENTO) * factor_escalado && posicion_provisional_tras_fisicas.y < MARCO_SUPERIOR * factor_escalado) {
		// Ir hacia arriba e izquierda
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){-traslacion_nave.x, -traslacion_nave.y});
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){-traslacion_nave.x, -traslacion_nave.y});
		desplazamiento_superior = desplazamiento_superior + traslacion_nave.y;
		punto_return = (struct Punto) {0,0};
	}
	else if(posicion_provisional_tras_fisicas.x < MARCO_DESPLAZAMIENTO * factor_escalado) {
		// Ir solo hacia la derecha
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){-traslacion_nave.x, 0});
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){-traslacion_nave.x, 0});
		punto_return = (struct Punto) {0, traslacion_nave.y};

	}
	else if(posicion_provisional_tras_fisicas.x > (tamano_inicial_pantalla_X - MARCO_DESPLAZAMIENTO) * factor_escalado) {
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){-traslacion_nave.x, 0});
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){-traslacion_nave.x, 0});
		punto_return = (struct Punto){0, traslacion_nave.y};
	}
	else if(posicion_provisional_tras_fisicas.y < MARCO_SUPERIOR * factor_escalado) {
		desplazamiento_superior = desplazamiento_superior + traslacion_nave.y;
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){0, -traslacion_nave.y});
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){0, -traslacion_nave.y});
		punto_return = (struct Punto){punto_return.x, 0};
	}
	if(modo_zoom == DESACTIVADO && desplazamiento_superior < 0 && (posicion_provisional_tras_fisicas.y > MARCO_INFERIOR * factor_escalado)) {
		desplazamiento_superior = desplazamiento_superior + traslacion_nave.y;
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){0, -traslacion_nave.y});
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){0, -traslacion_nave.y});
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
	}
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

/**
 * @brief Colocacion y gestion de los terrenos auxiliares dada la posicion de la
 * 		  nave
 */
void gestionar_terreno_auxiliar_segun_posicion_nave() {
	if(modo_zoom == ACTIVADO) {
		// Desactivar modo_zoom
		trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){0, traslacion_dibujables_por_borde_inferior});
		trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){0, traslacion_dibujables_por_borde_inferior});
		trasladarDibujable(nave->objeto, (struct Punto){0, -traslacion_dibujables_por_borde_inferior});
		escalar_nave_partida(1/entrada_modo_zoom_nave, 1/entrada_modo_zoom_nave);
		escalar_terreno_partida_dado_punto(posicion_nave_cuando_zoom, 1/entrada_modo_zoom_terreno, 1/entrada_modo_zoom_terreno);
	}

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
						trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
					}
					else {
						trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
					}
					terreno_auxiliar_en_izda = 0; // terreno_auxiliar en la derecha
				}
			}
			else if((tamano_inicial_pantalla_X * n_terreno + MARCO_TERRENO) * factor_escalado > pos_real_nave_x * factor_escalado) {
				// La nave ha pasado el limite izquierdo de su terreno y quiere cruzar al terreno siguiente (ie, a la izda)
				if(terreno_auxiliar_en_izda == 0) {
					// El terreno auxiliar esta a la derecha -> moverlo a la izda
					if(terreno_auxiliar == 1) {
						trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
					}
					else {
						trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
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
						trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
					}
					else {
						trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){((-2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
					}
					terreno_auxiliar_en_izda = 1; // terreno_auxiliar en la derecha
				}
			}
			else if(pos_real_nave_x * factor_escalado > (tamano_inicial_pantalla_X * (n_terreno + 1) - MARCO_TERRENO) * factor_escalado) {
				// La nave ha pasado el limite derecho de su terreno y quiere cruzar al terreno siguiente (ie, a la dcha)
				if(terreno_auxiliar_en_izda == 1) {
					// El terreno auxiliar esta a la izquierda -> moverlo a la derecha
					if(terreno_auxiliar == 1) {
						trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
					}
					else {
						trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){((2)*tamano_inicial_pantalla_X) * factor_escalado, 0});
					}
					terreno_auxiliar_en_izda = 0; // terreno_auxiliar en la derecha
				}
			}
		}
		else {
			comprobar_si_nave_entra_a_centro_dcha(n_terreno);
		}
	}
	if(modo_zoom == ACTIVADO) {
		// Reactivar modo_zoom
		escalar_terreno_partida_dado_punto(posicion_nave_cuando_zoom, entrada_modo_zoom_terreno, entrada_modo_zoom_terreno);
		escalar_nave_partida(entrada_modo_zoom_nave, entrada_modo_zoom_nave);
	}
}


void comprobar_posibles_colisiones() {
	int bonificador = 1;
	uint8_t es_arista_aterrizable = 0;
	int colision_detectada = gestionar_colisiones(&bonificador, &es_arista_aterrizable);
	if(colision_detectada == 1) {
		// Determinar tipo de aterrizaje y puntos conseguidos
		uint16_t puntos_conseguidos = evaluar_aterrizaje(bonificador, es_arista_aterrizable);
		puntuacion_partida += puntos_conseguidos;
		printf("Has conseguido %d puntos en este aterrizaje\n", puntos_conseguidos);
		se_ha_aterrizado(puntos_conseguidos);
	}
}




void manejar_instante_partida(){
	if(fisicas == ACTIVADAS) {
		struct Punto traslacion_por_fisicas = calcularFisicas(nave);
	//	pos_real_nave_x += traslacion_por_fisicas.x * factor_escalado;
		struct Punto posicion_provisional_tras_fisicas = nave->objeto->origen;
		trasladar_punto(&posicion_provisional_tras_fisicas, traslacion_por_fisicas);
		struct Punto traslacion_nave = gestionar_posicion_nave_marcos(traslacion_por_fisicas, posicion_provisional_tras_fisicas);
		gestionar_terreno_auxiliar_segun_posicion_nave(posicion_provisional_tras_fisicas);
		gestionar_zoom_aterrizaje(traslacion_nave);
		if(modo_zoom == ACTIVADO) {
			comprobar_posibles_colisiones();

		}
	}
}



void inicializar_componentes_partida(){
	printf("se está comenzando partida\n");
	terreno_0 = crear_dibujable(&Terreno);
	terreno_1 = crear_dibujable(&Terreno);
	generar_plataformas(&plataformas_0, &plataformas_1, &Terreno, terreno_1->origen, &numero_plataformas);
	trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){0, 350});
	trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){-tamano_inicial_pantalla_X, 350});
    nave = (struct objetoFisico*)malloc(sizeof(struct objetoFisico));
    nave -> objeto = crear_dibujable(&Nave_Base);
    nave -> velocidad[0] = 2;
    nave -> velocidad[1] = 0;
    nave -> aceleracion[0] = 0;
    nave -> aceleracion[1] = 0;
    nave -> masa = masa_nave;
	nave -> rotacion = 0;
    trasladarDibujable(nave -> objeto, (struct Punto){MARCO_DESPLAZAMIENTO + 1, 80});
	pos_real_nave_x = MARCO_DESPLAZAMIENTO + 1;

	motor_debil = crear_dibujable(&Nave_Propulsion_Minima);
	motor_medio = crear_dibujable(&Nave_Propulsion_Media);
	motor_fuerte = crear_dibujable(&Nave_Propulsion_Maxima);

    fisicas = ACTIVADAS;
	inicio = 1;
	escalar_escena_partida(factor_escalado, factor_escalado);
	printf("Combustible inicial: %d\n", combustible);
}

void inicializar_partida(){
	inicializar_cabecera();
	comenzar_nueva_ronda();
}

void comenzar_nueva_ronda(){
	nave_ha_entrado_a_centro_terreno = 1;
	terreno_auxiliar_en_izda = 1;
	terreno_auxiliar = 1;	
	pos_real_nave_x = valor_inicial_nave_x;
	modo_zoom = DESACTIVADO;
	inicializar_componentes_partida();
}

void insertar_monedas(int monedas) {
	combustible += monedas * fuel_por_moneda;
}

void finalizarPartida(){
    destruir_objeto_fisico(nave);
    nave -> objeto = NULL;
    fisicas = DESACTIVADAS;
}