#ifndef GESTOR_COLISIONES_H
#define GESTOR_COLISIONES_H

#include "dibujable.h"

/**
 * @brief Funcion que comprueba si existe colision entre las aristas de dos
 *        dibujables
 * 
 * @note Se comprueban todas las aristas del primer dibujable con todas las
 *       aristas del segundo dibujable
 * 
 * @param objeto1 Dibujable 1
 * @param objeto2 Dibujable 2
 * 
 * @return 1 si hay colision, 0 si no la hay
 */
uint8_t hay_colision(struct Dibujable* objeto1, struct Dibujable* objeto2, struct Arista* arista_colision);


#endif // GESTOR_COLISIONES_H