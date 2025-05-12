#ifndef VARIABLES_JUEGO_H
#define VARIABLES_JUEGO_H

#include <stdint.h>

/* Estado de la aplicación */
typedef enum {
    ESTADO_PIDIENDO_MONEDA,
    ESTADO_OPCIONES,
    ESTADO_CONTROLES,
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

/**
 * @brief Enumeración para el estado de las fisicas
 * 
 * DESACTIVADAS: 0
 * ACTIVADAS: 1
 */
enum fisicas {
    DESACTIVADAS,
    ACTIVADAS
};

// Objeto fisico que representa a la nave
extern struct objetoFisico* nave;

// Estado actual del juego
extern estado_aplicacion estado_actual;

// Indica si las fisicas estan activadas (1) o no (0)
extern uint8_t fisicas;

// Tipo de aterrizaje conseguido
extern Aterrizaje tipo_aterrizaje;

// Indica si el modo zoom esta activado (1) o no (0)
extern Modo_Zoom modo_zoom;

// Variables de la partida
extern int combustible;
extern int puntuacion_partida;
extern float velocidad_horizontal;
extern float velocidad_vertical;
extern float altitud;
extern float pos_real_nave_x;
extern float pos_real_nave_y;

// Indica el numero de veces que se ha desplazado el terreno hacia la izquierda 
// o la derecha (izquierda si son negativos, derecha si son positivos)
extern int terrenos_desplazados_a_dcha;


// Plataformas
extern struct Plataforma* plataformas_0;
extern struct Plataforma* plataformas_1;
extern uint8_t numero_plataformas;


#endif // VARIABLES_JUEGO_H