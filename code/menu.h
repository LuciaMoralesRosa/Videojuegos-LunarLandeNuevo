#ifndef MENU_H
#define MENU_H

#include <windows.h>

typedef enum {
    OPCION_PLAY,
    OPCION_TEST_DIBUJABLES,
    OPCION_OPTIONS,
    OPCION_EXIT,
    NUM_OPCIONES  // Nos sirve para saber el total de opciones.
} OpcionMenu;

void inicializarMenu(void);

void dibujarMenuEnBuffer(HDC hdc, HWND hwndReal);


LRESULT procesarEventoMenu(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

OpcionMenu obtenerOpcionSeleccionada(void);

#endif // MENU_H
