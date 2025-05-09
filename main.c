#include "code/lunar_lander.h"
#include "code/gestor_plataformas.h"
#include "code/palabra.h"
#include "code/fragmentacion_nave.h"

#include "code/menus/menu_insertar_moneda.h"
#include "code/menus/menu_opciones.h"
#include "code/menus/menu_controles.h"
#include "code/menus/cabecera_juego.h"
#include "code/menus/menu_aterrizaje.h"
#include "code/menus/menu_final_partida.h"

#include "resources/superficie_lunar.h"
#include "resources.h"

#include "data/variables_globales.h"
#include "data/constantes.h"
#include "data/variables_juego.h"

#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>

#include "ia/modelo.h"

#define timer_TICK_juego 1
#define timer_IA 2
#define timer_segundo 3
#define timer_mostrar_mensajes 4


// ---------------------------- VARIABLES ESCALADO -----------------------------
// Factor por el que escalar la escena
uint8_t fullscreen = 0;
uint8_t esc_presionado = 0;
// Rectangulo que contiene la ventana anterior al resize de pantalla completa
RECT rectVentanaAnterior;
// -----------------------------------------------------------------------------

// VARIABLES MAQUINA ESTADOS JUEGO
uint8_t moneda_presionada = 0;
uint8_t monedas_introducidas = 0;
int timestamp_pintar_mensaje = 0;

Tipo_Mision mision = TRAINING;
Tipo_Terreno terreno = ORIGINAL;
uint8_t modo_superfacil = 0;

struct Punto* p1 = NULL;
struct Punto* p2 = NULL;
struct Punto* p3 = NULL;
struct Punto* p4 = NULL;

int ia = 1;

// Controles personalizados
int esperando_que_pulse_tecla = 0;


float minimo(float a, float b) {
    return (a < b) ? a : b;
}

void AttachConsoleToStdout() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    // Obtener el handle de la ventana de la consola
    HWND consoleWindow = GetConsoleWindow();
    if (consoleWindow != NULL) {
        // Mover la ventana a la posición (X=100, Y=100) con ancho y alto predeterminados
        RECT r;
        GetWindowRect(consoleWindow, &r); // obtener el tamaño actual
        MoveWindow(consoleWindow, 1025, 0, r.right - r.left, r.bottom - r.top, TRUE);
    }
}

void repintar_ventana(HWND hwnd) {
    InvalidateRect(hwnd, NULL, TRUE); // Enviar mensaje WM_PAINT para repintar
}



// Funcion utilizada en la opcion de test de dibujables
void pruebasDibujables(HDC hdcMem) {
}

void inicializar_puntos() {
    p1 = malloc(sizeof(struct Punto));
    p2 = malloc(sizeof(struct Punto));
    p3 = malloc(sizeof(struct Punto));
    p4 = malloc(sizeof(struct Punto));
}


