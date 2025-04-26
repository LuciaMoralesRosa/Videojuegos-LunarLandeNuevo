#include "elementos_partida.h"

int inicio = 0;
struct objetoFisico* nave = NULL;
struct Dibujable* motor_debil = NULL;
struct Dibujable* motor_medio = NULL;
struct Dibujable* motor_fuerte = NULL;
struct Dibujable* terreno_0 = NULL;
struct Dibujable* terreno_1 = NULL;
struct Plataforma* plataformas_0 = NULL;
struct Plataforma* plataformas_1 = NULL;
uint8_t numero_plataformas = 0;

int traslacion_dibujables_por_borde_inferior = 0;
struct Punto posicion_nave_cuando_zoom = {0};
