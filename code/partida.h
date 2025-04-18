/**
 * @file partida.h
 * @brief Contiene las funciones necesarias para manejar la partida y su estado
 */

#ifndef PARTIDA_H
#define PARTIDA_H

#include "lunar_lander.h"
#include "fisicas.h"
#include "transformaciones.h"
#include "../resources/nave.h"
#include "../resources/caracteres.h"

extern int combustible;

void dibujar_escena(HDC hdc);

void rotar_nave(uint8_t direccion);

void manejar_instante_partida();

void inicializar_partida();

void anyadirMoneda();

void insertar_monedas(int monedas);

void comenzarPartida();

void finalizarPartida();

void findeJuego();

void escalar_escena_partida(float factor_x, float factor_y);

void continuar_tras_aterrizaje_partida();

#endif // PARTIDA_H