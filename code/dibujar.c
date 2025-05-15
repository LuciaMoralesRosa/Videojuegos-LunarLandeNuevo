#include "dibujar.h"

/**
 * @brief Dibuja una posición.
 *
 * @param x1    Coordenada X del punto a dibujar.
 * @param y1    Coordenada Y del punto a dibujar.
 *
 */
void SetPixel(int xp, int yp) {
    PORTB = xp;
    PORTD = yp;
}

void DrawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        SetPixel(x1, y1); // Dibuja el pixel actual

        if (x1 == x2 && y1 == y2) break; // Si llegamos al final, salimos

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}