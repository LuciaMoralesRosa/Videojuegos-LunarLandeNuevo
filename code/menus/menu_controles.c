#include "menu_controles.h"
#include "../palabra.h"
#include "../../resources/caracteres.h"
#include "../../code/transformaciones.h"
#include "../../data/variables_globales.h"
#include <stdio.h>
#include <windows.h>


static char* campos_cadenas[CONTROL_NUM_OPCIONES] = {
    "INSERTAR MONEDA",
    "ACTIVAR PROPULSOR",
    "ROTAR IZQUIERDA",
    "ROTAR DERECHA",
    "PAUSA",
    "VOLVER AL MENU DE OPCIONES"
};

static char* campos_cadenas_valores[CONTROL_NUM_OPCIONES - 1] = {
    "5",
    "FLECHA SUPERIOR",
    "FLECHA IZQUIERDA",
    "FLECHA DERECHA",
    "ESPACIO"
};

static struct Palabra* palabra_controles = {0};
static struct Palabra* indicador = {0};

static Opcion_Menu_Controles campo_seleccionado = CONTROL_MONEDA;

static struct Palabra* campos_controles[CONTROL_NUM_OPCIONES] = {0};
static struct Palabra* campos_controles_palabras[CONTROL_NUM_OPCIONES - 1] = {0};


char* obtener_nombre_tecla_dado_ascii(int code) {
    BYTE virtualKey;
    // Si el código es imprimible, convertirlo usando VkKeyScan
    if (isprint(code)) {
        SHORT vk = VkKeyScan(code);
        if (vk == -1) return _strdup("Desconocido");
        virtualKey = LOBYTE(vk);
    } else {
        // Asumimos que ya es un virtual-key code
        virtualKey = code;
    }

    switch(code) {
        case 38: {
            return _strdup("FLECHA SUPERIOR");
            break;
        }
        case 37: {
            return _strdup("FLECHA IZQUIERDA");
            break;
        }
        case 40: {
            return _strdup("FLECHA INFERIOR");
            break;
        }
        case 39: {
            return _strdup("FLECHA DERECHA");
            break;
        }
        case 97: {
            return _strdup("1 - NUM PAD");
            break;
        }
        case 98: {
            return _strdup("2 - NUM PAD");
            break;
        }
        case 99: {
            return _strdup("3 - NUM PAD");
            break;
        }
        case 100: {
            return _strdup("4 - NUM PAD");
            break;
        }
        case 101: {
            return _strdup("5 - NUM PAD");
            break;
        }
        case 102: {
            return _strdup("6 - NUM PAD");
            break;
        }
        case 103: {
            return _strdup("7 - NUM PAD");
            break;
        }
        case 104: {
            return _strdup("8 - NUM PAD");
            break;
        }
        case 105: {
            return _strdup("9 - NUM PAD");
            break;
        }
        case 96: {
            return _strdup("0 - NUM PAD");
            break;
        }

    }

    // Casos personalizados
    switch (virtualKey) {
        case VK_UP: return _strdup("FLECHA SUPERIOR");
        case VK_LEFT: return _strdup("FLECHA IZQUIERDA");
        case VK_RIGHT: return _strdup("FLECHA DERECHA");
        case VK_SPACE: return _strdup("ESPACIO");
        case VK_RETURN: return _strdup("ENTER");
        case VK_ESCAPE: return _strdup("ESCAPE");
        case VK_SHIFT: return _strdup("SHIFT");
        case VK_CONTROL: return _strdup("CONTROL");
    }

    UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

    // Teclas extendidas
    switch (virtualKey) {
        case VK_LEFT: case VK_RIGHT: case VK_UP: case VK_DOWN:
        case VK_INSERT: case VK_DELETE: case VK_HOME: case VK_END:
        case VK_PRIOR: case VK_NEXT:
            scanCode |= 0x100;
            break;
    }

    char buffer[64];
    if (GetKeyNameText(scanCode << 16, buffer, sizeof(buffer)) == 0) {
        return _strdup("Desconocido");
    }

    return _strdup(buffer);
}


void inicializar_menu_controles(){
    palabra_controles = crear_palabra_desde_cadena("CONTROLES", (struct Punto){440, 200});
    float y = tamano_inicial_pantalla_Y / 2 - 100;
    
    indicador = crear_palabra_desde_cadena(">", (struct Punto){
        289 - 2 * ANCHURA_CARACTER_MAX, y
    });
    
    for(int i = 0; i < CONTROL_NUM_OPCIONES; i++) {
        if(i == CONTROL_NUM_OPCIONES - 1) {
            campos_controles[i] = crear_palabra_desde_cadena(campos_cadenas[i],
                (struct Punto){tam_ventana_x - (400 * factor_escalado), tam_ventana_y - (50 * factor_escalado)}
            );
        }
        else {
            campos_controles[i] = crear_palabra_desde_cadena(campos_cadenas[i],
                (struct Punto){289, y}
            );
            y = y + ALTURA_CARACTER_MAX + 30;
        }
    }
    
    
    y = tamano_inicial_pantalla_Y / 2 - 100;
    for(int i = 0; i < CONTROL_NUM_OPCIONES - 1; i++) {
        campos_controles_palabras[i] = crear_palabra_desde_cadena(campos_cadenas_valores[i],
            (struct Punto){550, y}
        );
        y = y + ALTURA_CARACTER_MAX + 30;
    }
    escalar_menu_controles(factor_escalado);
}

