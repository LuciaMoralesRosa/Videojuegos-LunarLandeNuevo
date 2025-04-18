#ifndef VARIABLES_JUEGO_H
#define VARIABLES_JUEGO_H

/* Estado de la aplicación */
typedef enum {
    ESTADO_PIDIENDO_MONEDA,
    ESTADO_OPCIONES,
    ESTADO_JUEGO,
    ESTADO_ATERRIZAJE,
    ESTADO_FIN_PARTIDA
} estado_aplicacion;

extern estado_aplicacion estado_actual;

#endif // VARIABLES_JUEGO_H