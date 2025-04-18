#ifndef MENU_ATERRIZAJE_H
#define MENU_ATERRIZAJE_H

#include <windows.h>

#include "../../data/variables_juego.h"

void generar_mensaje_aterrizaje(int puntuacion);
void dibujar_mensajes_aterrizaje(HDC hdc);
void escalar_menu_aterrizaje(float factor);
void borrar_menu_aterrizaje(void);
#endif // MENU_ATERRIZAJE_H 