#ifndef FRAGMENTACION_NAVE_H
#define FRAGMENTACION_NAVE_H

#include "dibujable.h"

void establecer_fragmentos_al_colisionar(float v_vertical, float v_horizontal, struct Punto punto_colision);

void inicializar_nave_fragmentada();

void fisicas_fragmentos();
void dibujar_fragmentos();

#endif // FRAGMENTACION_NAVE_H