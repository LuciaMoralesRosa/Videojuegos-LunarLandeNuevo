#ifndef VARIABLES_GLOBALES_H
#define VARIABLES_GLOBALES_H

#include <stdint.h>


extern float tam_ventana_x;
extern float tam_ventana_y;
extern float factor_escalado; 

// Valor de la aceleración gravitatoria en m/ms² (negativa por ser hacia abajo).
extern float gravedad_m_ms;
extern int friccion_atmosfera_activada;

extern int modo_ia_activado;
extern int partida_empezada;

/**
 * @enum Tipo_Mision
 * @brief Tipos posibles de misión seleccionables en el menú.
 */
typedef enum {
    ORIGINAL, ///< Terreno original del juego.
    FACIL     ///< Terreno bastante mas sencillo.
} Tipo_Terreno;


typedef enum {
    MISSION,     ///< Selección del tipo de misión.
    TERRENO,   ///< Activación/desactivación de asteroides.
    SUPERFACIL,     ///< Activación/desactivación de torretas.
    CONTROLES,
    EXIT,        ///< Salir del menú de opciones.
    NUM_OPCIONES ///< Total de opciones.
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


extern Tipo_Terreno tipo_terreno_seleccionado;
extern Tipo_Mision tipo_mision_seleccionado;
extern Opcion_Menu opcion_menu_seleccionado;

extern int TECLA_MONEDA;
extern int TECLA_PROPULSOR;
extern int TECLA_ROTAR_IZDA;
extern int TECLA_ROTAR_DCHA;
extern int TECLA_PAUSA;

#endif // VARIABLES_GLOBALES_H