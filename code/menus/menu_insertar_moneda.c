#include "menu_insertar_moneda.h"
#include "../palabra.h"
#include "../../data/constantes.h"
#include "../transformaciones.h"


struct Palabra* insertar_moneda = NULL;
void crear_palabra_insertar_moneda(void){
    insertar_moneda = crearPalabraDesdeCadena("INSERTE UNA MONEDA PARA CONTINUAR",
        (struct Punto){280, tamano_inicial_pantalla_Y-300});
}

void mostrar_insertar_moneda(HDC hdc){
    dibujar_palabra(insertar_moneda, hdc);
}

void escalar_menu_insertar_moneda(float factor_escalado) {
    escalar_palabra_centrada(insertar_moneda, factor_escalado);
}

void destruir_menu_insertar_moneda(void) {
    destruir_palabra(insertar_moneda);
}