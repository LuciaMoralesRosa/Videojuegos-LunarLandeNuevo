#include "terreno.h"
#include "../data/variables_globales.h"
#include "../data/constantes.h"
#include "../resources/superficie_lunar.h"


void DrawLine2(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        SetPixel(hdc, x1, y1, color); // Dibuja el pixel actual

        if (x1 == x2 && y1 == y2) break; // Si llegamos al final, salimos

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

uint8_t esta_totalmente_visible(const struct Dibujable* dibujable) {
    return (dibujable->puntos[0].x >= 0 &&
            dibujable->puntos[dibujable->num_puntos - 1].x <= tamano_inicial_pantalla_X * factor_escalado);
}


void dibujar_arista_terreno(HDC hdc, struct Arista arista) {
	DrawLine2(hdc, arista.origen->x,
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

void dibujar_desde_primer_punto_fuera(HDC hdc, const struct Dibujable* dibujable, struct Plataforma* plataformas, uint8_t n){
    for (int i = dibujable->num_puntos - 1; i >= 0; i--) {
        struct Arista arista = dibujable->aristas[i];
        if (dibujable->puntos[i].x < 0) {
            break;
        }
        dibujar_arista_terreno(hdc, arista);
    }
    for(int i = 0; i < n; i++){
        if(plataformas[i].linea[0].puntos[1].x < 0) {
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

