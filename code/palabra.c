#include "palabra.h"

#include "../resources/caracteres.h"
#include "transformaciones.h"

struct Palabra* crear_palabra(struct Punto origen){
    struct Palabra* palabra = (struct Palabra*)malloc(sizeof(struct Palabra));
    if (!palabra) {
        fprintf(stderr, "Error crítico: asignación de memoria fallida.\n");
        exit(EXIT_FAILURE);
    }
    palabra->origen = origen;
    palabra->num_letras = 0;
    palabra->letras = NULL;
    palabra->factor_escalado_x = 1;
    palabra->factor_escalado_y = 1;
    return palabra;
}

void agregar_letra(struct Palabra* palabra, const struct DibujableConstante* letra) {
    // Aumentar el tamaño del arreglo de letras
    palabra->num_letras++;
    /*
    palabra->letras = (struct Dibujable*)realloc(palabra->letras,
                       palabra->num_letras * sizeof(struct Dibujable));
    
    // Definir como dibujable y ajustar posicion trasladando el origen
    struct Dibujable* letraDibujable = crearDibujable(letra);
    struct Punto nuevoOrigen = {palabra->origen.x + (palabra->num_letras - 1) *
                               (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER),
                               palabra->origen.y};
    colocar_dibujable(letraDibujable, nuevoOrigen);

    // Añadir la letra a la palabra
    palabra->letras[palabra->num_letras - 1] = *letraDibujable;
    */

    struct Dibujable** temporal = realloc(palabra->letras, palabra->num_letras*sizeof(struct Dibujable*));
    if (!temporal) {
        fprintf(stderr, "Error al redimensionar el arreglo de caracteres.\n");
        exit(EXIT_FAILURE);
    }
    palabra->letras = temporal;
    
    // Crear el objeto Dibujable correspondiente al carácter
    struct Dibujable* letraDibujable = crearDibujable(letra);
    if (!letraDibujable) {
        fprintf(stderr, "Error al crear el objeto dibujable.\n");
        exit(EXIT_FAILURE);
    }
    
    // Calcular la posición para este carácter
    struct Punto nuevoOrigen = {
        palabra->origen.x + (palabra->num_letras - 1) * (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER),
        palabra->origen.y
    };
    colocar_dibujable(letraDibujable, nuevoOrigen);
    
    // Almacenar el puntero al objeto recién creado en el arreglo
    palabra->letras[palabra->num_letras - 1] = letraDibujable;
}

int16_t calcular_centro_x_palabra(struct Palabra* palabra) {
    if(!palabra || !palabra->letras) return -1;
    // - el centro sera: (letras * ancho + letras-1 separacion)/2 - ancho/2
    uint8_t media_palabra = ((palabra->num_letras * ANCHURA_CARACTER_MAX + 
                            (palabra->num_letras - 1) * SEPARACION_CARACTER) / 2);
    
    return (palabra->origen.x - (ANCHURA_CARACTER_MAX / 2) + media_palabra);
}

void dibujar_palabra(struct Palabra* palabra, HDC hdc) {
    if (palabra == NULL || palabra->num_letras == 0) return;
    for (uint8_t i = 0; i < palabra->num_letras; i++) {
        dibujarDibujable(hdc, palabra->letras[i]);
    }
}

void destruir_palabra(struct Palabra* palabra) {
    if (palabra == NULL) return;
    if (palabra->letras != NULL) {
        for (uint8_t i = 0; i < palabra->num_letras; i++) {
            destruirDibujable(palabra->letras[i]);
        }
        free(palabra->letras);
    }
    free(palabra);
}

struct Palabra* crear_palabra_desde_cadena(const char* cadena, struct Punto origen) {
    struct Palabra* txt = crear_palabra(origen);
    for (int i = 0; cadena[i] != '\0'; i++) {
        char c = cadena[i];
        switch (c) {
            case 'A': agregar_letra(txt, &Letra_A_Base); break;
            case 'B': agregar_letra(txt, &Letra_B_Base); break;
            case 'C': agregar_letra(txt, &Letra_C_Base); break;
            case 'D': agregar_letra(txt, &Letra_D_Base); break;
            case 'E': agregar_letra(txt, &Letra_E_Base); break;
            case 'F': agregar_letra(txt, &Letra_F_Base); break;
            case 'G': agregar_letra(txt, &Letra_G_Base); break;
            case 'H': agregar_letra(txt, &Letra_H_Base); break;
            case 'I': agregar_letra(txt, &Letra_I_Base); break;
            case 'J': agregar_letra(txt, &Letra_J_Base); break;
            case 'K': agregar_letra(txt, &Letra_K_Base); break;
            case 'L': agregar_letra(txt, &Letra_L_Base); break;
            case 'M': agregar_letra(txt, &Letra_M_Base); break;
            case 'N': agregar_letra(txt, &Letra_N_Base); break;
            case 'O': agregar_letra(txt, &Letra_O_Base); break;
            case 'P': agregar_letra(txt, &Letra_P_Base); break;
            case 'Q': agregar_letra(txt, &Letra_Q_Base); break;
            case 'R': agregar_letra(txt, &Letra_R_Base); break;
            case 'S': agregar_letra(txt, &Letra_S_Base); break;
            case 'T': agregar_letra(txt, &Letra_T_Base); break;
            case 'U': agregar_letra(txt, &Letra_U_Base); break;
            case 'V': agregar_letra(txt, &Letra_V_Base); break;
            case 'W': agregar_letra(txt, &Letra_W_Base); break;
            case 'X': agregar_letra(txt, &Letra_X_Base); break;
            case 'Y': agregar_letra(txt, &Letra_Y_Base); break;
            case 'Z': agregar_letra(txt, &Letra_Z_Base); break;
            case '0': agregar_letra(txt, &Numero_0_Base); break;
            case '1': agregar_letra(txt, &Numero_1_Base); break;
            case '2': agregar_letra(txt, &Numero_2_Base); break;
            case '3': agregar_letra(txt, &Numero_3_Base); break;
            case '4': agregar_letra(txt, &Numero_4_Base); break;
            case '5': agregar_letra(txt, &Numero_5_Base); break;
            case '6': agregar_letra(txt, &Numero_6_Base); break;
            case '7': agregar_letra(txt, &Numero_7_Base); break;
            case '8': agregar_letra(txt, &Numero_8_Base); break;
            case '9': agregar_letra(txt, &Numero_9_Base); break;
            case ' ': agregar_letra(txt, &Simbolo_Espacio_Base); break;
            case ':': agregar_letra(txt, &Simbolo_DosPuntos_Base); break;
            case '>': agregar_letra(txt, &Simbolo_Derecha_Base); break;
            case '<': agregar_letra(txt, &Simbolo_Izquierda_Base); break;
            case '-': agregar_letra(txt, &Checkbox_Base); break;
            case '+': agregar_letra(txt, &Checkbox_Base_Check); break;
            // Agnadir mas simbolos
            default: break;
        }
    }
    return txt;
}