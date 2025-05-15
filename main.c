#include "code/lunar_lander.h"
#include "code/gestor_plataformas.h"
#include "code/palabra.h"
#include "code/fragmentacion_nave.h"

#include "code/menus/cabecera_juego.h"
#include "code/menus/menu_aterrizaje.h"
#include "code/menus/menu_final_partida.h"

#include "code/ia/ia.h"

#include "resources/superficie_lunar.h"

#include "data/variables_globales.h"
#include "data/constantes.h"
#include "data/variables_juego.h"

#include <windows.h>

#define timer_TICK_juego 1
#define timer_IA 2
#define timer_segundo 3
#define timer_mostrar_mensajes 4
#define timer_IA_2 5
#define timer_fin_partida 6


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

int timer_ia_fin_partida_activado = 0;
int timer_fin_partida_activado = 0;

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


void iniciar_nueva_partida(HWND hwnd) {
    monedas_introducidas = 0;
    estado_actual = ESTADO_PIDIENDO_MONEDA;
    iniciar_partida(monedas_introducidas);
    repintar_ventana(hwnd);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            SetTimer(hwnd, timer_TICK_juego, intervalo_fisicas_ms, NULL);
            SetTimer(hwnd, timer_IA, 8000, NULL);
            SetTimer(hwnd, timer_segundo, 1000, NULL);
            break;
        }
        
        case WM_TIMER: {
            switch(estado_actual) {
                case ESTADO_PIDIENDO_MONEDA: {
                    if(wParam == timer_IA) {
                        // Gestionar partida ia
                        iniciar_partida(1);
                        inicializar_ia();
                        modo_ia_activado = 1;
                        estado_actual = ESTADO_JUEGO;
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
                    if(modo_ia_activado){
                        if(timer_ia_fin_partida_activado == 0){
                            SetTimer(hwnd, timer_IA_2, 5000, NULL);
                            timer_ia_fin_partida_activado = 1;
                        }
                        if (wParam == timer_IA_2){
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
                            timer_ia_fin_partida_activado = 0;
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

            switch(estado_actual) {
                case ESTADO_PIDIENDO_MONEDA: {
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
                    inicializar_ia();
                    printf("Cambiando control a piloto automatico (IA activada)\n");
                }
                else {
                    printf("Cambiando control a manual (IA desactivada)\n");
                }
            }
            switch(estado_actual) {
                case ESTADO_PIDIENDO_MONEDA: {
                    if (GetAsyncKeyState(TECLA_MONEDA) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000){                        
                        pulsar_tecla(ESPACIO);
                        iniciar_partida(1);
                        estado_actual = ESTADO_JUEGO;
                    }
                    break;
                }

                case ESTADO_JUEGO: {
                    if (GetAsyncKeyState(TECLA_PROPULSOR) & 0x8000) { 
                        if(combustible >= combustible_motor){
                            pulsar_tecla(ARRIBA);
                        }
                    }
                    if (GetAsyncKeyState(TECLA_ROTAR_IZDA) & 0x8000) pulsar_tecla(IZQUIERDA);
                    if (GetAsyncKeyState(TECLA_ROTAR_DCHA) & 0x8000) pulsar_tecla(DERECHA);
                    if (GetAsyncKeyState(TECLA_MONEDA) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000) {
                        pulsar_tecla(MONEDA);
                        moneda_presionada = 1;
                    }
                    break;
                }

                case ESTADO_ATERRIZAJE: {
                    if (GetAsyncKeyState(TECLA_MONEDA) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000) {
                        pulsar_tecla(MONEDA);
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
                case ESTADO_JUEGO: {
                    if (!(GetAsyncKeyState(TECLA_PROPULSOR) & 0x8000)) {
                        levantar_tecla(ARRIBA);
                    }
                    if (!(GetAsyncKeyState(TECLA_ROTAR_IZDA) & 0x8000)) levantar_tecla(IZQUIERDA);
                    if (!(GetAsyncKeyState(TECLA_ROTAR_DCHA) & 0x8000)) levantar_tecla(DERECHA);
                    if (!(GetAsyncKeyState(TECLA_MONEDA) & 0x8000) && moneda_presionada) {
                        moneda_presionada = 0;
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
            KillTimer(hwnd, timer_IA_2);
            KillTimer(hwnd, timer_fin_partida);
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
    
    inicializar_aleatoriedad();
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