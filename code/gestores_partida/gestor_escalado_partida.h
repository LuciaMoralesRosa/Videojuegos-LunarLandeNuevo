#ifndef GESTOR_ESCALADO_PARTIDA_H
#define GESTOR_ESCALADO_PARTIDA_H

#include "../dibujable.h"

void escalar_nave_partida(float factor_x, float factor_y);

void escalar_terreno_partida(float factor_x, float factor_y);

void escalar_terreno_partida_dado_punto(struct Punto punto, float factor_x, float factor_y);

void escalar_escena_gestor(float factor_x, float factor_y);


#endif // GESTOR_ESCALADO_PARTIDA_H