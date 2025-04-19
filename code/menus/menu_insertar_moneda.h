#ifndef MENU_INSERTAR_MONEDA_H
#define MENU_INSERTAR_MONEDA_H
#include <windows.h>

/**
 * @brief Crea el mensaje de "Insertar moneda" para continuar la partida.
 * 
 * Inicializa la estructura Palabra con el texto que indica al jugador que
 * debe insertar una moneda.
 * También aplica el escalado inicial para que se vea correctamente en la
 * pantalla.
 */
void crear_palabra_insertar_moneda(void);

/**
 * @brief Dibuja el mensaje de "Insertar moneda" en pantalla.
 * 
 * Muestra el mensaje generado en la función crear_palabra_insertar_moneda()
 * dentro del contexto gráfico proporcionado.
 * 
 * @param hdc Contexto gráfico de Windows donde se dibuja el texto.
 */
void mostrar_insertar_moneda(HDC hdc);

/**
 * @brief Escala el mensaje de "Insertar moneda" según el factor especificado.
 * 
 * Aplica un factor de escalado en los ejes X e Y al mensaje de "Insertar moneda"
 * 
 * @param factor Valor de escalado a aplicar.
 */
void escalar_menu_insertar_moneda(float factor_escalado);

/**
 * @brief Libera la memoria utilizada por el mensaje de "Insertar moneda".
 * 
 * Destruye la estructura Palabra asociada a insertar_moneda para evitar fugas
 * de memoria.
 */
void destruir_menu_insertar_moneda(void);


#endif // MENU_INSERTAR_MONEDA_H