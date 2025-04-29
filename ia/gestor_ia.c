#include "gestor_ia.h"

#include <stdio.h>
#include "modelo.h"

#include "../code/lunar_lander.h"

void realizar_acciones_dada_mejor_accion(int mejor_accion){
    int accion[5];

    // Decodificar los 5 bits
    for (int i = 0; i < 5; i++) {
        accion[i] = (mejor_accion >> i) & 1;
    }

    if(accion[2]) {
        pulsar_tecla(DERECHA);
    }
    if(accion[1]) {
        pulsar_tecla(IZQUIERDA);
    }
    if(accion[0]) {
        pulsar_tecla(ARRIBA);
    }
}


void ejecutar_ia(float* entrada) {
    float salida[32];

    // Simula inferencia
    inferencia(entrada, salida);

    int mejor_accion = 0;
    float mejor_valor = salida[0];
    for (int i = 1; i < 32; i++) {
        if (salida[i] > mejor_valor) {
            mejor_valor = salida[i];
            mejor_accion = i;
        }
    }

    // mejor_accion es el índice que debes ejecutar

    realizar_acciones_dada_mejor_accion(mejor_accion);
}
