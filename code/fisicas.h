/**
 * @file fisicas.h
 * @brief Contiene las funciones necesarias para manejar las físicas del juego.
 *
 * Este módulo gestiona el comportamiento físico de los objetos en la escena,
 * incluyendo la aplicación de gravedad, propulsión y rotación de la nave.
 */


#ifndef FISICAS_H
#define FISICAS_H

#include "dibujable.h"
#include "transformaciones.h"

// Intervalo de tiempo entre actualizaciones de físicas (en milisegundos).
#define intervalo_fisicas_ms 32

// Número de píxeles que representan un metro en pantalla.
#define pixels_por_metro 32

// Valor de la aceleración gravitatoria en m/ms² (negativa por ser hacia abajo).
#define gravedad_m_ms -0.00020625 

// Potencia del propulsor en m/ms².
#define propulsor_m_ms 0.001

/**
 * @brief Representa un objeto físico que se mueve según leyes básicas de física
 * 
 * @param objeto Puntero al objeto dibujable asociado.
 * @param velocidad Vector de velocidad [vx, vy].
 * @param aceleracion Vector de aceleración [ax, ay].
 * @param masa Masa del objeto.
 * @param rotacion Ángulo de rotación del objeto (en grados).
 */
struct objetoFisico {
    struct Dibujable* objeto;
    float velocidad[2];
    float aceleracion[2];
    int masa;
	int16_t rotacion;
};

/**
 * @brief Calcula la nueva posición de un objeto físico según las leyes del
 * movimiento.
 *
 * Esta función actualiza la aceleración y la velocidad del objeto en función
 * de la gravedad y del estado del propulsor. También aplica las órdenes de
 * rotación izquierda y derecha si están activadas. El objeto es desplazado en
 * el espacio y se calcula su nueva posición en el plano cartesiano.
 * 
 * También actualiza valores globales como altitud, velocidad horizontal y
 * vertical y el consumo de combustible si el propulsor está en uso.
 *
 * @param objeto Puntero al objeto físico que se va a actualizar.
 * 
 * @return struct Punto La variación de posición que debe aplicarse al objeto.
 */
struct Punto calcularFisicas(struct objetoFisico* objeto);


/**
 * @brief Aumenta la potencia del propulsor de la nave.
 */
void propulsar(void);

/**
 * @brief Activa el estado de propulsión (permite que la nave impulse).
 */
void activar_propulsor(void);

/**
 * @brief Desactiva el estado de propulsión (detiene el impulso).
 */
void desactivar_propulsor(void);

/**
 * @brief Indica que se debe girar la nave hacia la izquierda.
 */
void girar_izquierda(void);

/**
 * @brief Indica que se debe girar la nave hacia la derecha.
 */
void girar_derecha(void);

/**
 * @brief Obtiene el estado actual del propulsor.
 * 
 * @return Valor entre 0 y 3 indicando el nivel de propulsión.
 */
uint8_t obtener_propulsor(void);

/**
 * @brief Libera los recursos asociados a un objeto físico.
 * 
 * @param objeto Puntero al objeto físico a destruir.
 */
void destruir_objeto_fisico(struct objetoFisico* objeto);

void activar_ia(void);
void desactivar_ia(void);
uint8_t ia_activada(void);

#endif // FISICAS_H
