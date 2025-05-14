#include "fisicas.h"

#include "partida.h"
#include "../data/variables_juego.h"
#include "../data/variables_globales.h"

static uint8_t propulsor_activado = 0;
static uint8_t propulsor = 0;
static uint8_t orden_girar_izquierda = 0;
static uint8_t orden_girar_derecha = 0;


struct Punto calcularFisicas(struct objetoFisico* elemento){
	if(orden_girar_izquierda && !orden_girar_derecha && (elemento -> rotacion <= 90 || elemento -> rotacion >= 270 + ANGULO_ROTACION)){
		elemento -> rotacion = (elemento -> rotacion - ANGULO_ROTACION + 360) % 360;
		rotar_nave(0);
	}
	else if (!orden_girar_izquierda && orden_girar_derecha && (elemento -> rotacion <= 90 - ANGULO_ROTACION || elemento -> rotacion >= 270)){
		elemento -> rotacion = (elemento -> rotacion + ANGULO_ROTACION) % 360;
		rotar_nave(1);
	}
	orden_girar_izquierda = 0;
	orden_girar_derecha = 0;

	if(propulsor && combustible >= combustible_motor){
		elemento -> aceleracion[0] += propulsor_m_ms * SIN_TABLA[elemento -> rotacion];
		elemento -> aceleracion[1] += propulsor_m_ms * COS_TABLA[elemento -> rotacion];
		combustible -= combustible_motor;
	}

    // Calculo de la aceleracion
	elemento -> aceleracion[1] += gravedad_m_ms;
	
    // Calculo de la velocidad en cada eje
	elemento -> velocidad[0] += elemento -> aceleracion[0] * intervalo_fisicas_ms;
	elemento -> velocidad[1] += elemento -> aceleracion[1] * intervalo_fisicas_ms;
	if(friccion_atmosfera_activada) {
		elemento -> velocidad[0] += -FRICCION_ATMOSFERA * elemento->velocidad[0];
		elemento -> velocidad[1] += -FRICCION_ATMOSFERA * elemento->velocidad[1];
	}
	
    // Calculo de la nueva posicion dadas las velocidades
    struct Punto nueva_posicion = {
        elemento -> velocidad[0] * intervalo_fisicas_ms / pixels_por_metro,
        -(elemento -> velocidad[1] * intervalo_fisicas_ms / pixels_por_metro)
    };

	// Obtencion de valores globales para la cabecera
	altitud = elemento->objeto->origen.y;
	velocidad_horizontal = elemento -> velocidad[0];
	velocidad_vertical = elemento -> velocidad[1];

	elemento -> aceleracion[0] = 0;
	elemento -> aceleracion[1] = 0;
	
	if(!propulsor_activado){
		switch(propulsor){
			case 0:
				break;
			default:
				propulsor--;
				break;
		}
	}

	return nueva_posicion;
}

void propulsar(void){
	switch(propulsor){
		case 3:
			propulsor--;
			break;
		default:{
			propulsor++;
			if(modo_ia_activado) {
				propulsor++;
			}
			break;
		}
	}
}

void activar_propulsor(void){
	propulsor_activado = 1;
}

void desactivar_propulsor(void){
	propulsor_activado = 0;
}

void girar_izquierda(void){
	orden_girar_izquierda = 1;
}

void girar_derecha(void){
	orden_girar_derecha = 1;
}

uint8_t obtener_propulsor(void){
	return propulsor;
}

void destruir_objeto_fisico(struct objetoFisico* objeto){
    destruir_dibujable(objeto -> objeto);
    free(objeto);
}