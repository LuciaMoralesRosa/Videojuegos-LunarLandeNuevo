/**
 * @file fisicas.h
 * @brief Contiene las funciones necesarias para manejar las fisicas del juego
 */

#ifndef FISICAS_H
#define FISICAS_H

#include "dibujable.h"
#include "transformaciones.h"

#define intervalo_fisicas_ms 32
#define pixels_por_metro 32
#define gravedad_m_ms -0.00020625 // Aceleracion con la que cae la nave
#define propulsor_m_ms 0.001 // Potencia del motor para contrarrestar la gravedad y desplazarse
#define combustible_motor 1

struct objetoFisico {
    struct Dibujable* objeto;
    float velocidad[2];
    float aceleracion[2];
    int masa;
	int16_t rotacion;
};

void destruirObjetoFisico(struct objetoFisico* objeto);

void calcularFisicas(struct objetoFisico* objeto);

void propulsar();

void activar_propulsor();

void desactivar_propulsor();

void girar_izquierda();

void girar_derecha();

uint8_t obtener_propulsor();

#endif // FISICAS_H
