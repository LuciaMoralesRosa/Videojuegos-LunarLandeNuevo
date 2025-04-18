#ifndef AUXILIARES_MENUS_H
#define AUXILIARES_MENUS_H

#include "../dibujable.h"
#include "../palabra.h"

extern const struct Punto origen_msj_1;
extern const struct Punto origen_msj_2;
extern const struct Punto origen_msj_3;
extern const struct Punto origen_msj_4;
extern const struct Punto origen_msj_5;

void crear_cadena_dado_valor_4_digitos(int valor, char* buffer);

void crear_cadena_dado_valor_3_digitos(int valor, char* buffer);

struct Palabra* crear_mensaje_puntuacion(int puntuacion, struct Punto origen);
#endif // AUXILIARES_MENUS_H