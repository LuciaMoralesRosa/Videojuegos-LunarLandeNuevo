#ifndef FLECHAS_H
#define FLECHAS_H

#include "../code/dibujable.h"
#include "../resources/caracteres.h"

#define ALTURA_FLECHA_VERTICAL ALTURA_CARACTER_MAX * 2
#define ANCHURA_FLECHA_VERTICAL ANCHURA_CARACTER_MAX
#define CENTRO_CONST_FLECHA_VERTICAL {ALTURA_FLECHA_VERTICAL/2, ANCHURA_FLECHA_VERTICAL/2}

#define ALTURA_FLECHA_HORIZONTAL ALTURA_CARACTER_MAX
#define ANCHURA_FLECHA_HORIZONTAL ANCHURA_CARACTER_MAX * 3
#define CENTRO_CONST_FLECHA_HORIZONTAL {ALTURA_FLECHA_HORIZONTAL/2, ANCHURA_FLECHA_HORIZONTAL/2}

#define TAMAGNO_PUNTA_FLECHA 4

extern const struct DibujableConstante Flecha_Vertical_Arriba_Base;
extern const struct DibujableConstante Flecha_Vertical_Abajo_Base;

extern const struct DibujableConstante Flecha_Horizontal_Derecha_Base;
extern const struct DibujableConstante Flecha_Horizontal_Izquierda_Base;

#endif // FLECHAS_H