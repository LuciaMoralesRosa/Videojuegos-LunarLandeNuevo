#include "menu_opciones.h"
#include "../palabra.h"
#include "../../resources/caracteres.h"
#include "../../code/transformaciones.h"
#include "../../data/variables_globales.h"
#include <stdio.h>
#include <windows.h>

/**
 * @brief Variables internas del menú de opciones.
 *
 * Estas variables controlan el estado y el contenido visual del menú de
 * configuración de partida.
 */

// Opción actualmente seleccionada por el jugador en el menú.
static Opcion_Menu campo_seleccionado = MISSION;

// Tipo de misión actualmente seleccionado.
static Tipo_Mision tipo_mision = TRAINING;

// Tipo de terreno actualmente seleccionado
static Tipo_Terreno tipo_terreno = ORIGINAL;


// Flag que indica si los asteroides están activados (1) o no (0).
//static uint8_t asteroides_activados = 0;
// Flag que indica si las torretas están activadas (1) o no (0).
//static uint8_t torretas_activadas = 0;

// Flag que indica si el modo superfacil esta activado (1) o no (0).
static uint8_t modo_superfacil = 0;

// Arreglo de punteros a las palabras que representan las opciones del menú.
static struct Palabra* campos_menu[NUM_OPCIONES] = {0};

// Título principal del menú 
static struct Palabra* titulo_menu = {0};

// Indicador gráfico que señala la opción actualmente seleccionada.
static struct Palabra* indicador = {0};

static struct Palabra* espacio_continuar = {0};

/**
 * @brief Cadenas de texto utilizadas para crear cada opción del menú.
 * 
 * Cambian en tiempo de ejecucion segun las selecciones del jugador
 */
// static char* campos_cadenas[NUM_OPCIONES] = {
//     "MISSION: TRAINING MISSION",
//     "*  ACTIVE ASTEROIDS",
//     "*  ACTIVE TURRETS",
//     "EXIT"
// };
static char* campos_cadenas[NUM_OPCIONES] = {
    "MISSION: TRAINING MISSION",
    "TERRENO: ORIGINAL",
    "*  MODO SUPERFACIL",
    "EXIT"
};

void inicializar_menu_nueva_partida(void) {
    campo_seleccionado = MISSION;
    float y = tamano_inicial_pantalla_Y / 2 - 100;
    
    titulo_menu = crear_palabra_desde_cadena("LUNAR LANDER", (struct Punto){
        420, 200
    });
    indicador = crear_palabra_desde_cadena(">", (struct Punto){
        350 - 2 * ANCHURA_CARACTER_MAX, y
    });
    espacio_continuar = crear_palabra_desde_cadena("PRESIONE EL ESPACIO PARA CONTINUAR", (struct Punto){
        275, 600
    });
    for (int i = 0; i < NUM_OPCIONES; i++) {
        campos_menu[i] = crear_palabra_desde_cadena(campos_cadenas[i],
            (struct Punto){350, y}
        );
        y = y + ALTURA_CARACTER_MAX + 30;
    }

    escalar_menu_opciones(factor_escalado);
}

void dibujar_menu_opciones(HDC hdc, HWND hwndReal) {    
    dibujar_palabra(titulo_menu, hdc);
    dibujar_palabra(indicador, hdc);
    dibujar_palabra(espacio_continuar, hdc);

    // Dibujar cada opción del menú
    for (uint8_t i = 0; i < NUM_OPCIONES; i++) {
        dibujar_palabra(campos_menu[i], hdc);
    }
}

void escalar_menu_opciones(float factor){
    escalar_palabra_en_escena_dados_ejes(titulo_menu, factor, factor);
    escalar_palabra_en_escena_dados_ejes(indicador, factor, factor);
    escalar_palabra_en_escena_dados_ejes(espacio_continuar, factor, factor);

    for(uint8_t i = 0; i < NUM_OPCIONES; i++) {
        escalar_palabra_en_escena_dados_ejes(campos_menu[i], factor, factor);
    }
}


LRESULT procesar_pulsado_flechas(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    (void)lParam; // Evita el warning

    if(uMsg == WM_KEYDOWN) {
        switch(wParam) {
            case VK_UP:
                if(campo_seleccionado > MISSION)
                    campo_seleccionado--;
                InvalidateRect(hwnd, NULL, TRUE);  // repintado
                break;
            case VK_DOWN:
                if(campo_seleccionado < NUM_OPCIONES - 1)
                    campo_seleccionado++;
                InvalidateRect(hwnd, NULL, TRUE); // repintado
                break;
            default:
                break;
        }
    }
    colocar_palabra(indicador, (struct Punto){
        campos_menu[campo_seleccionado] -> origen.x - 2 * ANCHURA_CARACTER_MAX,
        campos_menu[campo_seleccionado] -> origen.y
    });
    return 0;
}

Opcion_Menu obtener_opcion_seleccionada(void) {
    return campo_seleccionado;
}

Tipo_Mision obtener_tipo_mision(void) {
    return tipo_mision;
}

Tipo_Terreno obtener_tipo_terreno(void) {
    return tipo_terreno;
}

