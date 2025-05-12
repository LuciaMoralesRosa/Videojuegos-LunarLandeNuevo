#include "variables_juego.h"
#include "constantes.h"


struct objetoFisico* nave = NULL;

estado_aplicacion estado_actual = ESTADO_PIDIENDO_MONEDA;
Aterrizaje tipo_aterrizaje = COLISION;
Modo_Zoom modo_zoom = DESACTIVADO;

uint8_t fisicas = DESACTIVADAS;

float pos_real_nave_x = 0;
float pos_real_nave_y = 0;

int terrenos_desplazados_a_dcha = 0;

int combustible = 0;
int puntuacion_partida = 0;


float velocidad_horizontal = 0.0;
float velocidad_vertical = 0.0;
float altitud = 0.0;


// Plataformas
struct Plataforma* plataformas_0 = NULL;
struct Plataforma* plataformas_1 = NULL;
uint8_t numero_plataformas = 0;