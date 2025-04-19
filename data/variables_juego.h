#ifndef VARIABLES_JUEGO_H
#define VARIABLES_JUEGO_H

#include <stdint.h>

/* Estado de la aplicación */
typedef enum {
    ESTADO_PIDIENDO_MONEDA,
    ESTADO_OPCIONES,
    ESTADO_JUEGO,
    ESTADO_ATERRIZAJE,
    ESTADO_FIN_PARTIDA
} estado_aplicacion;

typedef enum {
    PERFECTO,
    BRUSCO, 
    COLISION
} Aterrizaje;

typedef enum {
    ACTIVADO,
    DESACTIVADO
} Modo_Zoom;

extern estado_aplicacion estado_actual;
extern Aterrizaje tipo_aterrizaje;
extern Modo_Zoom modo_zoom;
extern uint8_t nave_borde_inferior;

extern int combustible;
extern int puntuacion_partida;
extern float velocidad_horizontal;
extern float velocidad_vertical;
extern float altitud;


#endif // VARIABLES_JUEGO_H