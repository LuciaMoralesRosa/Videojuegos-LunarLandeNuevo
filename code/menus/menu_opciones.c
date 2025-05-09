#include "menu_opciones.h"
#include "../palabra.h"
#include "../../resources/caracteres.h"
#include "../../code/transformaciones.h"
#include "../../data/variables_globales.h"
#include "../../data/variables_juego.h"
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

// Explicacion de la misision seleccionada actualmente
static struct Palabra* explicacion_mision = {0};

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
    "MISION: ENTRENAMIENTO",
    "TERRENO: ORIGINAL",
    "*  MODO SUPERFACIL",
    "CONTROLES",
    "EXIT"
};

static char* explicacion_training = "GRAVEDAD LIGERA";
static char* explicacion_cadet = "GRAVEDAD MODERADA";
static char* explicacion_prime = "GRAVEDAD FUERTE";
static char* explicacion_command = "GRAVEDAD APLASTANTE";


void inicializar_menu_nueva_partida(void) {
    campo_seleccionado = MISSION;
    float y = tamano_inicial_pantalla_Y / 2 - 100;
    
    titulo_menu = crear_palabra_desde_cadena("LUNAR LANDER", (struct Punto){
        420, 200
    });
    escalar_palabra_centrada(titulo_menu, 1.5);
    indicador = crear_palabra_desde_cadena(">", (struct Punto){
        350 - 2 * ANCHURA_CARACTER_MAX, y
    });
    espacio_continuar = crear_palabra_desde_cadena("PRESIONE EL ESPACIO PARA CONTINUAR", (struct Punto){
        275, 600
    });

    explicacion_mision = crear_palabra_desde_cadena(explicacion_training, (struct Punto){
        470, y + ALTURA_CARACTER_MAX + 15
    });

    for (int i = 0; i < NUM_OPCIONES; i++) {
        if(i == NUM_OPCIONES - 1) {
            campos_menu[i] = crear_palabra_desde_cadena(campos_cadenas[i],
                (struct Punto){tam_ventana_x - (100 * factor_escalado), tam_ventana_y - (50 * factor_escalado)}
            );
        }
        else{
            campos_menu[i] = crear_palabra_desde_cadena(campos_cadenas[i],
                (struct Punto){350, y}
            );
            y = y + ALTURA_CARACTER_MAX + 30;
            if(i == 0) {
                y = y + ALTURA_CARACTER_MAX + 30;
            }
        }
    }

    escalar_menu_opciones(factor_escalado);
}


void dibujar_menu_opciones(HDC hdc, HWND hwndReal) {    
    dibujar_palabra(titulo_menu, hdc);
    dibujar_palabra(indicador, hdc);
    dibujar_palabra(espacio_continuar, hdc);
    dibujar_palabra(explicacion_mision, hdc);

    // Dibujar cada opción del menú
    for (uint8_t i = 0; i < NUM_OPCIONES; i++) {
        dibujar_palabra(campos_menu[i], hdc);
    }
}

void escalar_menu_opciones(float factor){
    escalar_palabra_en_escena_dados_ejes(titulo_menu, factor, factor);
    escalar_palabra_en_escena_dados_ejes(indicador, factor, factor);
    escalar_palabra_en_escena_dados_ejes(espacio_continuar, factor, factor);
    escalar_palabra_en_escena_dados_ejes(explicacion_mision, factor, factor);

    for(uint8_t i = 0; i < NUM_OPCIONES; i++) {
        escalar_palabra_en_escena_dados_ejes(campos_menu[i], factor, factor);
    }

}


