#ifndef AUXILIARES_MENUS_H
#define AUXILIARES_MENUS_H

#include "../dibujable.h"
#include "../palabra.h"

extern const struct Punto origen_msj_1;
extern const struct Punto origen_msj_2;
extern const struct Punto origen_msj_3;
extern const struct Punto origen_msj_4;
extern const struct Punto origen_msj_5;


/**
 * @brief Crea una cadena de 4 caracteres representando un valor numérico.
 *
 * Convierte un valor entero entre 0 y 9999 a una cadena de 4 caracteres
 * numéricos.
 * 
 * @param valor El valor entero a convertir (rango: 0–9999).
 * @param buffer Un arreglo de al menos 5 caracteres donde se guarda la cadena
 * resultante.
 */
void crear_cadena_dado_valor_4_digitos(int valor, char* buffer);

/**
 * @brief Crea una cadena de 4 caracteres representando un valor de 3 dígitos
 * con un 0 inicial.
 *
 * Convierte un valor entero entre 0 y 999 a una cadena del tipo "0XYZ"
 * 
 * @param valor El valor entero a convertir (rango: 0–999).
 * @param buffer Un arreglo de al menos 5 caracteres donde se guarda la cadena
 * resultante.
 */
void crear_cadena_dado_valor_3_digitos(int valor, char* buffer);

/**
 * @brief Crea una palabra con la puntuación y la palabra "POINTS".
 * 
 * Usa una puntuación entera y la convierte a una palabra como "0045 POINTS"
 * 
 * @param puntuacion La puntuación a mostrar (entre 0 y 9999).
 * @param origen El punto de origen donde se dibujará el mensaje.
 * 
 * @return Un puntero a la estructura Palabra creada.
 */
struct Palabra* crear_mensaje_puntuacion(int puntuacion, struct Punto origen);

#endif // AUXILIARES_MENUS_H