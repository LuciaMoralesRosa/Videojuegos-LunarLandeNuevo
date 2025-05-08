#include "terreno.h"
#include "../data/variables_globales.h"
#include "../data/constantes.h"
#include "../resources/superficie_lunar.h"
#include "dibujar.h"

uint8_t esta_totalmente_visible(const struct Dibujable* dibujable) {
    return (dibujable->puntos[0].x >= 0 &&
            dibujable->puntos[dibujable->num_puntos - 1].x <= tamano_inicial_pantalla_X * factor_escalado);
}


void dibujar_arista_terreno(HDC hdc, struct Arista arista) {
	DrawLine(hdc, arista.origen->x,
		arista.origen->y,
		arista.destino->x,
		arista.destino->y,
		RGB(255, 255, 255));
}

void dibujar_hasta_primer_punto_fuera(HDC hdc, const struct Dibujable* dibujable, struct Plataforma* plataformas, uint8_t n) {
	for(int i = 0; i < dibujable->num_aristas; i++) {
        struct Arista arista = dibujable->aristas[i];
        if(arista.destino->x > tamano_inicial_pantalla_X * factor_escalado) {
            struct Punto destino = {tamano_inicial_pantalla_X * factor_escalado, arista.destino->y};
            struct Arista arista_aux = {arista.origen, &destino};
            dibujar_arista_terreno(hdc, arista_aux);
            break;
        }
        dibujar_arista_terreno(hdc, arista);
	}
    for(int i = 0; i < n; i++){
        if(plataformas[i].linea[0].puntos[0].x <= tamano_inicial_pantalla_X * factor_escalado) {
            dibujar_plataforma(hdc, plataformas[i]);
        }
    }
}


// Sirve para cuando el terreno se sale por la izquierda
void dibujar_desde_primer_punto_fuera(HDC hdc, const struct Dibujable* dibujable, struct Plataforma* plataformas, uint8_t n){

    for (int i = dibujable->num_aristas - 1; i >= 0; i--) {
        struct Arista arista = dibujable->aristas[i];
        if (dibujable->aristas[i].origen->x < 0) {
            struct Punto nuevo_punto = (struct Punto){0, dibujable->puntos[i].y};
            arista = (struct Arista) {&nuevo_punto, dibujable->aristas[i].destino};
            dibujar_arista_terreno(hdc, arista);
            break;
        }
        dibujar_arista_terreno(hdc, arista);
    }

    for(int i = 0; i < n; i++){
        if(plataformas[i].linea[0].puntos[1].x >= 0) {
            dibujar_plataforma(hdc, plataformas[i]);
        }
    }
}

uint8_t esta_totalmente_fuera(const struct Dibujable* dibujable) {
    // Comprobamos si todos los puntos están a la izquierda
    uint8_t fuera_izquierda = 1;
    uint8_t fuera_derecha = 1;

    for (int i = 0; i < dibujable->num_puntos; i++) {
        float x = dibujable->puntos[i].x;

        if (x >= 0) {
            fuera_izquierda = 0;
        }
        if (x <= tamano_inicial_pantalla_X * factor_escalado) {
            fuera_derecha = 0;
        }

        // Si al menos un punto está dentro, no está totalmente fuera
        if (!fuera_izquierda && !fuera_derecha) {
            return 0;
        }
    }
    return 1; // Todos están fuera a la izquierda o derecha
}

void dibujar_plataformas(HDC hdc, struct Plataforma* plataformas, int n) {
    for(int i = 0; i < n; i++){
        dibujar_plataforma(hdc, plataformas[i]);
    }
}

void dibujar_superficie_lunar(HDC hdc, struct Dibujable* terreno, struct Plataforma* plataformas, uint8_t numero_plataformas){
    if(esta_totalmente_visible(terreno)) {
        // El dibujable esta completamente en la pantalla
        dibujar_dibujable(hdc, terreno);
        dibujar_plataformas(hdc, plataformas, numero_plataformas);
    }
	else {
		// Encontrar punto por donde se sale
        if(!esta_totalmente_fuera(terreno)) {
            if(terreno->puntos[0].x >= 0) {
                // Se sale por el lado derecho
                dibujar_hasta_primer_punto_fuera(hdc, terreno, plataformas, numero_plataformas);
            }
            else {
                // Se sale por el lado izquierdo
                dibujar_desde_primer_punto_fuera(hdc, terreno, plataformas, numero_plataformas);
            }
        }
	}
}


void dibujar_punto_cielo(HDC hdc, int x, int y) {
    DrawLine(hdc, x, y, x+1, y, RGB(255, 255, 255));
    DrawLine(hdc, x, y+1, x+1, y+1, RGB(255, 255, 255));

}

void dibujar_cielo_estrellado(HDC hdc) {
    dibujar_punto_cielo(hdc, 59, 91);
    dibujar_punto_cielo(hdc, 89, 210);
    dibujar_punto_cielo(hdc, 122, 177);
    dibujar_punto_cielo(hdc, 127, 112);
    dibujar_punto_cielo(hdc, 213, 196);
    dibujar_punto_cielo(hdc, 364, 244);
    dibujar_punto_cielo(hdc, 419, 215);
    dibujar_punto_cielo(hdc, 435, 76);
    dibujar_punto_cielo(hdc, 503, 265);
    dibujar_punto_cielo(hdc, 511, 250);
    dibujar_punto_cielo(hdc, 645, 88);
    dibujar_punto_cielo(hdc, 601, 67);
    dibujar_punto_cielo(hdc, 671, 233);
    dibujar_punto_cielo(hdc, 732, 277);
    dibujar_punto_cielo(hdc, 786, 241);
    dibujar_punto_cielo(hdc, 823, 244);
    dibujar_punto_cielo(hdc, 834, 122);
    dibujar_punto_cielo(hdc, 842, 289);
    dibujar_punto_cielo(hdc, 910, 16);
    dibujar_punto_cielo(hdc, 978, 168);
    dibujar_punto_cielo(hdc, 994, 121);
    dibujar_punto_cielo(hdc, 384, 310);
    dibujar_punto_cielo(hdc, 972, 327);
    dibujar_punto_cielo(hdc, 147, 342);

}