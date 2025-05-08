/**
 * @file menu_opciones.h
 * @brief Interfaz para el menú de configuración antes de iniciar una nueva
 * partida.
 *
 * Permite al jugador configurar parámetros como el tipo de misión, activar 
 * asteroides o torretas, y salir al menú principal. También maneja el
 * renderizado y la navegación mediante el teclado.
 */

#ifndef MENU_CONTROLES_H
#define MENU_CONTROLES_H

#include <windows.h>

#include "../../data/variables_globales.h"
#endif // MENU_CONTROLES_H

typedef enum {
    CONTROL_MONEDA, ///< Misión de entrenamiento.
    CONTROL_PROPULSOR,    ///< Misión de cadete.
    CONTROL_IZDA,    ///< Misión estándar.
    CONTROL_DCHA,   ///< Misión avanzada.
    CONTROL_PAUSA,
    CONTROL_VOLVER,
    CONTROL_NUM_OPCIONES
} Opcion_Menu_Controles;

void inicializar_menu_controles();

void dibujar_menu_controles(HDC hdc, HWND hwndReal);

void escalar_menu_controles(float factor);

void destruir_menu_controles(void);

LRESULT procesar_pulsado_flechas_controles(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void gestionar_opcion_seleccionada_controles(void);

Opcion_Menu_Controles obtener_opcion_seleccionada_controles(void);

void establecer_tecla_control(WPARAM tecla);