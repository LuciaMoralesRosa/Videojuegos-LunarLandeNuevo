#include "texto.h"
#include "../resources/caracteres.h"
#include "transformaciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Texto* crear_texto(struct Punto origen) {
    struct Texto* texto = malloc(sizeof(struct Texto));
    if (!texto) {
        fprintf(stderr, "Error crítico: asignación de memoria fallida.\n");
        exit(EXIT_FAILURE);
    }
    texto->origen = origen;
    texto->num_caracteres = 0;
    texto->caracteres = NULL;  // Ahora se espera que sea un arreglo de punteros
    texto->factor_escalado_x = 1;
    texto->factor_escalado_y = 1;
    return texto;
}

void agregar_caracter(struct Texto* texto, const struct DibujableConstante* caracter) {
    // Incrementar el número de caracteres
    texto->num_caracteres++;
    
    // Redimensionar el arreglo para almacenar punteros a Dibujable
    struct Dibujable** temp = realloc(texto->caracteres,
                             texto->num_caracteres * sizeof(struct Dibujable*));
    if (!temp) {
        fprintf(stderr, "Error al redimensionar el arreglo de caracteres.\n");
        exit(EXIT_FAILURE);
    }
    texto->caracteres = temp;
    
    // Crear el objeto Dibujable correspondiente al carácter
    struct Dibujable* letraDibujable = crearDibujable(caracter);
    if (!letraDibujable) {
        fprintf(stderr, "Error al crear el objeto dibujable.\n");
        exit(EXIT_FAILURE);
    }
    
    // Calcular la posición para este carácter
    struct Punto nuevoOrigen = {
        texto->origen.x + (texto->num_caracteres - 1) * (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER),
        texto->origen.y
    };
    colocar_dibujable(letraDibujable, nuevoOrigen);
    
    // Almacenar el puntero al objeto recién creado en el arreglo
    texto->caracteres[texto->num_caracteres - 1] = letraDibujable;
}

int16_t calcular_centro_x_texto(struct Texto* texto) {
    if (!texto || !texto->caracteres) return -1;
    uint8_t media_texto = ((texto->num_caracteres * ANCHURA_CARACTER_MAX +
                            (texto->num_caracteres - 1) * SEPARACION_CARACTER) / 2);
    return (texto->origen.x - (ANCHURA_CARACTER_MAX / 2) + media_texto);
}

void dibujar_texto(struct Texto* texto, HDC hdc) {
    if (texto == NULL || texto->num_caracteres == 0) return;
    for (uint8_t i = 0; i < texto->num_caracteres; i++) {
        dibujarDibujable(hdc, texto->caracteres[i]);
    }
}

void destruir_texto(struct Texto* texto) {
    if (texto == NULL) return;
    if (texto->caracteres != NULL) {
        for (uint8_t i = 0; i < texto->num_caracteres; i++) {
            destruirDibujable(texto->caracteres[i]);
        }
        free(texto->caracteres);
    }
    free(texto);
}

struct Texto* crearTextoDesdeCadena(const char* cadena, struct Punto origen) {
    struct Texto* txt = crear_texto(origen);
    for (int i = 0; cadena[i] != '\0'; i++) {
        char c = cadena[i];
        switch (c) {
            case 'A': agregar_caracter(txt, &Letra_A_Base); break;
            case 'B': agregar_caracter(txt, &Letra_B_Base); break;
            case 'C': agregar_caracter(txt, &Letra_C_Base); break;
            case 'D': agregar_caracter(txt, &Letra_D_Base); break;
            case 'E': agregar_caracter(txt, &Letra_E_Base); break;
            case 'F': agregar_caracter(txt, &Letra_F_Base); break;
            case 'G': agregar_caracter(txt, &Letra_G_Base); break;
            case 'H': agregar_caracter(txt, &Letra_H_Base); break;
            case 'I': agregar_caracter(txt, &Letra_I_Base); break;
            case 'J': agregar_caracter(txt, &Letra_J_Base); break;
            case 'K': agregar_caracter(txt, &Letra_K_Base); break;
            case 'L': agregar_caracter(txt, &Letra_L_Base); break;
            case 'M': agregar_caracter(txt, &Letra_M_Base); break;
            case 'N': agregar_caracter(txt, &Letra_N_Base); break;
            case 'O': agregar_caracter(txt, &Letra_O_Base); break;
            case 'P': agregar_caracter(txt, &Letra_P_Base); break;
            case 'Q': agregar_caracter(txt, &Letra_Q_Base); break;
            case 'R': agregar_caracter(txt, &Letra_R_Base); break;
            case 'S': agregar_caracter(txt, &Letra_S_Base); break;
            case 'T': agregar_caracter(txt, &Letra_T_Base); break;
            case 'U': agregar_caracter(txt, &Letra_U_Base); break;
            case 'V': agregar_caracter(txt, &Letra_V_Base); break;
            case 'W': agregar_caracter(txt, &Letra_W_Base); break;
            case 'X': agregar_caracter(txt, &Letra_X_Base); break;
            case 'Y': agregar_caracter(txt, &Letra_Y_Base); break;
            case 'Z': agregar_caracter(txt, &Letra_Z_Base); break;
            case '0': agregar_caracter(txt, &Numero_0_Base); break;
            case '1': agregar_caracter(txt, &Numero_1_Base); break;
            case '2': agregar_caracter(txt, &Numero_2_Base); break;
            case '3': agregar_caracter(txt, &Numero_3_Base); break;
            case '4': agregar_caracter(txt, &Numero_4_Base); break;
            case '5': agregar_caracter(txt, &Numero_5_Base); break;
            case '6': agregar_caracter(txt, &Numero_6_Base); break;
            case '7': agregar_caracter(txt, &Numero_7_Base); break;
            case '8': agregar_caracter(txt, &Numero_8_Base); break;
            case '9': agregar_caracter(txt, &Numero_9_Base); break;
            case ' ': agregar_caracter(txt, &Simbolo_Espacio_Base); break;
            case ':': agregar_caracter(txt, &Simbolo_DosPuntos_Base); break;
            case '>': agregar_caracter(txt, &Simbolo_Derecha_Base); break;
            case '<': agregar_caracter(txt, &Simbolo_Izquierda_Base); break;
            // Agnadir mas simbolos
            default: break;
        }
    }
    return txt;
}