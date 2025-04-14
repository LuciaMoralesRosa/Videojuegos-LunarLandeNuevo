#ifndef TEXTO_H
#define TEXTO_H

#include "dibujable.h"

struct Texto {
    struct Dibujable** caracteres;  // Ahora es un arreglo de punteros
    uint8_t num_caracteres;
    struct Punto origen; // Origen de la primera letra
    float factor_escalado_x;
    float factor_escalado_y;
};

struct Texto* crear_texto(struct Punto origen);
void agregar_caracter(struct Texto* texto, const struct DibujableConstante* caracter);
int16_t calcular_centro_x_texto(struct Texto* texto);
void dibujar_texto(struct Texto* texto, HDC hdc);
void destruir_texto(struct Texto* texto);
struct Texto* crearTextoDesdeCadena(const char* cadena, struct Punto origen);

#endif // TEXTO_H