/*
// VERSION CON TORRETAS Y ASTEROIDES
void gestionar_opcion_seleccionada(void) {
    switch (campo_seleccionado) {
    case MISSION:{
        struct Punto p = campos_menu[0] -> origen;
        destruir_palabra(campos_menu[0]);
        switch (tipo_mision) {
        case TRAINING:
            campos_cadenas[0] = "MISSION: CADET MISSION";
            campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], p);
            tipo_mision = CADET;
            break;
        case CADET:
            campos_cadenas[0] = "MISSION: PRIME MISSION";
            campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], p);
            tipo_mision = PRIME;
            break;
        case PRIME:
            campos_cadenas[0] = "MISSION: COMMAND MISSION";
            campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], p);
            tipo_mision = COMMAND;
            break;
        case COMMAND:
            campos_cadenas[0] = "MISSION: TRAINING MISSION";
            campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], p);
            tipo_mision = TRAINING;
            break;        
        default:
            break;
        }
        break;
    }
    case ASTEROIDS: {
        struct Punto p1 = campos_menu[1] -> origen;
        destruir_palabra(campos_menu[1]);
        if(asteroides_activados == 0) {
            campos_cadenas[1] = "+  ACTIVE ASTEROIDS";
            campos_menu[1] = crear_palabra_desde_cadena(campos_cadenas[1], p1);
            asteroides_activados = 1;
        } else {
            campos_cadenas[1] = "*  ACTIVE ASTEROIDS";
            campos_menu[1] = crear_palabra_desde_cadena(campos_cadenas[1], p1);
            asteroides_activados = 0;
        }
        break; 
    }       
    case TURRETS: {
        struct Punto p2 = campos_menu[2] -> origen;
        destruir_palabra(campos_menu[2]);
        if(torretas_activadas == 0) {
            campos_cadenas[2] = "+  ACTIVE TURRETS";
            campos_menu[2] = crear_palabra_desde_cadena(campos_cadenas[2], p2);
            torretas_activadas = 1;
        } else {
            campos_cadenas[2] = "*  ACTIVE TURRETS";
            campos_menu[2] = crear_palabra_desde_cadena(campos_cadenas[2], p2);
            torretas_activadas = 0;
        }
        break;
    }
    default:
        break;
    }
}
*/



void gestionar_opcion_seleccionada(void) {
    switch (campo_seleccionado) {
        case MISSION: {
            struct Punto p = campos_menu[0] -> origen;
            destruir_palabra(campos_menu[0]);
            switch (tipo_mision) {
                case TRAINING:
                    campos_cadenas[0] = "MISSION: CADET MISSION";
                    campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], p);
                    tipo_mision = CADET;
                    break;
                case CADET:
                    campos_cadenas[0] = "MISSION: PRIME MISSION";
                    campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], p);
                    tipo_mision = PRIME;
                    break;
                case PRIME:
                    campos_cadenas[0] = "MISSION: COMMAND MISSION";
                    campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], p);
                    tipo_mision = COMMAND;
                    break;
                case COMMAND:
                    campos_cadenas[0] = "MISSION: TRAINING MISSION";
                    campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], p);
                    tipo_mision = TRAINING;
                    break;        
                default:
                    break;
            }
            if(tipo_mision != TRAINING) {
                if(modo_superfacil == 1) {
                    destruir_palabra(campos_menu[2]);
                    // Si se activa un modo distinto al TRAINING se debe desactivar el modo superfacil
                    modo_superfacil = 0;
                    campos_cadenas[2] = "*  MODO SUPER FACIL";
                    campos_menu[2] = crear_palabra_desde_cadena(campos_cadenas[2], campos_menu[2] -> origen);
                }
            }
            break;
        }
        case TERRENO: {
            struct Punto p1 = campos_menu[1] -> origen;
            destruir_palabra(campos_menu[1]);
            switch(tipo_terreno) {
                case ORIGINAL: 
                    campos_cadenas[1] = "TERRENO: FACIL";
                    campos_menu[1] = crear_palabra_desde_cadena(campos_cadenas[1], p1);
                    tipo_terreno = FACIL;
                    break;
                case FACIL:
                    campos_cadenas[1] = "TERRENO: ORIGINAL";
                    campos_menu[1] = crear_palabra_desde_cadena(campos_cadenas[1], p1);
                    tipo_terreno = ORIGINAL;
                    if(modo_superfacil == 1) {
                        destruir_palabra(campos_menu[2]);
                        // Si se activa un terreno distinto al FACIL se debe desactivar el modo superfacil
                        modo_superfacil = 0;
                        campos_cadenas[2] = "*  MODO SUPERFACIL";
                        campos_menu[2] = crear_palabra_desde_cadena(campos_cadenas[2], campos_menu[2] -> origen);
                    }
                    break;
                default:
                    break;
            }
            break; 
        }       
        case SUPERFACIL: {
            struct Punto p2 = campos_menu[2] -> origen;
            destruir_palabra(campos_menu[2]);
            if(modo_superfacil == 0) {
                campos_cadenas[2] = "+  MODO SUPERFACIL";
                campos_menu[2] = crear_palabra_desde_cadena(campos_cadenas[2], p2);
                modo_superfacil = 1;

                // Establecer terreno y mision al nivel mas facil
                destruir_palabra(campos_menu[1]);
                tipo_terreno = FACIL;
                campos_cadenas[1] = "TERRENO: FACIL";
                campos_menu[1] = crear_palabra_desde_cadena(campos_cadenas[1], campos_menu[1] -> origen);

                destruir_palabra(campos_menu[0]);
                tipo_mision = TRAINING; 
                campos_cadenas[0] = "MISSION: TRAINING MISSION";
                campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], campos_menu[0] -> origen);
            } else {
                campos_cadenas[2] = "*  MODO SUPERFACIL";
                campos_menu[2] = crear_palabra_desde_cadena(campos_cadenas[2], p2);
                modo_superfacil= 0;
            }
            break;
        }
        default:
            break;
    }
}


void destruir_menu_opciones(void) {
    // Libera cada objeto creado para las opciones.
    for (int i = 0; i < NUM_OPCIONES; i++) {
        if (campos_menu[i]) {
            destruir_palabra(campos_menu[i]);
            campos_menu[i] = NULL;
        }
    }
    destruir_palabra(titulo_menu);
    destruir_palabra(indicador);
    destruir_palabra(espacio_continuar);
}