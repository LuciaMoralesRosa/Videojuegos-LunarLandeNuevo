#include "menu_insertar_moneda.h"
#include "../palabra.h"
#include "../../data/constantes.h"
#include "../../data/variables_globales.h"

#include "../transformaciones.h"


struct Palabra* insertar_moneda = NULL;
struct Palabra* pulse_5 = NULL;

void crear_palabra_insertar_moneda(void){
    insertar_moneda = crear_palabra_desde_cadena("INSERTE UNA MONEDA PARA CONTINUAR",
        (struct Punto){280, tamano_inicial_pantalla_Y-300});
        pulse_5 = crear_palabra_desde_cadena("PULSE LA TECLA 5 PARA INSERTAR MONEDA",
            (struct Punto){270, tamano_inicial_pantalla_Y-250});
    escalar_menu_insertar_moneda(factor_escalado);
}

void mostrar_insertar_moneda(HDC hdc){
    dibujar_palabra(insertar_moneda, hdc);
    dibujar_palabra(pulse_5, hdc);
}

void escalar_menu_insertar_moneda(float factor) {
    escalar_palabra_en_escena_dados_ejes(insertar_moneda, factor, factor);
    escalar_palabra_en_escena_dados_ejes(pulse_5, factor, factor);
}

void destruir_menu_insertar_moneda(void) {
    destruir_palabra(insertar_moneda);
    destruir_palabra(pulse_5);
}