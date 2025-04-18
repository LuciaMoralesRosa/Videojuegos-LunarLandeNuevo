#ifndef MENU_OPCIONES_H
#define MENU_OPCIONES_H

#include <windows.h>

typedef enum {
    MISSION,
    ASTEROIDS,
    TURRETS,
    EXIT,
    NUM_OPCIONES  // Nos sirve para saber el total de opciones.
} OpcionMenu;

typedef enum {
    TRAINING,
    CADET,
    PRIME,
    COMMAND
} Tipo_Mision;

void inicializar_menu_nueva_partida(void);

void dibujarMenuEnBuffer(HDC hdc, HWND hwndReal);


LRESULT procesar_pulsado_flechas(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

OpcionMenu obtenerOpcionSeleccionada(void);

void gestionar_opcion_seleccionada(void);

#endif // MENU_OPCIONES_H
