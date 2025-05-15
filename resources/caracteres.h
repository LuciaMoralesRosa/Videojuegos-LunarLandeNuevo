#ifndef CARACTERES_H
#define CARACTERES_H

#include "../code/dibujable.h"

#define ALTURA_CARACTER_MAX 16
#define ANCHURA_CARACTER_MAX 10 
#define SEPARACION_CARACTER 4
#define CENTRO_CONST_CARACTER {ANCHURA_CARACTER_MAX/2, ALTURA_CARACTER_MAX/2}

// Letras
extern const struct DibujableConstante Letra_A_Base;
extern const struct DibujableConstante Letra_C_Base;
extern const struct DibujableConstante Letra_H_Base;
extern const struct DibujableConstante Letra_P_Base;
extern const struct DibujableConstante Letra_T_Base;
extern const struct DibujableConstante Letra_V_Base;
extern const struct DibujableConstante Letra_X_Base;

// Números
extern const struct DibujableConstante Numero_0_Base;
extern const struct DibujableConstante Numero_1_Base;
extern const struct DibujableConstante Numero_2_Base;
extern const struct DibujableConstante Numero_3_Base;
extern const struct DibujableConstante Numero_4_Base;
extern const struct DibujableConstante Numero_5_Base;
extern const struct DibujableConstante Numero_6_Base;
extern const struct DibujableConstante Numero_7_Base;
extern const struct DibujableConstante Numero_8_Base;
extern const struct DibujableConstante Numero_9_Base;

// Simbolos
extern const struct DibujableConstante Simbolo_DosPuntos_Base;
extern const struct DibujableConstante Simbolo_Espacio_Base;
extern const struct DibujableConstante Menos_Base;

#endif // CARACTERES_H