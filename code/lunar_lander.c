#include "lunar_lander.h"
#include "partida.h"
#include "fisicas.h"
#include "../data/variables_juego.h"
#include "../data/variables_globales.h"
#include "fragmentacion_nave.h"

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
        anyadirMoneda();
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

    if(modo_ia_activado) {
        estado_teclas[IZQUIERDA] = 0;
        estado_teclas[DERECHA] = 0;
        estado_teclas[ARRIBA] = 0;
    }
}

void manejar_instante(){
    manejar_instante_partida();
}

void pintar_pantalla(){
    dibujar_escena();
    if(estado_actual == ESTADO_ATERRIZAJE && tipo_aterrizaje == COLISION) {
        dibujar_fragmentos();
    }
}

void cambiar_estado(int nuevo_estado){
    estado = nuevo_estado;
}

void iniciar_partida(int monedas_introducidas) {
    inicializar_partida();
    insertar_monedas(monedas_introducidas);
    comenzarPartida();
}

void continuar_tras_aterrizaje(void){
    continuar_tras_aterrizaje_partida();
    comenzarPartida();
}