#include "gestor_zoom.h"
#include <math.h>
#include <stdio.h>

#include "../data/constantes.h"
#include "../data/variables_globales.h"

#define DISTANCIA_ACTIVAR_ZOOM 150.0f
#define DISTANCIA_DESACTIVAR_ZOOM 360.0f
#define DISTANCIA_BORDE_INFERIOR 200.0f

// Devuelve la distancia mínima entre el punto P y el segmento AB
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

uint8_t hay_arista_en_radio_zoom(uint8_t modo_zoom, struct Punto nave, struct Dibujable* terreno) {
	for (int i = 0; i < terreno->num_aristas; i++) {
        struct Arista arista = terreno->aristas[i];
        float d = distancia_punto_a_segmento(nave, *(arista.origen), *(arista.destino));
		if (modo_zoom == 0) {
			// Si esta el zoom activado 
			if (d <= DISTANCIA_DESACTIVAR_ZOOM) {
				return 1;
			}
		}
		else {
			if (d <= DISTANCIA_ACTIVAR_ZOOM) {
				return 1;
			}
		}
        
    }
    return 0;
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