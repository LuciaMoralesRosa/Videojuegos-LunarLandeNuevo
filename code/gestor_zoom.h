#ifndef GESTOR_ZOOM
#define GESTOR_ZOOM

#include <stdint.h> 
#include "dibujable.h"

uint8_t hay_arista_en_radio_zoom(uint8_t modo_zoom, struct Punto nave, struct Dibujable* terreno);

uint8_t hay_arista_en_radio_activar_zoom(struct Punto nave, struct Dibujable* terreno);
uint8_t no_hay_arista_en_radio_desactivar_zoom(struct Punto nave, struct Dibujable* terreno);
uint8_t nave_proxima_a_borde_inferior(struct Punto nave);
#endif // GESTOR_ZOOM