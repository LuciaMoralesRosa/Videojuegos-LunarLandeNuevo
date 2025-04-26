#ifndef GESTOR_ZOOM
#define GESTOR_ZOOM

#include <stdint.h> 
#include "../dibujable.h"

#define entrada_modo_zoom_nave 1
#define entrada_modo_zoom_terreno 2.2

/**
 * @brief Indica si existe una arista del terreno dentro del radio estipulado 
 * 		  para activar el zoom con centro en la nave
 * 
 * @param nave Objeto nave cuyo centro indica el punto a partir el cual se 
 * 			   calculara el radio
 * @param terreno terreno cuyas aristas se evaluaran
 * 
 * @return 0 si no hay arista en el radio, 1 si la hay y se debe activar el zoom
 */
uint8_t hay_arista_en_radio_activar_zoom(struct Punto nave, struct Dibujable* terreno);

/**
 * @brief Indica si no existe una arista del terreno dentro del radio estipulado 
 * 		  para desactivar el zoom con centro en la nave
 * 
 * @param nave Objeto nave cuyo centro indica el punto a partir el cual se 
 * 			   calculara el radio
 * @param terreno terreno cuyas aristas se evaluaran
 * 
 * @return 0 si hay arista en el radio, 1 si no la hay y se debe desactivar el
 * 		   zoom
 */
uint8_t no_hay_arista_en_radio_desactivar_zoom(struct Punto nave, struct Dibujable* terreno);

/**
 * @brief Indica si la nave esta proxima al borde inferior de la pantalla
 * 
 */
uint8_t nave_proxima_a_borde_inferior(struct Punto nave);





void desactivar_zoom();

void activar_zoom();

void gestionar_zoom_aterrizaje(struct Punto traslacion_nave);

#endif // GESTOR_ZOOM