#include "palabra.h"

#include "../resources/caracteres.h"
#include "transformaciones.h"

struct Palabra* crear_palabra(struct Punto origen){
    struct Palabra* palabra = (struct Palabra*)malloc(sizeof(struct Palabra));
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
}

int16_t calcular_centro_x_palabra(struct Palabra* palabra) {
    if(!palabra || !palabra->letras) return -1;
    // - el centro sera: (letras * ancho + letras-1 separacion)/2 - ancho/2
    uint8_t media_palabra = ((palabra->num_letras * ANCHURA_CARACTER_MAX + 
                            (palabra->num_letras - 1) * SEPARACION_CARACTER) / 2);
    
    return (palabra->origen.x - (ANCHURA_CARACTER_MAX / 2) + media_palabra);
}

void dibujar_palabra(struct Palabra* palabra, HDC hdc) {
    for (uint8_t i = 0; i < palabra->num_letras; i++) {
        dibujarDibujable(hdc, &palabra->letras[i]);
    }
}

void destruir_palabra(struct Palabra* palabra) {
    if(palabra == NULL) return;
    if(palabra -> letras != NULL){
        for(uint8_t i = 0; i < palabra -> num_letras; i++){
            destruirDibujable(&palabra -> letras[i]);
        }
        free(palabra -> letras);
    }
    free(palabra);
}