LRESULT procesar_pulsado_flechas(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    (void)lParam; // Evita el warning

    if(uMsg == WM_KEYDOWN) {
        switch(wParam) {
            case VK_UP:
                if(campo_seleccionado > MISSION){
                    campo_seleccionado--;
                    if(partida_empezada == 1)  {
                        if(campo_seleccionado == SUPERFACIL){
                            campo_seleccionado--;
                        }
                        if(campo_seleccionado == TERRENO){
                            campo_seleccionado--;
                        }
                    }
                }
                InvalidateRect(hwnd, NULL, TRUE);  // repintado
                break;
            case VK_DOWN:
                if(campo_seleccionado < NUM_OPCIONES - 1){
                    campo_seleccionado++;
                    if(partida_empezada == 1) {
                        if(campo_seleccionado == TERRENO){
                            campo_seleccionado++;
                        }
                        if(campo_seleccionado == SUPERFACIL){
                            campo_seleccionado++;
                        }
                    }
                }
                InvalidateRect(hwnd, NULL, TRUE); // repintado
                break;
            default:
                break;
        }
    }
    colocar_palabra(indicador, (struct Punto){
        (campos_menu[campo_seleccionado] -> origen.x - 2 * ANCHURA_CARACTER_MAX) * factor_escalado,
        campos_menu[campo_seleccionado] -> origen.y * factor_escalado
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




void gestionar_opcion_seleccionada(void) {
    switch (campo_seleccionado) {
        case MISSION: {
            struct Punto p = campos_menu[0] -> origen;
            destruir_palabra(campos_menu[0]);
            switch (tipo_mision) {
                case TRAINING:
                    campos_cadenas[0] = "MISION: CADETE";
                    explicacion_mision = crear_palabra_desde_cadena(explicacion_cadet, (struct Punto) {
                        (p.x + 120),
                        (p.y + ALTURA_CARACTER_MAX + 15)
                    });
                    campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], p);
                    escalar_palabra_en_escena_dados_ejes(campos_menu[0], factor_escalado, factor_escalado);
                    escalar_palabra_en_escena_dados_ejes(explicacion_mision, factor_escalado, factor_escalado);
                    tipo_mision = CADET;
                    break;

                case CADET:
                    campos_cadenas[0] = "MISION: TENIENTE";
                    explicacion_mision = crear_palabra_desde_cadena(explicacion_prime, (struct Punto) {
                        (p.x + 120),
                        (p.y + ALTURA_CARACTER_MAX + 15)
                    });
                    campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], p);
                    escalar_palabra_en_escena_dados_ejes(campos_menu[0], factor_escalado, factor_escalado);
                    escalar_palabra_en_escena_dados_ejes(explicacion_mision, factor_escalado, factor_escalado);
                    tipo_mision = PRIME;
                    break;

                case PRIME:
                    campos_cadenas[0] = "MISION: COMANDANTE";
                    explicacion_mision = crear_palabra_desde_cadena(explicacion_command, (struct Punto) {
                        (p.x + 120),
                        (p.y + ALTURA_CARACTER_MAX + 15)
                    });
                    campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], p);
                    escalar_palabra_en_escena_dados_ejes(campos_menu[0], factor_escalado, factor_escalado);
                    escalar_palabra_en_escena_dados_ejes(explicacion_mision, factor_escalado, factor_escalado);
                    tipo_mision = COMMAND;
                    break;

                case COMMAND:
                    campos_cadenas[0] = "MISION: ENTRENAMIENTO";
                    explicacion_mision = crear_palabra_desde_cadena(explicacion_training, (struct Punto) {
                        (p.x + 120),
                        (p.y + ALTURA_CARACTER_MAX + 15)
                    });
                    campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], p);
                    escalar_palabra_en_escena_dados_ejes(campos_menu[0], factor_escalado, factor_escalado);
                    escalar_palabra_en_escena_dados_ejes(explicacion_mision, factor_escalado, factor_escalado);
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
                    campos_cadenas[2] = "*  MODO SUPERFACIL";
                    campos_menu[2] = crear_palabra_desde_cadena(campos_cadenas[2], campos_menu[2] -> origen);
                    escalar_palabra_en_escena_dados_ejes(campos_menu[2], factor_escalado, factor_escalado);
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
                    escalar_palabra_en_escena_dados_ejes(campos_menu[1], factor_escalado, factor_escalado);
                    tipo_terreno = FACIL;
                    break;
                case FACIL:
                    campos_cadenas[1] = "TERRENO: ORIGINAL";
                    campos_menu[1] = crear_palabra_desde_cadena(campos_cadenas[1], p1);
                    escalar_palabra_en_escena_dados_ejes(campos_menu[1], factor_escalado, factor_escalado);
                    tipo_terreno = ORIGINAL;
                    if(modo_superfacil == 1) {
                        destruir_palabra(campos_menu[2]);
                        // Si se activa un terreno distinto al FACIL se debe desactivar el modo superfacil
                        modo_superfacil = 0;
                        campos_cadenas[2] = "*  MODO SUPERFACIL";
                        campos_menu[2] = crear_palabra_desde_cadena(campos_cadenas[2], campos_menu[2] -> origen);
                        escalar_palabra_en_escena_dados_ejes(campos_menu[1], factor_escalado, factor_escalado);
                        escalar_palabra_en_escena_dados_ejes(campos_menu[2], factor_escalado, factor_escalado);
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
                escalar_palabra_en_escena_dados_ejes(campos_menu[2], factor_escalado, factor_escalado);
                modo_superfacil = 1;

                // Establecer terreno y mision al nivel mas facil
                destruir_palabra(campos_menu[1]);
                tipo_terreno = FACIL;
                campos_cadenas[1] = "TERRENO: FACIL";
                campos_menu[1] = crear_palabra_desde_cadena(campos_cadenas[1], campos_menu[1] -> origen);
                escalar_palabra_en_escena_dados_ejes(campos_menu[1], factor_escalado, factor_escalado);

                tipo_mision = TRAINING; 
                campos_cadenas[0] = "MISION: ENTRENAMIENTO";
                explicacion_mision = crear_palabra_desde_cadena(explicacion_training, (struct Punto) {
                    (campos_menu[0]->origen.x + 120),
                    (campos_menu[0]->origen.y + ALTURA_CARACTER_MAX + 15)
                });
                campos_menu[0] = crear_palabra_desde_cadena(campos_cadenas[0], campos_menu[0] -> origen);
                escalar_palabra_en_escena_dados_ejes(explicacion_mision, factor_escalado, factor_escalado);
                escalar_palabra_en_escena_dados_ejes(campos_menu[0], factor_escalado, factor_escalado);
            } else {
                campos_cadenas[2] = "*  MODO SUPERFACIL";
                campos_menu[2] = crear_palabra_desde_cadena(campos_cadenas[2], p2);
                escalar_palabra_en_escena_dados_ejes(campos_menu[2], factor_escalado, factor_escalado);
                modo_superfacil= 0;
            }
            break;
        }
        case CONTROLES: {
            printf("Caso controles\n");
            tipo_mision_seleccionado = tipo_mision;
            tipo_terreno_seleccionado = tipo_terreno;
            opcion_menu_seleccionado = campo_seleccionado;
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


void retornar_menu_opciones(void) {
    float y = tamano_inicial_pantalla_Y / 2 - 100;
    
    titulo_menu = crear_palabra_desde_cadena("LUNAR LANDER", (struct Punto){
        420, 200
    });
    escalar_palabra_centrada(titulo_menu, 1.5);
    indicador = crear_palabra_desde_cadena(">", (struct Punto){
        350 - 2 * ANCHURA_CARACTER_MAX, y
    });
    espacio_continuar = crear_palabra_desde_cadena("PRESIONE EL ESPACIO PARA CONTINUAR", (struct Punto){
        275, 600
    });

    explicacion_mision = crear_palabra_desde_cadena(explicacion_training, (struct Punto){
        470, y + ALTURA_CARACTER_MAX + 15
    });

    for (int i = 0; i < NUM_OPCIONES; i++) {
        if(i == NUM_OPCIONES - 1) {
            campos_menu[i] = crear_palabra_desde_cadena(campos_cadenas[i],
                (struct Punto){tam_ventana_x - (100 * factor_escalado), tam_ventana_y - (50 * factor_escalado)}
            );
        }
        else{
            campos_menu[i] = crear_palabra_desde_cadena(campos_cadenas[i],
                (struct Punto){350, y}
            );
            y = y + ALTURA_CARACTER_MAX + 30;
            if(i == 0) {
                y = y + ALTURA_CARACTER_MAX + 30;
            }
        }
    }

    escalar_menu_opciones(factor_escalado);
}