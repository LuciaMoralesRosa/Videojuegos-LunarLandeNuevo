#ifndef TERRENO_H
#define TERRENO_H

#include <stdio.h>
#include "dibujable.h"
#include "gestor_plataformas.h"

void dibujar_cielo_estrellado(HDC hdc);

void dibujar_superficie_lunar(HDC hdc, struct Dibujable* dibujable, struct Plataforma* plataformas, uint8_t numero_plataformas);

#endif // TERRENO_H