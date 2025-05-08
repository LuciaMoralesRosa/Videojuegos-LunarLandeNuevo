#include "variables_globales.h"
#include <windows.h>

float tam_ventana_x = 1024;
float tam_ventana_y = 768;
float factor_escalado = 1.0f; 

float gravedad_m_ms = 0;
int friccion_atmosfera_activada = 0;

int modo_ia_activado = 0;

Tipo_Terreno tipo_terreno_seleccionado = ORIGINAL;
Tipo_Mision tipo_mision_seleccionado = TRAINING;
Opcion_Menu opcion_menu_seleccionado = MISSION;

int TECLA_MONEDA = 0x35;
int TECLA_PROPULSOR = 0x26;
int TECLA_ROTAR_IZDA =  0x25;
int TECLA_ROTAR_DCHA =  0x27;
int TECLA_PAUSA =  0x20;

uint16_t segundos_transcurridos = 0;