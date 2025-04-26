#ifndef TERRENO_H
#define TERRENO_H

#include <stdio.h>
#include "dibujable.h"
#include "plataformas.h"


void dibujar_superficie_lunar(HDC hdc, struct Dibujable* dibujable, struct Plataforma* plataformas, uint8_t numero_plataformas);

#endif // TERRENO_H