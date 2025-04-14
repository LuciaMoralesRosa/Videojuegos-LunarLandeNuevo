#include "transformaciones.h"

#include "../resources/caracteres.h"


/**
 * @brief Traslada un punto en la dirección indicada
 * 
 * @param origen Punto a trasladar
 * @param traslacion Punto de traslación
 */
void trasladarPunto(struct Punto* origen, struct Punto traslacion){
    origen -> x += traslacion.x;
    origen -> y += traslacion.y;
}

void trasladarDibujable(struct Dibujable* dibujable, struct Punto traslacion){
    trasladarPunto(&dibujable -> origen, traslacion);
    if (!dibujable -> puntos) return;
    for(uint8_t i = 0; i < dibujable -> num_puntos; i++){
        trasladarPunto(&dibujable -> puntos[i], traslacion);
    }
}

void colocar_dibujable(struct Dibujable* dibujable, struct Punto destino){
    if (!dibujable->puntos) return;
    struct Punto traslacion = (struct Punto){ destino.x - dibujable -> origen.x, destino.y - dibujable -> origen.y};
    trasladarDibujable(dibujable, traslacion);
}

void colocar_palabra(struct Palabra* palabra, struct Punto destino){
    if(!palabra || !palabra->letras) return;
    palabra->origen.x = destino.x;
    palabra->origen.y = destino.y;

    float separacion_centros = (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER) * palabra->factor_escalado_x;
    if(separacion_centros < 1) {
        separacion_centros = 1;
    }

    for(uint8_t i = 0; i < palabra -> num_letras; i++) {
        float nuevo_x = destino.x + separacion_centros * i;
        colocar_dibujable(&palabra->letras[i], (struct Punto){nuevo_x, destino.y});
    } 
}

void colocarDibujable(struct Dibujable* dibujable, struct Punto destino){
	struct Punto traslacion = (struct Punto){
		destino.x - dibujable -> origen.x,
		destino.y - dibujable -> origen.y
	};
    trasladarDibujable(dibujable, traslacion);
}

/**
 * @brief Rota un punto dado un centro y un sentido
 * 
 * @param punto Punto a rotar
 * @param centro Centro de rotación
 * @param sentido 1 para horario, -1 para antihorario
 */
void rotarPuntoDadoCentro(struct Punto* punto, struct Punto centro, int8_t sentido){
    float x = punto->x - centro.x;
    float y = punto->y - centro.y;
    float nuevaX, nuevaY;

    nuevaX = x * COS_TABLA[ANGULO_ROTACION] - y * sentido * SIN_TABLA[ANGULO_ROTACION];
    nuevaY = x * sentido * SIN_TABLA[ANGULO_ROTACION] + y * COS_TABLA[ANGULO_ROTACION];
    
    punto->x = (nuevaX + centro.x);
    punto->y = (nuevaY + centro.y);
}

void rotarDibujable(struct Dibujable* dibujable, unsigned char direccion){
    if (!dibujable->puntos) return;
    int8_t sentido = (direccion == 0) ? -1 : 1;
    for(uint8_t i = 0; i < dibujable->num_puntos; i++){
        rotarPuntoDadoCentro(&dibujable->puntos[i], dibujable->origen, sentido);
    }
}


void escalarXPuntoDadoCentro(struct Punto* punto, struct Punto centro, float factor){
    float x = punto->x - centro.x;
    punto->x = (float)(x * factor + centro.x);
}

void escalarYPuntoDadoCentro(struct Punto* punto, struct Punto centro, float factor){
    float y = punto->y - centro.y;
    punto->y = (float)(y * factor + centro.y);
}

void escalarDibujable(struct Dibujable* dibujable, float factor){
    escalarDibujableDadosEjes(dibujable, factor, factor);
}

void escalar_dibujable_en_escena_dados_ejes(struct Dibujable* dibujable, float factorX, float factorY) {
    if (!dibujable->puntos) return;
    dibujable->origen.x *= factorX;
    dibujable->origen.y *= factorY;
    
    for(uint8_t i = 0; i < dibujable->num_puntos; i++){
        dibujable->puntos[i].x *= factorX;
        dibujable->puntos[i].y *= factorY;      
    }
}

