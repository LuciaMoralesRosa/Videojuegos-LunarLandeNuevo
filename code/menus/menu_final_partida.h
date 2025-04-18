#ifndef MENU_FINAL_PARTIDA_H
#define MENU_FINAL_PARTIDA_H

#include <windows.h>

void generar_mensaje_final_partida(int puntuacion);
void dibujar_mensajes_final_partida(HDC hdc);
void escalar_menu_final_partida(float factor);
void borrar_menu_final_partida(void);

#endif // MENU_FINAL_PARTIDA_H