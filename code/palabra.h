#ifndef PALABRA_H
#define PALABRA_H

#include "dibujable.h"

/**
 * @brief Estructura que representa una palabra
 */
struct Palabra {
    struct Dibujable** letras;
    uint8_t num_letras;
    struct Punto origen; // Se corresponde con el origen de la primera letra
    float factor_escalado_x;
    float factor_escalado_y;
};

/**
 * @brief Crea una palabra vacia
 * 
 * @param origen Punto de origen de la palabra
 */
struct Palabra* crear_palabra(struct Punto origen);

/**
 * @brief Agrega una letra a la palabra
 * 
 * @param palabra Palabra a la que se le agregara la letra
 * @param letra Letra a agregar
 */
void agregar_letra(struct Palabra* palabra, const struct DibujableConstante* letra);



/**
 * @brief Dibuja una palabra
 * 
 * @param palabra Palabra a dibujar
 * @param hdc Contexto de dispositivo
 */
void dibujar_palabra(struct Palabra* palabra);


struct Palabra* crear_palabra_desde_cadena(const char* cadena, struct Punto origen);


#endif // PALABRA_H