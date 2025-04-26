#include "lunar_lander.h"
#include "partida.h"
#include "fisicas.h"
#include "../data/variables_juego.h"
#include "../data/variables_globales.h"


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
    if(tecla == MONEDA) {
        insertar_monedas(1);
    }
}

void levantar_tecla(int tecla){
    estado_teclas[tecla] = 0;
}

void manejar_teclas(){
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
    manejar_instante_partida();
    if(estado_actual == ESTADO_JUEGO){
    }
}

void pintar_pantalla(HDC hdc){
    dibujar_escena(hdc);
}

void cambiar_estado(int nuevo_estado){
    estado = nuevo_estado;
}

void escalar_escena(float factor_x, float factor_y) {
    escalar_escena_partida(factor_x, factor_y);
}

void iniciar_partida(int monedas_introducidas) {
    combustible = 0;
    insertar_monedas(monedas_introducidas);
    inicializar_partida();
}

void continuar_tras_aterrizaje(void){
    comenzar_nueva_ronda();
}