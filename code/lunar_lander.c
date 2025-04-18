#include "lunar_lander.h"
#include "partida.h"
#include "fisicas.h"
#include "../data/variables_juego.h"

static int estado = PEDIR;
static int estado_teclas[5] = {
    0,
    0,
    0,
    0,
    0
};

void pulsar_tecla(int tecla){
    estado_teclas[tecla] = 1;
}

void levantar_tecla(int tecla){
    estado_teclas[tecla] = 0;
}

void manejar_teclas(){
    switch(estado){
/*
        case PEDIR:
            if(estado_teclas[MONEDA]){
                printf("Moneda insertada\n");
                inicializarPartida();
                anyadirMoneda();
                estado = MENU;
            }
            break;

        case MENU:
            if(estado_teclas[MONEDA]){
                printf("Moneda insertada\n");
                anyadirMoneda();
            }
            else if(estado_teclas[ESPACIO]){
                printf("Partida Comenzada\n");
                comenzarPartida();
                estado = JUGANDO;
            }
            break;

        case JUGANDO:
            if(estado_teclas[MONEDA]){
                anyadirMoneda();
            }
            if(estado_teclas[ARRIBA]){
                activar_propulsor();
                propulsar();
            } else {
                desactivar_propulsor();
            }

            if(estado_teclas[IZQUIERDA]){
                girar_izquierda();
            }
            if(estado_teclas[DERECHA]){
                girar_derecha();
            }
            break;
*/
        default:
            break;
    }
    if(estado_teclas[MONEDA]){
        anyadirMoneda();
    }
    if(estado_teclas[ARRIBA]){
        activar_propulsor();
        propulsar();
    } else {
        desactivar_propulsor();
    }

    if(estado_teclas[IZQUIERDA]){
        girar_izquierda();
    }
    if(estado_teclas[DERECHA]){
        girar_derecha();
    }
}

void manejar_instante(){
    if(estado_actual == ESTADO_JUEGO){
        manejar_instante_partida();
    }
}

void pintar_pantalla(HDC hdc){
    if(estado_actual == ESTADO_JUEGO) {
        dibujar_escena(hdc);
    }
}

void cambiar_estado(int nuevo_estado){
    estado = nuevo_estado;
}

void escalar_escena(float factor_x, float factor_y) {
    escalar_escena_partida(factor_x, factor_y);
}

void iniciar_partida(int monedas_introducidas) {
    inicializar_partida();
    insertar_monedas(monedas_introducidas);
    comenzarPartida();
}