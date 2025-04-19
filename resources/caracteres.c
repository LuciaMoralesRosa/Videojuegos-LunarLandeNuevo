#include "caracteres.h"

const struct DibujableConstante Letra_A_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, ALTURA_CARACTER_MAX}, {0, 5}, {5, 0},{ANCHURA_CARACTER_MAX, 5},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}, 
        {0, 9}, {ANCHURA_CARACTER_MAX, 9} // Puente A
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {5, 6}
    },
    7,
    5
};

const struct DibujableConstante Letra_B_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, ALTURA_CARACTER_MAX}, {0, 0}, {7, 0}, {ANCHURA_CARACTER_MAX, 2},
        {ANCHURA_CARACTER_MAX, 6}, {7, 8}, {ANCHURA_CARACTER_MAX, 10},
        {ANCHURA_CARACTER_MAX, 14}, {7, ALTURA_CARACTER_MAX}, {0, 8}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 0},
        {9, 5}
    },
    10,
    10
};

const struct DibujableConstante Letra_C_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}, {0, ALTURA_CARACTER_MAX}, 
        {0, 0}, {ANCHURA_CARACTER_MAX, 0}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}
    },
    4,
    3
};


const struct DibujableConstante Letra_D_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {4,0}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2 -3},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2 +3}, {4, ALTURA_CARACTER_MAX},
        {0, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 0}
    },
    6,
    6
};

const struct DibujableConstante Letra_E_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {ANCHURA_CARACTER_MAX, 0}, {0, 0}, {0, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX},
        {0, ALTURA_CARACTER_MAX/2}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {4, 5}
    },
    6,
    4
};

const struct DibujableConstante Letra_F_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {ANCHURA_CARACTER_MAX, 0},
        {0, ALTURA_CARACTER_MAX},
        {0, ALTURA_CARACTER_MAX/2}, {ANCHURA_CARACTER_MAX-2, ALTURA_CARACTER_MAX/2} 
    },
    (struct UnionAristas[]){
        {0, 1}, {0, 2}, {3, 4}
    },
    5,
    3
};

const struct DibujableConstante Letra_G_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {ANCHURA_CARACTER_MAX, 0}, {0, 0}, {0, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2}, {ANCHURA_CARACTER_MAX/2, ALTURA_CARACTER_MAX/2}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, 
    },
    6,
    5
};

const struct DibujableConstante Letra_H_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {0, ALTURA_CARACTER_MAX/2}, {0, ALTURA_CARACTER_MAX}, 
        {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX} 
    },
    (struct UnionAristas[]){
        {0, 2}, {3, 5}, {1, 4}
    },
    6,
    3
};

const struct DibujableConstante Letra_I_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {ANCHURA_CARACTER_MAX/2, 0}, {ANCHURA_CARACTER_MAX, 0}, 
        {0, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX/2, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}, 
    },
    (struct UnionAristas[]){
        {0, 2}, {3, 5}, {1, 4}
    },
    6,
    3
};

const struct DibujableConstante Letra_J_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
       {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}, {0, ALTURA_CARACTER_MAX},
       {0, ALTURA_CARACTER_MAX/2 + 2},
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3},
    },
    4,
    3
};

const struct DibujableConstante Letra_K_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
       {0, 0}, {0, ALTURA_CARACTER_MAX/2}, {0, ALTURA_CARACTER_MAX},
       {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 2}, {1, 3}, {1, 4},
    },
    5,
    3
};

const struct DibujableConstante Letra_L_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {0, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}
    },
    3,
    2
};

const struct DibujableConstante Letra_M_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {0, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX, 0}, 
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX/2, ALTURA_CARACTER_MAX/2}
    },
    (struct UnionAristas[]){
        {0, 1}, {2, 3}, {0, 4}, {2, 4}
    },
    5,
    4
};

const struct DibujableConstante Letra_N_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {0, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 1}, {0, 3}, {2, 3}
    },
    4,
    3
};

const struct DibujableConstante Letra_O_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {0, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX, 0}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 0}
    },
    4,
    4
};

const struct DibujableConstante Letra_P_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {0, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2},
        {0, ALTURA_CARACTER_MAX/2}  
    },
    (struct UnionAristas[]){
        {0, 1}, {0, 2}, {2, 3}, {3, 4}
    },
    5,
    4
};