void escalar_palabra_en_escena_dados_ejes(struct Palabra* palabra, float factorX, float factorY) {
    if (!palabra->letras) return;
    for(uint8_t i = 0; i < palabra->num_letras; i++){
        escalar_dibujable_en_escena_dados_ejes(&palabra->letras[i], factorX, factorY);
    }

}

void escalarDibujableDadosEjes(struct Dibujable* dibujable, float factorX, float factorY){
    if (!dibujable->puntos) return;

    for(uint8_t i = 0; i < dibujable->num_puntos; i++){
        escalarXPuntoDadoCentro(&dibujable->puntos[i], dibujable->origen, factorX);
        escalarYPuntoDadoCentro(&dibujable->puntos[i], dibujable->origen, factorY);
    }
}
        
void escalar_palabra_centrada(struct Palabra* palabra, float factor){
    escalar_palabra_centrada_dados_ejes(palabra, factor, factor);
}


/**
 * @brief Escala una palabra dados los centros de cada letra en los ejes X e Y
 * 
 * @param palabra Palabra a escalar
 * @param factorX factor de escalado en el eje X
 * @param factorY factor de escalado en el eje Y
 */
void escalar_palabra(struct Palabra* palabra, float factorX, float factorY) {
    for(uint8_t i = 0; i < palabra -> num_letras; i++){
        if(!palabra->letras[i].puntos) return;
        for(uint8_t j = 0; j < palabra->letras[i].num_puntos; j++){
            escalarXPuntoDadoCentro(&palabra->letras[i].puntos[j], palabra->letras[i].origen, factorX);
            escalarYPuntoDadoCentro(&palabra->letras[i].puntos[j], palabra->letras[i].origen, factorY);
        }
    }
}


void escalar_palabra_centrada_dados_ejes(struct Palabra* palabra, float factorX, float factorY){
    if(!palabra->letras) return;

    // Establecer los factores de escalado
    palabra->factor_escalado_x = factorX;
    palabra->factor_escalado_y = factorY;

    float distancia_origen_a_centro = ((palabra->num_letras * ANCHURA_CARACTER_MAX) + ((palabra->num_letras-1)* SEPARACION_CARACTER) - ANCHURA_CARACTER_MAX) / 2;
    float nuevo_origen_palabra_x = (palabra->origen.x + distancia_origen_a_centro) - (distancia_origen_a_centro * factorX);
    
    // Generar el escalado de los caracteres de la palabra
    escalar_palabra(palabra, factorX, factorY);

    // Establecer las posiciones de los caracteres dado el nuevo origen
    colocar_palabra(palabra, (struct Punto){nuevo_origen_palabra_x, palabra->letras[0].origen.y});
}


void escalar_plataforma_dados_ejes(struct Plataforma* plataforma, float factorX, float factorY){
    if(!plataforma->linea) return;
    escalarDibujableDadosEjes(plataforma->linea, factorX, factorY);
    escalar_palabra_centrada_dados_ejes(plataforma->palabra, factorX, factorY);
}



void trasladar_palabra(struct Palabra* palabra, struct Punto traslacion){
    if(!palabra || !palabra->letras) return;
    trasladarPunto(&palabra->origen, traslacion);
    for(uint8_t i = 0; i < palabra->num_letras; i++) {
        trasladarDibujable(&palabra->letras[i], traslacion);
    }
}

void trasladar_superficie_lunar(struct Dibujable* terreno, struct Plataforma* plataformas, uint8_t num_plataformas, struct Punto traslacion) {
    trasladarDibujable(terreno, traslacion);
    for(uint8_t i = 0; i < num_plataformas; i++) {
        trasladarDibujable(plataformas[i].linea, traslacion);
        trasladar_palabra(plataformas[i].palabra, traslacion);
    }
}
