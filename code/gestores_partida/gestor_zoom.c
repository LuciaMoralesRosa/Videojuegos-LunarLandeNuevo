#include "gestor_zoom.h"
#include <math.h>
#include <stdio.h>

#include "elementos_partida.h"
#include "../../data/constantes.h"
#include "../../data/variables_globales.h"
#include "../../data/variables_juego.h"
#include "../transformaciones.h"
#include "gestor_escalado_partida.h"
#include "gestor_colisiones.h"

#define DISTANCIA_ACTIVAR_ZOOM 150.0f
#define DISTANCIA_DESACTIVAR_ZOOM 350.0f
#define DISTANCIA_BORDE_INFERIOR 200.0f

/**
 * @brief Devuelve la distancia mínima entre el punto P y el segmento AB
 */
float distancia_punto_a_segmento(struct Punto P, struct Punto A, struct Punto B) {
    float ABx = B.x - A.x;
    float ABy = B.y - A.y;
    float APx = P.x - A.x;
    float APy = P.y - A.y;

    float ab2 = ABx * ABx + ABy * ABy;
    float ap_ab = APx * ABx + APy * ABy;
    float t = ab2 == 0 ? 0 : ap_ab / ab2;

    if (t < 0.0f) t = 0.0f;
    else if (t > 1.0f) t = 1.0f;

    float closest_x = A.x + ABx * t;
    float closest_y = A.y + ABy * t;

    float dx = P.x - closest_x;
    float dy = P.y - closest_y;

    return sqrtf(dx * dx + dy * dy);
}

uint8_t hay_arista_en_radio_activar_zoom(struct Punto nave, struct Dibujable* terreno) {
	for (int i = 0; i < terreno->num_aristas; i++) {
        struct Arista arista = terreno->aristas[i];
        float d = distancia_punto_a_segmento(nave, *(arista.origen), *(arista.destino));
		if (d <= DISTANCIA_ACTIVAR_ZOOM) {
			return 1;
		}
    }
    return 0;
}


uint8_t no_hay_arista_en_radio_desactivar_zoom(struct Punto nave, struct Dibujable* terreno) {
	for (int i = 0; i < terreno->num_aristas; i++) {
        struct Arista arista = terreno->aristas[i];
        float d = distancia_punto_a_segmento(nave, *(arista.origen), *(arista.destino));
		if (d <= DISTANCIA_DESACTIVAR_ZOOM) {
			return 0;
		}
    }
    return 1;
}

uint8_t nave_proxima_a_borde_inferior(struct Punto nave) {
	if(nave.y >= tam_ventana_y - (DISTANCIA_BORDE_INFERIOR * factor_escalado)) {
		return 1;
	}
	else{
		return 0;
	}
}




void desactivar_zoom() {
	trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, (struct Punto){0, traslacion_dibujables_por_borde_inferior});
	trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, (struct Punto){0, traslacion_dibujables_por_borde_inferior});
	trasladarDibujable(nave->objeto, (struct Punto){0, -traslacion_dibujables_por_borde_inferior});
	escalar_nave_partida(1/entrada_modo_zoom_nave, 1/entrada_modo_zoom_nave);
	escalar_terreno_partida_dado_punto(posicion_nave_cuando_zoom, 1/entrada_modo_zoom_terreno, 1/entrada_modo_zoom_terreno);
}

void activar_zoom() {
	escalar_terreno_partida_dado_punto(posicion_nave_cuando_zoom, entrada_modo_zoom_terreno, entrada_modo_zoom_terreno);
	escalar_nave_partida(entrada_modo_zoom_nave, entrada_modo_zoom_nave);
}


void gestionar_zoom_aterrizaje(struct Punto traslacion_nave) {
	if(modo_zoom == DESACTIVADO) {
		if(hay_arista_en_radio_activar_zoom(nave->objeto->origen, terreno_0) || hay_arista_en_radio_activar_zoom(nave->objeto->origen, terreno_1)) {
			modo_zoom = ACTIVADO;
			posicion_nave_cuando_zoom = nave->objeto->origen;
			activar_zoom();
		}
	}
	else if(modo_zoom == ACTIVADO) {
		if(no_hay_arista_en_radio_desactivar_zoom(nave->objeto->origen, terreno_0) && no_hay_arista_en_radio_desactivar_zoom(nave->objeto->origen, terreno_1)) {
			modo_zoom = DESACTIVADO;
			desactivar_zoom();
		}
	}
	if(modo_zoom == ACTIVADO){
		if(nave_proxima_a_borde_inferior(nave->objeto->origen)) {
			traslacion_dibujables_por_borde_inferior = traslacion_dibujables_por_borde_inferior + traslacion_nave.y;
			struct Punto nueva_traslacion_nave = {traslacion_nave.x, 0};
			trasladarDibujable(nave->objeto, nueva_traslacion_nave);
			struct Punto traslacion_terreno = {0, -traslacion_nave.y};
			trasladar_superficie_lunar(terreno_0, plataformas_0, numero_plataformas, traslacion_terreno);
			trasladar_superficie_lunar(terreno_1, plataformas_1, numero_plataformas, traslacion_terreno);
		}
		else {
			trasladarDibujable(nave->objeto, traslacion_nave);
		}
	}
	else {
		trasladarDibujable(nave->objeto, traslacion_nave);
	}
}
