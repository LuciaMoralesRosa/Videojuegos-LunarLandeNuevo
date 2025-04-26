#ifndef ELEMENTOS_PARTIDA_H
#define ELEMENTOS_PARTIDA_H

#include "../dibujable.h"
#include "../gestor_plataformas.h"
#include "../palabra.h"

/**
 * @brief Representa un objeto físico que se mueve según leyes básicas de física
 * 
 * @param objeto Puntero al objeto dibujable asociado.
 * @param velocidad Vector de velocidad [vx, vy].
 * @param aceleracion Vector de aceleración [ax, ay].
 * @param masa Masa del objeto.
 * @param rotacion Ángulo de rotación del objeto (en grados).
 */
struct objetoFisico {
   struct Dibujable* objeto;
   float velocidad[2];
   float aceleracion[2];
   int masa;
   int16_t rotacion;
};

extern int inicio;
extern struct objetoFisico* nave;
extern struct Dibujable* motor_debil;
extern struct Dibujable* motor_medio;
extern struct Dibujable* motor_fuerte;
extern struct Dibujable* terreno_0;
extern struct Dibujable* terreno_1;
extern struct Plataforma* plataformas_0;
extern struct Plataforma* plataformas_1;
extern uint8_t numero_plataformas;

extern int traslacion_dibujables_por_borde_inferior;
extern struct Punto posicion_nave_cuando_zoom;


#endif // ELEMENTOS_PARTIDA_H
