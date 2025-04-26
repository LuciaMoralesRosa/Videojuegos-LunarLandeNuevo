/**
 * @file partida.h
 * @brief Contiene las funciones necesarias para manejar la partida y su estado
 */

#ifndef PARTIDA_H
#define PARTIDA_H

#include <windows.h>

#include "lunar_lander.h"
#include "fisicas.h"
#include "transformaciones.h"
#include "../resources/nave.h"
#include "../resources/caracteres.h"

void dibujar_escena(HDC hdc);

void rotar_nave(uint8_t direccion);

void manejar_instante_partida();

void inicializar_partida();

void insertar_monedas(int monedas);

void escalar_escena_partida(float factor_x, float factor_y);

void comenzar_nueva_ronda();

void destruir_partida();

#endif // PARTIDA_H