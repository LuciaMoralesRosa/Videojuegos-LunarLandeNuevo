#include "menu_insertar_moneda.h"
#include "../palabra.h"
#include "../../data/constantes.h"
#include "../../data/variables_globales.h"

#include "../transformaciones.h"


struct Palabra* insertar_moneda = NULL;
void crear_palabra_insertar_moneda(void){
    insertar_moneda = crear_palabra_desde_cadena("INSERTE UNA MONEDA PARA CONTINUAR",
        (struct Punto){280, tamano_inicial_pantalla_Y-300});
    escalar_menu_insertar_moneda(factor_escalado);
}

void mostrar_insertar_moneda(HDC hdc){
    dibujar_palabra(insertar_moneda, hdc);
}

void escalar_menu_insertar_moneda(float factor) {
    escalar_palabra_en_escena_dados_ejes(insertar_moneda, factor, factor);
}

void destruir_menu_insertar_moneda(void) {
    destruir_palabra(insertar_moneda);
}