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


void dibujar_fondo_estrellado(HDC hdc) {

}


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
    if(modo_ia_activado == 1) {
        enviar_datos_ia();
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
    manejar_instante_partida();
}

void pintar_pantalla(HDC hdc){
    dibujar_escena(hdc);
    if(estado_actual == ESTADO_ATERRIZAJE && tipo_aterrizaje == COLISION) {
        dibujar_fragmentos(hdc);
    }
}

void cambiar_estado(int nuevo_estado){
    estado = nuevo_estado;
}

void escalar_escena(float factor_x, float factor_y) {
    escalar_escena_partida(factor_x, factor_y);
}

void reestablecer_mision(Tipo_Mision mision) {
    switch(mision) {
        case TRAINING:
            printf("MISSION: TRAINNING\n");
            gravedad_m_ms = GRAVEDAD_TRAINING;
            friccion_atmosfera_activada = 1;
        break;
        case CADET:
            printf("MISSION: CADET\n");
            gravedad_m_ms = GRAVEDAD_CADET;
        break;
        case PRIME:
            printf("MISSION: PRIME\n");
            gravedad_m_ms = GRAVEDAD_PRIME;
        break;
        case COMMAND:
            printf("MISSION: COMMAND\n");
            gravedad_m_ms = GRAVEDAD_COMMAND;
        break;
    }
}


void iniciar_partida(int monedas_introducidas, Tipo_Mision mision, Tipo_Terreno terreno) {
    reestablecer_mision(mision);
    inicializar_partida(terreno);
    insertar_monedas(monedas_introducidas);
    comenzarPartida();
    escalar_escena_partida(factor_escalado, factor_escalado);
}

void continuar_tras_aterrizaje(void){
    continuar_tras_aterrizaje_partida();
    comenzarPartida();
    escalar_escena_partida(factor_escalado, factor_escalado);
}