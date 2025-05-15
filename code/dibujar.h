#ifndef DIBUJAR_H
#define DIBUJAR_H

#include <stdio.h>
#include <Arduino.h>

/**
 * @brief Dibuja una línea utilizando el algoritmo de Bresenham.
 * 
 * Esta función rasteriza una línea entre dos puntos (x1, y1) y (x2, y2) 
 * utilizando el algoritmo de Bresenham. Los píxeles se dibujan en el contexto
 * gráfico proporcionado usando el color especificado.
 *
 * @param hdc   Contexto de dispositivo donde se dibujará la línea (Windows HDC)
 * @param x1    Coordenada X del punto inicial de la línea.
 * @param y1    Coordenada Y del punto inicial de la línea.
 * @param x2    Coordenada X del punto final de la línea.
 * @param y2    Coordenada Y del punto final de la línea.
 * @param color Color de la línea en formato RGB (COLORREF).
 *
 * @note Utiliza la función SetPixel para dibujar cada punto
 */
void DrawLine(int x1, int y1, int x2, int y2);


#endif // DIBUJAR_H