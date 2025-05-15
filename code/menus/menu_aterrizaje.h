#ifndef MENU_ATERRIZAJE_H
#define MENU_ATERRIZAJE_H


#include "../../data/variables_juego.h"

/**
 * @brief Genera los mensajes correspondientes según el tipo de aterrizaje.
 * 
 * Según el valor de tipo_aterrizaje, selecciona y crea diferentes frases.
 * También crea el mensaje de puntuación y el aviso para continuar.
 * Aplica escalado a todas las palabras creadas.
 * 
 * @param puntuacion Valor numérico de la puntuación obtenida en el aterrizaje.
 */
void generar_mensaje_aterrizaje(int puntuacion);

/**
 * @brief Dibuja todos los mensajes del menú de aterrizaje en pantalla.
 * 
 * Muestra en pantalla los tres mensajes principales, la puntuación y el mensaje
 * de continuar, utilizando el contexto de dispositivo proporcionado.
 * 
 * @param hdc Contexto gráfico de Windows donde se dibujan las palabras.
 */
void dibujar_mensajes_aterrizaje();

/**
 * @brief Escala todos los textos del menú de aterrizaje según el factor dado.
 * 
 * Aplica un escalado uniforme a los textos visuales generados para que se
 * adapten a la pantalla.
 * 
 * @param factor Factor de escalado a aplicar en los ejes X e Y.
 */
void escalar_menu_aterrizaje(float factor);

#endif // MENU_ATERRIZAJE_H 