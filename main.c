#include "code/lunar_lander.h"
#include "code/menu.h"
#include "code/gestor_plataformas.h"
#include "code/palabra.h"
#include "resources/superficie_lunar.h"

#include "code/variables_globales.h"
#include "code/constantes.h"

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define timer 1


// Factor por el que escalar la escena
float factor_escalado = 1.0f; 

// 1 si la ventana esta en fullcreen, 0 si no (sin bordes ni cabecera)
uint8_t fullscreen = 0;
uint8_t esc_presionado = 0;
// Rectangulo que contiene la ventana anterior al resize de pantalla completa
RECT rectVentanaAnterior;

// Moneda presionada
uint8_t moneda_presionada = 0;

/* Estado de la aplicación */
typedef enum {
    ESTADO_MENU,
    ESTADO_JUEGO
} EstadoAplicacion;

EstadoAplicacion estadoActual = ESTADO_MENU;


struct Punto* p1 = NULL;
struct Punto* p2 = NULL;
struct Punto* p3 = NULL;
struct Punto* p4 = NULL;

float minimo(float a, float b) {
    return (a < b) ? a : b;
}

void AttachConsoleToStdout() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
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

/**
 * Escala la escena al tamaño de la ventana
 */
void escalar(HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    int ancho_cliente = rect.right - rect.left;
    int alto_cliente = rect.bottom - rect.top;
    tam_ventana_x = (float) ancho_cliente;
    tam_ventana_y = (float) alto_cliente;
    float factor_resized_x = (float)ancho_cliente / tamano_inicial_pantalla_X;
    float factor_resized_y = (float)alto_cliente / tamano_inicial_pantalla_Y;
    
    escalar_escena(1/factor_escalado, 1/factor_escalado);
    factor_escalado = minimo(factor_resized_x, factor_resized_y);
    escalar_escena(factor_escalado, factor_escalado);
    
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


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        SetTimer(hwnd, timer, intervalo_fisicas_ms, NULL);
        inicializarMenu();
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
            escalar(hwnd);
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
        escalar(hwnd);
        break;
    }
    case WM_TIMER: {
        if (wParam == timer) {
            manejar_instante();
            manejar_teclas();
            InvalidateRect(hwnd, NULL, FALSE);
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
        
        if (estadoActual == ESTADO_MENU) {
            // Pasa el hwnd real a la función
            dibujarMenuEnBuffer(hdcMem, hwnd);

        } else if (estadoActual == ESTADO_JUEGO) {
            dibujar_bordes(hdcMem);
            pintar_pantalla(hdcMem);
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
        if(estadoActual == ESTADO_MENU) {
            procesar_pulsado_flechas(hwnd, uMsg, wParam, lParam);
            if(wParam == VK_RETURN) {
                gestionar_opcion_seleccionada();
                OpcionMenu op = obtenerOpcionSeleccionada();
                if(op == EXIT) {
                    printf("Exit seleccionado\n");
                    PostQuitMessage(0); // Terminar el proceso
                }
            }
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            esc_presionado = 1;
            SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
        }
        if (GetAsyncKeyState(VK_UP) & 0x8000) pulsar_tecla(ARRIBA);
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) pulsar_tecla(IZQUIERDA);
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) pulsar_tecla(DERECHA);
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) pulsar_tecla(ESPACIO);
        if (GetAsyncKeyState(0x35) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000) pulsar_tecla(MONEDA);

    break;
    }
    case WM_KEYUP: {
        if (!(GetAsyncKeyState(VK_UP) & 0x8000)) levantar_tecla(ARRIBA);
        if (!(GetAsyncKeyState(VK_LEFT) & 0x8000)) levantar_tecla(IZQUIERDA);
        if (!(GetAsyncKeyState(VK_RIGHT) & 0x8000)) levantar_tecla(DERECHA);
        if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) levantar_tecla(ESPACIO);
        if (!(GetAsyncKeyState(0x35) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000)) levantar_tecla(MONEDA);
        break;
    }
    case WM_DESTROY: {
        KillTimer(hwnd, timer);
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance; // Evita el warning
    (void)lpCmdLine; // Evita el warning

    AttachConsoleToStdout();
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
    
    if (!hwnd) return 0;
    ShowWindow(hwnd, nCmdShow);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}