// Algoritmo de Bresenham para rasterizar una línea
void dibujar_linea(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        SetPixel(hdc, x1, y1, color);
        if (x1 == x2 && y1 == y2)
            break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

void dibujar_bordes(HDC hdc) {
    if (!p1 || !p2 || !p3 || !p4)
        return;
    dibujar_linea(hdc, p1->x, p1->y, p2->x, p2->y, RGB(255, 255, 255));
    dibujar_linea(hdc, p3->x, p3->y, p4->x, p4->y, RGB(255, 255, 255));
}

void escalar_textos(HWND hwnd, float factor){

    switch(estado_actual) {
        case ESTADO_PIDIENDO_MONEDA:{
            escalar_menu_insertar_moneda(factor);
            break;
        }
        case ESTADO_OPCIONES:{
            escalar_menu_opciones(factor);

            break;
        }
        case ESTADO_CONTROLES: {
            escalar_menu_controles(factor);
            break;
        }
        case ESTADO_JUEGO:{
            escalar_cabecera(factor);
            break;
        }
        case ESTADO_ATERRIZAJE:{
            escalar_cabecera(factor);
            escalar_menu_aterrizaje(factor);
            break;
        }
        case ESTADO_FIN_PARTIDA:{

            break;
        }
    }
    repintar_ventana(hwnd);
}


/**
 * Escala la escena al tamaño de la ventana
 */
void escalar_ventana(HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    int ancho_cliente = rect.right - rect.left;
    int alto_cliente = rect.bottom - rect.top;
    tam_ventana_x = (float) ancho_cliente;
    tam_ventana_y = (float) alto_cliente;
    float factor_resized_x = (float)ancho_cliente / tamano_inicial_pantalla_X;
    float factor_resized_y = (float)alto_cliente / tamano_inicial_pantalla_Y;
    
    escalar_escena(1/factor_escalado, 1/factor_escalado);
    escalar_textos(hwnd, 1/factor_escalado);
    factor_escalado = minimo(factor_resized_x, factor_resized_y);
    escalar_escena(factor_escalado, factor_escalado);
    escalar_textos(hwnd, factor_escalado);
    
    int tam_escena_x = (int)(tamano_inicial_pantalla_X * factor_escalado);
    int tam_escena_y = (int)(tamano_inicial_pantalla_Y * factor_escalado);
    
    if (!p1 || !p2 || !p3 || !p4) {
        printf("Error al asignar memoria.\n");
        return;
    }
    
    *p1 = (struct Punto){0, tam_escena_y + 1};
    *p2 = (struct Punto){tam_escena_x + 1, tam_escena_y + 1};
    *p3 = (struct Punto){tam_escena_x + 1, 0};
    *p4 = (struct Punto){tam_escena_x + 1, tam_escena_y + 1};
}


void iniciar_nueva_partida(HWND hwnd) {
    monedas_introducidas = 0;
    crear_palabra_insertar_moneda();
    estado_actual = ESTADO_PIDIENDO_MONEDA;
    pulsar_tecla(ESPACIO);
    iniciar_partida(monedas_introducidas, mision, terreno);
    repintar_ventana(hwnd);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            SetTimer(hwnd, timer_TICK_juego, intervalo_fisicas_ms, NULL);
            SetTimer(hwnd, timer_IA, 10000, NULL);
            SetTimer(hwnd, timer_segundo, 1000, NULL);
            break;
        }
        
        case WM_SYSCOMMAND: {
            if ((wParam & 0xFFF0) == SC_RESTORE) {
                if (fullscreen == 1 && esc_presionado == 1) {
                    fullscreen = 0;
                    SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                    SetWindowPos(
                        hwnd, NULL,
                        rectVentanaAnterior.left,
                        rectVentanaAnterior.top,
                        rectVentanaAnterior.right - rectVentanaAnterior.left,
                        rectVentanaAnterior.bottom - rectVentanaAnterior.top,
                        SWP_NOZORDER | SWP_FRAMECHANGED
                    );
                } else if (esc_presionado == 1) {
                    esc_presionado = 0;
                    return 0;
                }
                esc_presionado = 0;
            } else if ((wParam & 0xFFF0) == SC_MAXIMIZE) {
                fullscreen = 1;
                GetWindowRect(hwnd, &rectVentanaAnterior);
                SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
                escalar_ventana(hwnd);
            }
            break;
        }

        case WM_GETMINMAXINFO: {
            MINMAXINFO* mmi = (MINMAXINFO*)lParam;
            RECT rc = {0, 0, anchura_minima_ventana, altura_minima_ventana};
            AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
            mmi->ptMinTrackSize.x = rc.right - rc.left;
            mmi->ptMinTrackSize.y = rc.bottom - rc.top;
            break;
        }

        case WM_SIZE: {
            escalar_ventana(hwnd);
            break;
        }

        case WM_TIMER: {
            switch(estado_actual) {
                case ESTADO_PIDIENDO_MONEDA: {
                    if(wParam == timer_IA) {
                        // Gestionar partida ia
                        printf("El tiempo de espera ha vencido. Pasando a piloto automatico\n");
                        destruir_menu_insertar_moneda();
                        iniciar_partida(1, mision, terreno);
                        modo_ia_activado = 1;
                        estado_actual = ESTADO_JUEGO;
                    }
                    if (wParam == timer_TICK_juego) {
                        manejar_instante();
                        InvalidateRect(hwnd, NULL, FALSE);
                    }
                    break;
                }
                case ESTADO_JUEGO: {
                    if (wParam == timer_TICK_juego) {
                        manejar_instante();
                        manejar_teclas();
                        InvalidateRect(hwnd, NULL, FALSE);
                    }
                    if(wParam == timer_segundo) {
                        actualizar_segundos_cabecera();
                    }
                    break;
                }
                case ESTADO_ATERRIZAJE:{
                    if (wParam == timer_TICK_juego) {
                        if(tipo_aterrizaje == COLISION){
                            fisicas_fragmentos();
                            InvalidateRect(hwnd, NULL, FALSE);
                        }
                    }
                    break;
                }
                default: break;
            }
        break;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HDC hdcMem = CreateCompatibleDC(hdc);
            RECT rect;
            GetClientRect(hwnd, &rect);
            HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
            HGDIOBJ hOld = SelectObject(hdcMem, hbmMem);
            HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdcMem, &rect, brush);
            DeleteObject(brush);

            dibujar_bordes(hdcMem);
            switch(estado_actual) {
                case ESTADO_PIDIENDO_MONEDA: {
                    // Gestionar IA de fondo
                    pintar_pantalla(hdcMem);
                    // Pintar peticion
                    if((timestamp_pintar_mensaje % 25) > 7) {
                        mostrar_insertar_moneda(hdcMem);
                    }
                    timestamp_pintar_mensaje++;
                    break;
                }
                case ESTADO_OPCIONES: {
                    dibujar_menu_opciones(hdcMem, hwnd);
                    break;
                }
                case ESTADO_CONTROLES: {
                    printf("PAINT - Estado controles\n");
                    dibujar_menu_controles(hdcMem, hwnd);
                    break;
                }
                case ESTADO_JUEGO: {
                    pintar_pantalla(hdcMem);
                    break;
                }
                case ESTADO_ATERRIZAJE: {
                    pintar_pantalla(hdcMem);
                    dibujar_mensajes_aterrizaje(hdcMem);
                    break;
                }
                case ESTADO_FIN_PARTIDA: {
                    dibujar_mensajes_final_partida(hdcMem);
                    break;
                }
                default: break;
            }

            pruebasDibujables(hdcMem);
                        
            BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);
            SelectObject(hdcMem, hOld);
            DeleteObject(hbmMem);
            DeleteDC(hdcMem);
            EndPaint(hwnd, &ps);
            break;
        }
        
        case WM_KEYDOWN: {
            if(GetAsyncKeyState(0x49) & 0x8000) {
                // Código cuando se presiona 'i' o 'I'
                modo_ia_activado = modo_ia_activado == 0 ? 1 : 0;
                if(modo_ia_activado) {
                    printf("Cambiando control a piloto automatico (IA activada)\n");
                }
                else {
                    printf("Cambiando control a manual (IA desactivada)\n");
                }
            }
            switch(estado_actual) {
                case ESTADO_PIDIENDO_MONEDA: {
                    if (GetAsyncKeyState(TECLA_MONEDA) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000){
                        printf("\nHa insertado una moneda\n");
                        PlaySound(MAKEINTRESOURCE(IDR_SOUND_COIN), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
                        monedas_introducidas = 1;
                        inicializar_menu_nueva_partida();
                        estado_actual = ESTADO_OPCIONES;
                        printf("\nEsta en el menu de opciones:\n\tUse la tecla enter para cambiar el tipo de mision y de terreno\n");
                        printf("\tUse las flechas de arriba y abajo para desplazarse por el menu\n");
                        printf("\tUse el enter para seleccionar/deseleccionar el modo superfacil\n\n");
                        printf("\tPulse espacio para comenzar la partida\n\n");
                        repintar_ventana(hwnd);
                        destruir_menu_insertar_moneda();
                    }
                    break;
                }

                case ESTADO_OPCIONES: {
                    if (GetAsyncKeyState(TECLA_MONEDA) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000){
                        printf("Ha insertado una moneda\n");
                        PlaySound(MAKEINTRESOURCE(IDR_SOUND_COIN), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
                        monedas_introducidas++;
                    }
                    else if(wParam == VK_DOWN || wParam == VK_UP) {
                        procesar_pulsado_flechas(hwnd, uMsg, wParam, lParam);
                    }
                    else if(wParam == VK_RETURN) {
                        gestionar_opcion_seleccionada();
                        Opcion_Menu opcion_elegida = obtener_opcion_seleccionada();
                        if(opcion_elegida == EXIT) {
                            PostQuitMessage(0); // Terminar el proceso
                        }
                        else if(opcion_elegida == CONTROLES) {
                            estado_actual = ESTADO_CONTROLES;
                            inicializar_menu_controles();
                            destruir_menu_opciones();
                        }
                        repintar_ventana(hwnd);
                    }
                    else if (wParam == VK_SPACE) {
                        pulsar_tecla(ESPACIO);
                        if(modo_superfacil) {
                            mision = TRAINING;
                            terreno = FACIL;
                        }
                        else {
                            mision = obtener_tipo_mision();
                            terreno = obtener_tipo_terreno();
                        }
                        if(partida_empezada == 0){
                            iniciar_partida(monedas_introducidas, mision, terreno);
                        }
                        else {
                            reestablecer_mision(mision);
                        }
                        estado_actual = ESTADO_JUEGO;
                        partida_empezada = 1;
                        repintar_ventana(hwnd);
                        destruir_menu_opciones();
                    }
                    break;
                }
                case ESTADO_CONTROLES: {
                    if(esperando_que_pulse_tecla == 1) {
                        esperando_que_pulse_tecla = 0;
                        establecer_tecla_control(wParam);
                        repintar_ventana(hwnd);
                    }
                    else if(wParam == VK_DOWN || wParam == VK_UP) {
                        procesar_pulsado_flechas_controles(hwnd, uMsg, wParam, lParam);
                    }
                    else if(wParam == VK_RETURN) {
                        //gestionar_opcion_seleccionada_controles();
                        Opcion_Menu_Controles opcion_elegida = obtener_opcion_seleccionada_controles();
                        if(opcion_elegida == CONTROL_VOLVER) {
                            printf("Control volver\n");
                            estado_actual = ESTADO_OPCIONES;
                            retornar_menu_opciones();
                            destruir_menu_controles();
                        }
                        else {
                            esperando_que_pulse_tecla = 1;
                        }
                        repintar_ventana(hwnd);
                    }
                    break;
                }

                case ESTADO_JUEGO: {
                    if(GetAsyncKeyState(TECLA_PAUSA) & 0x8000) {
                        estado_actual = ESTADO_OPCIONES;
                        inicializar_menu_nueva_partida();
                        repintar_ventana(hwnd);
                    }
                    if (GetAsyncKeyState(TECLA_PROPULSOR) & 0x8000) { 
                        pulsar_tecla(ARRIBA);
                        PlaySound(MAKEINTRESOURCE(IDR_SOUND_MOTOR), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
                    }
                    if (GetAsyncKeyState(TECLA_ROTAR_IZDA) & 0x8000) pulsar_tecla(IZQUIERDA);
                    if (GetAsyncKeyState(TECLA_ROTAR_DCHA) & 0x8000) pulsar_tecla(DERECHA);
                    if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) pulsar_tecla(ESPACIO);
                    if (GetAsyncKeyState(TECLA_MONEDA) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000) {
                        printf("Ha insertado una monada\n");
                        pulsar_tecla(MONEDA);
                        moneda_presionada = 1;
                        //PlaySound(MAKEINTRESOURCE(IDR_SOUND_COIN), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
                    }
                    break;
                }

                case ESTADO_ATERRIZAJE: {
                    if (GetAsyncKeyState(TECLA_MONEDA) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000) {
                        printf("Ha insertado una monada\n");
                        pulsar_tecla(MONEDA);
                        PlaySound(MAKEINTRESOURCE(IDR_SOUND_COIN), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
                    }
                    if(wParam == VK_SPACE) {
                        // seguir jugando
                        if(combustible < combustible_motor) {
                            generar_mensaje_final_partida(puntuacion_partida);
                            estado_actual = ESTADO_FIN_PARTIDA;
                        }
                        else {
                            continuar_tras_aterrizaje();
                            estado_actual = ESTADO_JUEGO;
                        }
                        repintar_ventana(hwnd);
                    }
                    break;
                }

                case ESTADO_FIN_PARTIDA: {
                    if (GetAsyncKeyState(VK_SPACE) & 0x8000) { 
                        //resetear();
                    }
                    break;
                }

                default: break;
            }
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                esc_presionado = 1;
                SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
            }
        break;
        }

        case WM_KEYUP: {
            switch(estado_actual) {
                case ESTADO_PIDIENDO_MONEDA: {
                    break;
                }

                case ESTADO_OPCIONES: {
                    if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)){
                        
                    }
                    break;
                }

                case ESTADO_JUEGO: {
                    if (!(GetAsyncKeyState(TECLA_PROPULSOR) & 0x8000)) {
                        PlaySound(NULL, NULL, 0);
                        levantar_tecla(ARRIBA);
                    }
                    if (!(GetAsyncKeyState(TECLA_ROTAR_IZDA) & 0x8000)) levantar_tecla(IZQUIERDA);
                    if (!(GetAsyncKeyState(TECLA_ROTAR_DCHA) & 0x8000)) levantar_tecla(DERECHA);
                    if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) levantar_tecla(ESPACIO);
                    if (!(GetAsyncKeyState(TECLA_MONEDA) & 0x8000) && moneda_presionada) {
                        moneda_presionada = 0;
                        PlaySound(MAKEINTRESOURCE(IDR_SOUND_COIN), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
                    }
                    break;
                }

                case ESTADO_ATERRIZAJE: {
                    break;
                }

                case ESTADO_FIN_PARTIDA: {
                    break;
                }

                default: break;
            }
            
            break;
        }

        case WM_DESTROY: {
            KillTimer(hwnd, timer_TICK_juego);
            KillTimer(hwnd, timer_IA);
            KillTimer(hwnd, timer_segundo);
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance; // Evita el warning
    (void)lpCmdLine; // Evita el warning

    //AttachConsoleToStdout();
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "RasterWindow";
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClass(&wc);
    
    RECT rc = {0, 0, tamano_inicial_pantalla_X, tamano_inicial_pantalla_Y};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    HWND hwnd = CreateWindowEx(0, "RasterWindow", "Lunar Lander",
                               WS_OVERLAPPEDWINDOW, 0, 0,
                               (rc.right - rc.left), (rc.bottom - rc.top),
                               NULL, NULL, hInstance, NULL);
    
    inicializar_puntos();
    inicializar_aleatoriedad();
    printf("Presione '5' para insertar moneda y acceder al menu de opciones\n");
    printf("Recuerde: Si pulsa 'i' se activara el piloto automatico\n\n");
    iniciar_nueva_partida(hwnd);

    if (!hwnd) return 0;
    ShowWindow(hwnd, nCmdShow);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}