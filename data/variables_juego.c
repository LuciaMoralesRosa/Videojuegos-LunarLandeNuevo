#include "variables_juego.h"
#include "constantes.h"




estado_aplicacion estado_actual = ESTADO_PIDIENDO_MONEDA;
Aterrizaje tipo_aterrizaje = COLISION;
Modo_Zoom modo_zoom = DESACTIVADO;

uint8_t nave_borde_inferior = 0; 
uint8_t fisicas = DESACTIVADAS;

float pos_real_nave_x = 0;
int terrenos_desplazados_a_dcha = 0;

int combustible = 0;
int puntuacion_partida = 0;


float velocidad_horizontal = 0.0;
float velocidad_vertical = 0.0;
float altitud = 0.0;
