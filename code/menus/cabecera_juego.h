#ifndef CABECERA_JUEGO_H
#define CABECERA_JUEGO_H

#include <windows.h>

void inicializar_cabecera(void);
void escalar_cabecera(float factor);
void dibujar_cabecera(HDC hdc);

void actualizar_segundos_cabecera(void);

void actualizar_puntuacion_cabecera(void);

#endif // CABECERA_JUEGO_H