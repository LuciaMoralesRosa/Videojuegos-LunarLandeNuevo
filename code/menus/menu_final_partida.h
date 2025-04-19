#ifndef MENU_FINAL_PARTIDA_H
#define MENU_FINAL_PARTIDA_H

#include <windows.h>

/**
 * @brief Genera los mensajes de fin de partida por falta de combustible.
 * 
 * Crea los textos que se muestran al jugador cuando la partida termina por
 * quedarse sin combustible.
 * Incluye el mensaje de puntuación y dos frases finales. Luego aplica el
 * escalado correspondiente.
 * 
 * @param puntuacion Puntuación final obtenida por el jugador.
 */
void generar_mensaje_final_partida(int puntuacion);

/**
 * @brief Dibuja los mensajes de fin de partida en pantalla.
 * 
 * Muestra en pantalla el mensaje de puntuación y los mensajes principales de
 * cierre del juego.
 * 
 * @param hdc Contexto gráfico de Windows donde se dibujan las palabras.
 */
void dibujar_mensajes_final_partida(HDC hdc);

/**
 * @brief Escala los mensajes del menú final según el factor proporcionado.
 * 
 * Aplica un factor de escalado en los ejes X e Y a todos los textos del menú 
 * final de partida.
 * 
 * @param factor Valor de escalado a aplicar.
 */
void escalar_menu_final_partida(float factor);

/**
 * @brief Libera la memoria utilizada por los mensajes del menú final de partida.
 * 
 * Elimina todas las estructuras `Palabra` generadas en este módulo.
 */
void destruir_menu_final_partida(void);

#endif // MENU_FINAL_PARTIDA_H