void escalar_menu_controles(float factor) {
    escalar_palabra_en_escena_dados_ejes(palabra_controles, factor, factor);
    escalar_palabra_en_escena_dados_ejes(indicador, factor, factor);
    for(int i = 0; i < CONTROL_NUM_OPCIONES; i++){
        escalar_palabra_en_escena_dados_ejes(campos_controles[i], factor, factor);
        if(i != CONTROL_NUM_OPCIONES - 1) {
            escalar_palabra_en_escena_dados_ejes(campos_controles_palabras[i], factor, factor);
        }
    }
}

void dibujar_menu_controles(HDC hdc, HWND hwndReal) {
    dibujar_palabra(palabra_controles, hdc);
    dibujar_palabra(indicador, hdc);

    // Dibujar cada opción del menú
    for (uint8_t i = 0; i < CONTROL_NUM_OPCIONES; i++) {
        dibujar_palabra(campos_controles[i], hdc);
    }

    for (uint8_t i = 0; i < CONTROL_NUM_OPCIONES - 1; i++) {
        dibujar_palabra(campos_controles_palabras[i], hdc);
    }
}



LRESULT procesar_pulsado_flechas_controles(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    (void)lParam; // Evita el warning

    if(uMsg == WM_KEYDOWN) {
        switch(wParam) {
            case VK_UP:
                if(campo_seleccionado > CONTROL_MONEDA)
                    campo_seleccionado--;
                InvalidateRect(hwnd, NULL, TRUE);  // repintado
                break;
            case VK_DOWN:
                if(campo_seleccionado < CONTROL_NUM_OPCIONES - 1)
                    campo_seleccionado++;
                InvalidateRect(hwnd, NULL, TRUE); // repintado
                break;
            default:
                break;
        }
    }
    colocar_palabra(indicador, (struct Punto){
        (campos_controles[campo_seleccionado] -> origen.x - 2 * ANCHURA_CARACTER_MAX) * factor_escalado,
        campos_controles[campo_seleccionado] -> origen.y * factor_escalado
    });
    return 0;
}



void establecer_tecla_control(WPARAM tecla) {
    switch(campo_seleccionado) {
        case CONTROL_MONEDA: {
            TECLA_MONEDA = tecla;
            campos_cadenas_valores[CONTROL_MONEDA] = obtener_nombre_tecla_dado_ascii(tecla);
            campos_controles_palabras[CONTROL_MONEDA] = crear_palabra_desde_cadena(
                campos_cadenas_valores[CONTROL_MONEDA],
                campos_controles_palabras[CONTROL_MONEDA]->origen
            );
            break;
        }
        case CONTROL_PROPULSOR: {
            TECLA_PROPULSOR = tecla;
            campos_cadenas_valores[CONTROL_PROPULSOR] = obtener_nombre_tecla_dado_ascii(tecla);
            campos_controles_palabras[CONTROL_PROPULSOR] = crear_palabra_desde_cadena(
                campos_cadenas_valores[CONTROL_PROPULSOR],
                campos_controles_palabras[CONTROL_PROPULSOR]->origen
            );
            break;
        }
        case CONTROL_IZDA: {
            TECLA_ROTAR_IZDA = tecla;
            campos_cadenas_valores[CONTROL_IZDA] = obtener_nombre_tecla_dado_ascii(tecla);
            campos_controles_palabras[CONTROL_IZDA] = crear_palabra_desde_cadena(
                campos_cadenas_valores[CONTROL_IZDA],
                campos_controles_palabras[CONTROL_IZDA]->origen
            );
            break;
        }
        case CONTROL_DCHA: {
            TECLA_ROTAR_DCHA = tecla;
            campos_cadenas_valores[CONTROL_DCHA] = obtener_nombre_tecla_dado_ascii(tecla);
            campos_controles_palabras[CONTROL_DCHA] = crear_palabra_desde_cadena(
                campos_cadenas_valores[CONTROL_DCHA],
                campos_controles_palabras[CONTROL_DCHA]->origen
            );
            break;
        }
        case CONTROL_PAUSA: {
            TECLA_PAUSA = tecla;
            campos_cadenas_valores[CONTROL_PAUSA] = obtener_nombre_tecla_dado_ascii(tecla);
            campos_controles_palabras[CONTROL_PAUSA] = crear_palabra_desde_cadena(
                campos_cadenas_valores[CONTROL_PAUSA],
                campos_controles_palabras[CONTROL_PAUSA]->origen
            );
            break;
        }
        default:
            break;
    }
}

void gestionar_opcion_seleccionada_controles(void) {
    if(campo_seleccionado != CONTROL_VOLVER) {
        switch(campo_seleccionado) {
            case CONTROL_MONEDA: {
                break;
            }
            case CONTROL_PROPULSOR: {
                break;
            }
            case CONTROL_IZDA: {
                break;
            }
            case CONTROL_DCHA: {
                break;
            }
            case CONTROL_PAUSA: {
                break;
            }
            default:
                break;
        }
    }
}

Opcion_Menu_Controles obtener_opcion_seleccionada_controles(void) {
    return campo_seleccionado;
}

void destruir_menu_controles(void) {
    // Libera cada objeto creado para las opciones.
    for (int i = 0; i < CONTROL_NUM_OPCIONES; i++) {
        if (campos_controles[i]) {
            destruir_palabra(campos_controles[i]);
            campos_controles[i] = NULL;
        }
    }
    for (int i = 0; i < CONTROL_NUM_OPCIONES - 1; i++) {
        if (campos_controles_palabras[i]) {
            destruir_palabra(campos_controles_palabras[i]);
            campos_controles_palabras[i] = NULL;
        }
    }
    destruir_palabra(palabra_controles);
    destruir_palabra(indicador);
}
