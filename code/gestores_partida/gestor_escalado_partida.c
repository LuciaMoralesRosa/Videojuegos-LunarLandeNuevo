#include "gestor_escalado_partida.h"
#include "elementos_partida.h"
#include "../transformaciones.h"

void escalar_nave_partida(float factor_x, float factor_y){
	escalar_dibujable_en_escena_dados_ejes(motor_fuerte, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes(motor_medio, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes(motor_debil, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes(nave->objeto, factor_x, factor_y);
}

void escalar_terreno_partida(float factor_x, float factor_y) {
	escalar_dibujable_en_escena_dados_ejes(terreno_0, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes(terreno_1, factor_x, factor_y);
	for(uint8_t i = 0; i < numero_plataformas; i++) {
		escalar_dibujable_en_escena_dados_ejes(plataformas_0[i].linea, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes(plataformas_1[i].linea, factor_x, factor_y);
		for(uint8_t j = 0; j < plataformas_0[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes(plataformas_0[i].palabra->letras[j], factor_x, factor_y);
		}
		for(uint8_t j = 0; j < plataformas_1[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes(plataformas_1[i].palabra->letras[j], factor_x, factor_y);
		}
	}
}

void escalar_terreno_partida_dado_punto(struct Punto punto, float factor_x, float factor_y){
	escalar_dibujable_en_escena_dados_ejes_y_punto(terreno_0, punto, factor_x, factor_y);
	escalar_dibujable_en_escena_dados_ejes_y_punto(terreno_1, punto, factor_x, factor_y);
	for(uint8_t i = 0; i < numero_plataformas; i++) {
		escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_0[i].linea, punto, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_1[i].linea, punto, factor_x, factor_y);
		for(uint8_t j = 0; j < plataformas_0[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_0[i].palabra->letras[j], punto, factor_x, factor_y);
		}
		for(uint8_t j = 0; j < plataformas_1[i].palabra->num_letras; j++){
			escalar_dibujable_en_escena_dados_ejes_y_punto(plataformas_1[i].palabra->letras[j], punto, factor_x, factor_y);
		}
	}
}

void escalar_escena_gestor(float factor_x, float factor_y){
	if(inicio == 1) {
		escalar_nave_partida(factor_x, factor_y);
		escalar_terreno_partida(factor_x, factor_y);
	}
}