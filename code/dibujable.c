#include "dibujable.h"
#include <stdio.h>
#include "dibujar.h"
// ----------------------------- METODOS PUBLICOS ------------------------------

struct Dibujable* crear_dibujable(const struct DibujableConstante* constante) {
    // Reservar memoria para el Dibujable
    struct Dibujable* dibujable = (struct Dibujable*)malloc(sizeof(struct Dibujable));
    if (!dibujable) return NULL;  // Si falla malloc(), retorna NULL

    dibujable->origen = constante->origen;
    dibujable->num_puntos = constante->num_puntos;
    dibujable->num_aristas = constante->num_aristas;

    // Reservar memoria y copiar los puntos
    dibujable->puntos = (struct Punto*)malloc(constante->num_puntos * sizeof(struct Punto));
    if (!dibujable->puntos) {
        free(dibujable);
        return NULL;
    }
    memcpy(dibujable->puntos, constante->puntos, constante->num_puntos * sizeof(struct Punto));  

    // Reservar memoria y copiar las aristas
    dibujable->aristas = (struct Arista*)malloc(constante->num_aristas * sizeof(struct Arista));
    if (!dibujable->aristas) {
        free(dibujable->puntos);
        free(dibujable);
        return NULL;
    }
    for (uint8_t i = 0; i < constante->num_aristas; i++) {
        dibujable->aristas[i].origen = &dibujable->puntos[constante->unionAristas[i].origen];
        dibujable->aristas[i].destino = &dibujable->puntos[constante->unionAristas[i].destino];
    }
    return dibujable;
}

void dibujar_dibujable(HDC hdc, const struct Dibujable* dibujable){
    for(uint8_t i = 0; i < dibujable->num_aristas; i++) {
        DrawLine(hdc, dibujable->aristas[i].origen->x,
            dibujable->aristas[i].origen->y,
            dibujable->aristas[i].destino->x,
            dibujable->aristas[i].destino->y,
            RGB(255, 255, 255));
    }
}

uint8_t es_horizontal(struct Arista arista_colision){
    if(arista_colision.origen->y == arista_colision.destino->y) {
        return 1;
    }
    return 0;
}

void destruir_arista(struct Arista* arista){
    if(arista == NULL) return;
    if(arista -> origen != NULL){
        free(arista -> origen);
    }
    if(arista -> destino != NULL){
        free(arista -> destino);
    }
    free(arista);
}

void destruir_dibujable(struct Dibujable* dibujable) {
    if(dibujable == NULL) return;
    if(dibujable -> puntos != NULL){
        free(dibujable -> puntos);
    }
    if(dibujable -> aristas != NULL) {
        free(dibujable -> aristas);
    }
    free(dibujable);
}