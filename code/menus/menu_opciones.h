/**
 * @file menu_opciones.h
 * @brief Interfaz para el menú de configuración antes de iniciar una nueva
 * partida.
 *
 * Permite al jugador configurar parámetros como el tipo de misión, activar 
 * asteroides o torretas, y salir al menú principal. También maneja el
 * renderizado y la navegación mediante el teclado.
 */

#ifndef MENU_OPCIONES_H
#define MENU_OPCIONES_H

#include <windows.h>

/**
 * @enum Opcion_Menu
 * @brief Enumeración para las distintas opciones del menú.
 */
typedef enum {
    MISSION,     ///< Selección del tipo de misión.
    ASTEROIDS,   ///< Activación/desactivación de asteroides.
    TURRETS,     ///< Activación/desactivación de torretas.
    EXIT,        ///< Salir del menú de opciones.
    NUM_OPCIONES ///< Total de opciones (útil para loops o validaciones).
} Opcion_Menu;

/**
 * @enum Tipo_Mision
 * @brief Tipos posibles de misión seleccionables en el menú.
 */
typedef enum {
    TRAINING, ///< Misión de entrenamiento.
    CADET,    ///< Misión de cadete.
    PRIME,    ///< Misión estándar.
    COMMAND   ///< Misión avanzada.
} Tipo_Mision;


/**
 * @brief Inicializa todos los elementos del menú de opciones.
 * 
 * Crea los textos del título, las opciones y el indicador, además de aplicar el
 * escalado inicial.
 */
void inicializar_menu_nueva_partida(void);

/**
 * @brief Dibuja el menú de opciones en el contexto gráfico especificado.
 * 
 * @param hdc Contexto gráfico (handle del dispositivo).
 * @param hwndReal Ventana sobre la cual se está dibujando.
 */
void dibujar_menu_opciones(HDC hdc, HWND hwndReal);

/**
 * @brief Aplica un factor de escalado a todos los elementos del menú.
 * 
 * @param factor Valor de escalado a aplicar.
 */
void escalar_menu_opciones(float factor);

/**
 * @brief Procesa el evento de teclado relacionado con las flechas arriba/abajo.
 * 
 * Permite navegar por las distintas opciones del menú.
 *
 * @param hwnd Ventana activa.
 * @param uMsg Mensaje de Windows.
 * @param wParam Tecla presionada.
 * @param lParam Parámetro adicional del evento.
 * @return LRESULT Devuelve 0 si se procesó correctamente.
 */
LRESULT procesar_pulsado_flechas(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/**
 * @brief Devuelve la opción actualmente seleccionada en el menú.
 *
 * @return Opcion_Menu La opción seleccionada.
 */
Opcion_Menu obtener_opcion_seleccionada(void);

/**
 * @brief Gestiona la acción correspondiente a la opción actualmente seleccionada.
 * 
 * Cambia el tipo de misión o alterna la activación de asteroides/torretas.
 */
void gestionar_opcion_seleccionada(void);

/**
 * @brief Libera la memoria utilizada por los elementos del menú.
 */
void destruir_menu_opciones(void);

#endif // MENU_OPCIONES_H