const struct DibujableConstante Letra_Q_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {ANCHURA_CARACTER_MAX/2 - 3, 0}, {ANCHURA_CARACTER_MAX/2 + 3, 0},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2 - 3}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2 + 3},
        {ANCHURA_CARACTER_MAX/2 + 3, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX/2 - 3, ALTURA_CARACTER_MAX},
        {0, ALTURA_CARACTER_MAX/2 + 3}, {0, ALTURA_CARACTER_MAX/2 - 3},
        {ANCHURA_CARACTER_MAX/2 + 1, ALTURA_CARACTER_MAX/2 + 3}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 0}, {8, 9}
    },
    10,
    9
};

const struct DibujableConstante Letra_R_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {0, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2},
        {0, ALTURA_CARACTER_MAX/2}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}      
    },
    (struct UnionAristas[]){
        {0, 1}, {0, 2}, {2, 3}, {3, 4}, {4, 5}
    },
    6,
    5
};

const struct DibujableConstante Letra_S_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {ANCHURA_CARACTER_MAX, 0}, {0, 0}, {0, ALTURA_CARACTER_MAX/2}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}, {0, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5},
    },
    6,
    5
};

const struct DibujableConstante Letra_T_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {ANCHURA_CARACTER_MAX/2, 0}, {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX/2, ALTURA_CARACTER_MAX},
    },
    (struct UnionAristas[]){
        {0, 2}, {1, 3}
    },
    4,
    2
};

const struct DibujableConstante Letra_U_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {0, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX, 0}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}
    },
    4,
    3
};

const struct DibujableConstante Letra_V_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX/2, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 2}, {1, 2}
    },
    3,
    2
};

const struct DibujableConstante Letra_W_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, 
        {3, ALTURA_CARACTER_MAX},
        {5, ALTURA_CARACTER_MAX/2},
        {7, ALTURA_CARACTER_MAX},
        {10, 0}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}
    },
    5,
    4
};

const struct DibujableConstante Letra_X_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, 
        {ANCHURA_CARACTER_MAX, 0},
        {0, ALTURA_CARACTER_MAX},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX},
    },
    (struct UnionAristas[]){
        {0, 3}, {1, 2}
    },
    4,
    2
};

const struct DibujableConstante Letra_Y_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, 
        {ANCHURA_CARACTER_MAX, 0},
        {ANCHURA_CARACTER_MAX/2, ALTURA_CARACTER_MAX/2},
        {ANCHURA_CARACTER_MAX/2, ALTURA_CARACTER_MAX},
    },
    (struct UnionAristas[]){
        {0, 2}, {1, 2}, {2, 3}
    },
    4,
    3
};

const struct DibujableConstante Letra_Z_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, 
        {ANCHURA_CARACTER_MAX, 0},
        {0, ALTURA_CARACTER_MAX},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX},
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}
    },
    4,
    3
};

// Números

const struct DibujableConstante Numero_0_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {2, 0}, {ANCHURA_CARACTER_MAX - 2, 0}, {ANCHURA_CARACTER_MAX, 2},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX - 2},
        {ANCHURA_CARACTER_MAX - 2, ALTURA_CARACTER_MAX},
        {2, ALTURA_CARACTER_MAX}, {0, ALTURA_CARACTER_MAX - 2}, {0, 2}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 0}
    },
    8,
    8
};

const struct DibujableConstante Numero_1_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 1}
    },
    2,
    1
};

const struct DibujableConstante Numero_2_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2},
        {0, ALTURA_CARACTER_MAX/2}, {0, ALTURA_CARACTER_MAX}, 
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5},
    },
    6,
    5
};

const struct DibujableConstante Numero_3_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2},
        {0, ALTURA_CARACTER_MAX/2}, {0, ALTURA_CARACTER_MAX},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {4, 5}, {2, 5}
    },
    6,
    5
};

const struct DibujableConstante Numero_4_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {ANCHURA_CARACTER_MAX, 0}, 
        {0, ALTURA_CARACTER_MAX/2}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 2}, {1, 3}, {2, 3}, {1, 4}
    },
    5,
    4
};

const struct DibujableConstante Numero_5_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {ANCHURA_CARACTER_MAX, 0}, {0, 0}, {0, ALTURA_CARACTER_MAX/2},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}, {0, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5},
    },
    6,
    5
};

const struct DibujableConstante Numero_6_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {0, ALTURA_CARACTER_MAX}, {0, ALTURA_CARACTER_MAX/2},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 1}, {2, 3}, {3, 4}, {1, 4},
    },
    5,
    4
};

const struct DibujableConstante Numero_7_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}
    },
    3,
    2
};

