#include "nave.h"

// Tamaño de la nave: Horizontal = 22

const struct DibujableConstante Nave_Base = {
    CENTRO_CONST_NAVE,
    (struct Punto[]){
        {0, ALTURA_NAVE}, {1, ALTURA_NAVE}, {2, ALTURA_NAVE}, {3, 12}, {3, 10}, // 0-4
        {6, 10}, {4, 8}, {4, 2}, {6, 0}, {12, 0}, // 5-9
        {14, 2}, {14, 8}, {12, 10}, {15, 10}, {15, 12}, // 10-14
        {17, ALTURA_NAVE}, {16, ALTURA_NAVE}, {ANCHURA_NAVE, ALTURA_NAVE}, {6, 12}, {4, 16}, // 15-19
        {5, ALTURA_NAVE}, {12, 12}, {14, 16}, {13, ALTURA_NAVE} // 20-23
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {1, 3}, // Pata izq (3)
        {3, 4}, {4, 5}, {5, 12}, {12, 13}, {13, 14}, {14, 21}, {21, 18}, {18, 3}, // Base (8)
        {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}, {10, 11}, {11, 12}, // Cupula (7)
        {14, 15}, {16, 15}, {15, 17}, // Pata dcha (3)
        {18, 19}, {19, 20}, {21, 22}, {22, 23}, {19, 22} // Propulsor (5)
    },
    24,
    26
};

const struct DibujableConstante Nave_Propulsion_Maxima = {
    CENTRO_CONST_NAVE,
    (struct Punto[]){
        {4, 14}, // 19
        {14, 14}, // 22
        {9, 48} // Centro
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 0}
    },
    3,
    3
};

const struct DibujableConstante Nave_Propulsion_Media = {
    CENTRO_CONST_NAVE,
    (struct Punto[]){
        {4, 14}, // 19
        {14, 14}, // 22
        {9, 38} // Centro
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 0}
    },
    3,
    3
};

const struct DibujableConstante Nave_Propulsion_Minima = {
    CENTRO_CONST_NAVE,
    (struct Punto[]){
        {4, 14}, // 19
        {14, 14}, // 22
        {9, 28} // Centro
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 0}
    },
    3,
    3
};


const struct DibujableConstante Fragmento_Pata_Izda = {
    {0, 0},
    (struct Punto[]){
        {0, ALTURA_NAVE}, {1, ALTURA_NAVE}, {2, ALTURA_NAVE}, {3, 12}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {1, 3}, // Pata izq (3)
    },
    4,
    3
};


const struct DibujableConstante Fragmento_Pata_Dcha = {
    {0, 0},
    (struct Punto[]){
        {15, 12}, {17, ALTURA_NAVE}, {16, ALTURA_NAVE}, {ANCHURA_NAVE, ALTURA_NAVE},
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {1, 3}, // Pata dcha (3)
    },
    4,
    3
};

const struct DibujableConstante Fragmento_Cupula = {
    {0, 0},
    (struct Punto[]){
        {6, 10}, {4, 8}, {4, 2}, {6, 0}, {12, 0}, {14, 2}, {14, 8}, {12, 10}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 0}  
    },
    8,
    8
};

const struct DibujableConstante Fragmento_Base = {
    {0, 0},
    (struct Punto[]){
        {3, 12}, {3, 10}, {15, 10}, {15, 12}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 0}
    },
    4,
    4
};

const struct DibujableConstante Fragmento_Motor_Izda = {
    {0, 0},
    (struct Punto[]){
        {6, 12}, {4, 16}, {5, ALTURA_NAVE}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 0}
    },
    3,
    3
};

const struct DibujableConstante Fragmento_Motor_Dcha = {
    {0, 0},
    (struct Punto[]){
        {12, 12}, {14, 16}, {13, ALTURA_NAVE}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 0}
    },
    3,
    3
};

const struct DibujableConstante Fragmento_Motor_Base = {
    {0, 0},
    (struct Punto[]){
        {4, 16}, {14, 16}
    },
    (struct UnionAristas[]){
        {0, 1}
    },
    2,
    1
};
