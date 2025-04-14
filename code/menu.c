#include "menu.h"
#include "palabra.h"
#include "../resources/caracteres.h"
#include "../code/transformaciones.h"
#include <stdio.h>
#include <windows.h>

// Variable internas para el menu
static OpcionMenu opcionSeleccionada = OPCION_PLAY;

// Número de opciones definido en el header (NUM_OPCIONES)
static struct Palabra* opcionesTextuales[NUM_OPCIONES] = {0};

// Esto luego se recalcula al dibujar el menú
static int menuPosX = 0;
static int menuPosY_inicial = 0;
static const int espacioEntreOpciones = 30;

// Cadena de cada opción en mayúsculas.
static const char* cadenasOpciones[NUM_OPCIONES] = {
    "PLAY",
    "TEST DIBUJABLES",
    "OPTIONS",
    "EXIT"
};

// Función auxiliar (definida en este módulo o en otro archivo de utilidades)
// ya está definida arriba: crearPalabraDesdeCadena

void inicializarMenu(void) {
    printf("Inicializando menú...\n");
    opcionSeleccionada = OPCION_PLAY;
    // Esto se actualiza al dibujar
    struct Punto origenTemporal = {0, 0};
    // Se crean los objetos Texto para cada opción a partir de sus cadenas.
    for (int i = 0; i < NUM_OPCIONES; i++) {
        printf("Creando cadena de opciones de menu...\n");
        opcionesTextuales[i] = crearPalabraDesdeCadena(cadenasOpciones[i], origenTemporal);
    }
    printf("Cadenas de menu inicializadas\n");
}

void destruirMenu(void) {
    // Libera cada objeto Texto creado para las opciones.
    for (int i = 0; i < NUM_OPCIONES; i++) {
        if (opcionesTextuales[i]) {
            destruir_palabra(opcionesTextuales[i]);
            opcionesTextuales[i] = NULL;
        }
    }
}

static void actualizarPosicionesMenu(HWND hwnd) {
    // Obtener las dimensiones del área del cliente
    HDC hdc = GetDC(hwnd);
    RECT rect;
    GetClientRect(hwnd, &rect);
    int anchoCliente = rect.right - rect.left;
    int altoCliente = rect.bottom - rect.top;
    printf("HWND = %p\n", (void*)hwnd);
    printf("rect.left   = %ld, rect.top    = %ld\n", rect.left,  rect.top);
    printf("rect.right  = %ld, rect.bottom = %ld\n", rect.right, rect.bottom);
    printf("anchoCliente = %d, altoCliente = %d\n", anchoCliente, altoCliente);
    
    // Calcular el ancho máximo de las opciones del menú
    int anchoTotalMenu = 0, anchoOpcion;
    for (int i = 0; i < NUM_OPCIONES; i++) {
        anchoOpcion = opcionesTextuales[i]->num_letras * (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER);
        if (anchoOpcion > anchoTotalMenu)
            anchoTotalMenu = anchoOpcion;
    }
    
    // Calcular la posición X para centrar el menú horizontalmente
    menuPosX = (anchoCliente - anchoTotalMenu) / 2;
    
    // Calcular la altura total del menú (cada opción + espacio entre ellas)
    int altoTotalMenu = NUM_OPCIONES * ALTURA_CARACTER_MAX + (NUM_OPCIONES - 1) * espacioEntreOpciones;
    // Calcular la posición Y para centrar verticalmente el menú
    menuPosY_inicial = (altoCliente - altoTotalMenu) / 2;
    
    // Actualizar la posición de cada opción
    for (int i = 0; i < NUM_OPCIONES; i++) {
        struct Punto nuevoOrigen = { (float)menuPosX, (float)(menuPosY_inicial + i * (ALTURA_CARACTER_MAX + espacioEntreOpciones)) };
        opcionesTextuales[i]->origen = nuevoOrigen;
        colocar_palabra(opcionesTextuales[i], nuevoOrigen);
    }
    fprintf(stderr, "Posiciones de menú actualizadas: X=%d, Y=%d\n", menuPosX, menuPosY_inicial);
    
    ReleaseDC(hwnd, hdc);
}


void dibujarMenuEnBuffer(HDC hdc, HWND hwndReal) {
    actualizarPosicionesMenu(hwndReal);
    
    // Obtener el rectángulo del área del cliente
    RECT rect;
    GetClientRect(hwndReal, &rect);
    
    // Calcular la posición del título
    int marginTitulo = 70;
    int numCaracteresTitulo = 12;
    int anchoTitulo = numCaracteresTitulo * (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER);
    int xTitulo = (rect.right - rect.left - anchoTitulo) / 2;
    int yTitulo = menuPosY_inicial - ALTURA_CARACTER_MAX - marginTitulo;
    struct Punto origenTitulo = { (float)xTitulo, (float)yTitulo };
    
    // Dibujar el título
    struct Palabra* titulo = crearPalabraDesdeCadena("LUNAR LANDER", origenTitulo);
    dibujar_palabra(titulo, hdc);
    destruir_palabra(titulo);
    
    // Dibujar cada opción del menú
    for (int i = 0; i < NUM_OPCIONES; i++) {
        // Si es la opción seleccionada, dibujar el indicador
        if (i == obtenerOpcionSeleccionada()) {
            struct Punto indicadorOrigen;
            // Colocar el indicador a la izquierda de la opcion seleccionada
            indicadorOrigen.x = opcionesTextuales[i]->origen.x - 2 * ANCHURA_CARACTER_MAX;
            indicadorOrigen.y = opcionesTextuales[i]->origen.y;
            struct Palabra* indicador = crearPalabraDesdeCadena(">", indicadorOrigen);
            dibujar_palabra(indicador, hdc);
            destruir_palabra(indicador);
        }
        // Dibujar la opción de menú
        dibujar_palabra(opcionesTextuales[i], hdc);
    }
}


LRESULT procesarEventoMenu(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if(uMsg == WM_KEYDOWN) {
        switch(wParam) {
            case VK_UP:
                if(opcionSeleccionada > OPCION_PLAY)
                    opcionSeleccionada--;
                InvalidateRect(hwnd, NULL, TRUE);  // repintado
                break;
            case VK_DOWN:
                if(opcionSeleccionada < NUM_OPCIONES - 1)
                    opcionSeleccionada++;
                InvalidateRect(hwnd, NULL, TRUE); // repintado
                break;
            default:
                break;
        }
    }
    return 0;
}

OpcionMenu obtenerOpcionSeleccionada(void) {
    return opcionSeleccionada;
}