const struct DibujableConstante Numero_8_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {0, ALTURA_CARACTER_MAX}, {0, ALTURA_CARACTER_MAX/2},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX},
        {ANCHURA_CARACTER_MAX, 0}
    },
    (struct UnionAristas[]){
        {0, 1}, {2, 3}, {3, 4}, {4, 5}, {5, 0}, {1, 4}
    },
    6,
    6
};

const struct DibujableConstante Numero_9_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2}, 
        {0, ALTURA_CARACTER_MAX/2}, {0, 0},
        {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}
    },
    6,
    4
};

// Simbolos
const struct DibujableConstante Simbolo_DosPuntos_Base  = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {ANCHURA_CARACTER_MAX/2, ALTURA_CARACTER_MAX/2 - 3},
        {ANCHURA_CARACTER_MAX/2, ALTURA_CARACTER_MAX/2 - 4},
        {ANCHURA_CARACTER_MAX/2 + 1, ALTURA_CARACTER_MAX/2 - 3},
        {ANCHURA_CARACTER_MAX/2 + 1, ALTURA_CARACTER_MAX/2 - 4}, 

        {ANCHURA_CARACTER_MAX/2, ALTURA_CARACTER_MAX/2 + 4},
        {ANCHURA_CARACTER_MAX/2, ALTURA_CARACTER_MAX/2 + 3},
        {ANCHURA_CARACTER_MAX/2 + 1, ALTURA_CARACTER_MAX/2 + 4},
        {ANCHURA_CARACTER_MAX/2 + 1, ALTURA_CARACTER_MAX/2 + 3}
    },
    (struct UnionAristas[]){
        {0,1}, {1,2}, {2,3}, {3,0},
        {4,5}, {5,6}, {6,7}, {7,4}	
    },
    8,
    8
};

const struct DibujableConstante Simbolo_Espacio_Base  = {
    CENTRO_CONST_CARACTER,
    NULL,
    NULL,
    0,
    0
};

const struct DibujableConstante Simbolo_Derecha_Base  = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, ALTURA_CARACTER_MAX/2 + 5},
        {0, ALTURA_CARACTER_MAX/2 - 5},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2}
    },
    (struct UnionAristas[]){
        {0,1}, {1,2}, {2,0}
    },
    3,
    3
};

const struct DibujableConstante Simbolo_Izquierda_Base  = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2 + 5},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2 - 5},
        {0, ALTURA_CARACTER_MAX/2}
    },
    (struct UnionAristas[]){
        {0,1}, {1,2}, {2,0}
    },
    3,
    3
};

const struct DibujableConstante Checkbox_Base  = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0},
        {ANCHURA_CARACTER_MAX*2, 0},
        {ANCHURA_CARACTER_MAX*2, ANCHURA_CARACTER_MAX*2-1},
        {0, ANCHURA_CARACTER_MAX*2-1}
    },
    (struct UnionAristas[]){
        {0,1}, {1,2}, {2, 3}, {3, 0}
    },
    4,
    4
};

const struct DibujableConstante Checkbox_Base_Check  = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0},
        {ANCHURA_CARACTER_MAX*2, 0},
        {ANCHURA_CARACTER_MAX*2, ANCHURA_CARACTER_MAX*2-1},
        {0, ANCHURA_CARACTER_MAX*2 -1}
    },
    (struct UnionAristas[]){
        {0,1}, {1,2}, {2, 3}, {3, 0}, {0, 2}, {1, 3}
    },
    4,
    6
};

const struct DibujableConstante Menos_Base  = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, ALTURA_CARACTER_MAX/2},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2}
    },
    (struct UnionAristas[]){
        {0,1}
    },
    2,
    1
};

const struct DibujableConstante Apostrofe_Base  = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {ANCHURA_CARACTER_MAX/2, 0},
        {2, 6}
    },
    (struct UnionAristas[]){
        {0,1}
    },
    2,
    1
};

const struct DibujableConstante Interrogante_Base  = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {2, 0}, {ANCHURA_CARACTER_MAX, 0}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX/2},
        {2, ALTURA_CARACTER_MAX/2}, {2, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0,1}, {1, 2}, {2, 3}, {3, 4}
    },
    5,
    4
};

const struct DibujableConstante Coma_Base  = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {4, 13},
        {0, ALTURA_CARACTER_MAX+2}
    },
    (struct UnionAristas[]){
        {0,1}
    },
    2,
    1
};