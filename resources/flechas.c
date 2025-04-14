#include "flechas.h"


// Flechas Verticales 
const struct DibujableConstante Flecha_Vertical_Arriba_Base = {
    CENTRO_CONST_FLECHA_VERTICAL,
    (struct Punto[]){
        {ANCHURA_FLECHA_HORIZONTAL-1/2, ALTURA_FLECHA_VERTICAL-1}, {ANCHURA_FLECHA_HORIZONTAL-1/2, 0},
        {ANCHURA_FLECHA_HORIZONTAL-1/2 -TAMAGNO_PUNTA_FLECHA, TAMAGNO_PUNTA_FLECHA},
        {ANCHURA_FLECHA_HORIZONTAL-1/2 +TAMAGNO_PUNTA_FLECHA, TAMAGNO_PUNTA_FLECHA}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {1, 2}, {2,3}
    },
    3,
    4
};

const struct DibujableConstante Flecha_Vertical_Abajo_Base = {
    CENTRO_CONST_FLECHA_VERTICAL,
    (struct Punto[]){
        {ANCHURA_FLECHA_HORIZONTAL-1/2, ALTURA_FLECHA_VERTICAL-1}, {ANCHURA_FLECHA_HORIZONTAL-1/2, 0},
    },
    (struct UnionAristas[]){
        {ANCHURA_FLECHA_HORIZONTAL-1/2 -TAMAGNO_PUNTA_FLECHA, ANCHURA_FLECHA_HORIZONTAL-1 - TAMAGNO_PUNTA_FLECHA},
        {ANCHURA_FLECHA_HORIZONTAL-1/2 +TAMAGNO_PUNTA_FLECHA, ANCHURA_FLECHA_HORIZONTAL-1 - TAMAGNO_PUNTA_FLECHA}
    },
    3,
    4
};

// Flechas Horizontales
const struct DibujableConstante Flecha_Horizontal_Derecha_Base = {
    CENTRO_CONST_FLECHA_HORIZONTAL,
    (struct Punto[]){
        {0, ALTURA_FLECHA_HORIZONTAL-1/2}, {ANCHURA_FLECHA_HORIZONTAL-1, ALTURA_FLECHA_HORIZONTAL-1/2},
        {ANCHURA_FLECHA_HORIZONTAL-1/2 -TAMAGNO_PUNTA_FLECHA, ANCHURA_FLECHA_HORIZONTAL-1 + TAMAGNO_PUNTA_FLECHA},
        {ANCHURA_FLECHA_HORIZONTAL-1/2 -TAMAGNO_PUNTA_FLECHA, ANCHURA_FLECHA_HORIZONTAL-1 - TAMAGNO_PUNTA_FLECHA}
        
    },
    (struct UnionAristas[]){
        {0, 1}, {0, 2}, {0, 2}, {2,3}
    },
    3,
    4
};

const struct DibujableConstante Flecha_Horizontal_Izquierda_Base = {
    CENTRO_CONST_FLECHA_HORIZONTAL,
    (struct Punto[]){
        {0, ALTURA_FLECHA_HORIZONTAL-1/2}, {ANCHURA_FLECHA_HORIZONTAL-1, ALTURA_FLECHA_HORIZONTAL-1/2},
        {TAMAGNO_PUNTA_FLECHA, ANCHURA_FLECHA_HORIZONTAL-1 + TAMAGNO_PUNTA_FLECHA},
        {TAMAGNO_PUNTA_FLECHA, ANCHURA_FLECHA_HORIZONTAL-1 - TAMAGNO_PUNTA_FLECHA}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {1, 2}, {2,3}
    },
    3,
    4
};