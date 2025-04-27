#include "fragmentacion_nave.h"

#include "dibujable.h"
#include "../resources/nave.h"
#include "fisicas.h"

struct objetoFisico* of_fragmento_Pata_Izda = {0};
struct objetoFisico* of_fragmento_Pata_Dcha = {0};
struct objetoFisico* of_fragmento_Cupula = {0};
struct objetoFisico* of_fragmento_Base = {0};
struct objetoFisico* of_fragmento_Motor_Izda = {0};
struct objetoFisico* of_fragmento_Motor_Dcha = {0};
struct objetoFisico* of_fragmento_Motor_Base = {0};

// Reserva espacio para los objetos fisicos, los inicializa con su dibujable y 
// masa y establece el resto de atributos a 0
void inicializar_nave_fragmentada(){
	of_fragmento_Pata_Izda = (struct objetoFisico*)malloc(sizeof(struct objetoFisico));
	of_fragmento_Pata_Izda->objeto = crear_dibujable(&Fragmento_Pata_Izda);
	of_fragmento_Pata_Izda->masa = 1000 / 7;
	of_fragmento_Pata_Izda->aceleracion[0] = 0;
	of_fragmento_Pata_Izda->aceleracion[1] = 0;
	of_fragmento_Pata_Izda->velocidad[0] = 0;
	of_fragmento_Pata_Izda->velocidad[1] = 0;
	of_fragmento_Pata_Izda->rotacion = 0;

	of_fragmento_Pata_Dcha = (struct objetoFisico*)malloc(sizeof(struct objetoFisico));
	of_fragmento_Pata_Dcha->objeto = crear_dibujable(&Fragmento_Pata_Dcha);
	of_fragmento_Pata_Dcha->masa = 1000 / 7;
	of_fragmento_Pata_Dcha->aceleracion[0] = 0;
	of_fragmento_Pata_Dcha->aceleracion[1] = 0;
	of_fragmento_Pata_Dcha->velocidad[0] = 0;
	of_fragmento_Pata_Dcha->velocidad[1] = 0;
	of_fragmento_Pata_Dcha->rotacion = 0;

	of_fragmento_Cupula = (struct objetoFisico*)malloc(sizeof(struct objetoFisico));
	of_fragmento_Cupula->objeto = crear_dibujable(&Fragmento_Cupula);
	of_fragmento_Cupula->masa = 1000 / 7;
	of_fragmento_Cupula->aceleracion[0] = 0;
	of_fragmento_Cupula->aceleracion[1] = 0;
	of_fragmento_Cupula->velocidad[0] = 0;
	of_fragmento_Cupula->velocidad[1] = 0;
	of_fragmento_Cupula->rotacion = 0;

	of_fragmento_Base = (struct objetoFisico*)malloc(sizeof(struct objetoFisico));
	of_fragmento_Base->objeto = crear_dibujable(&Fragmento_Base);
	of_fragmento_Base->masa = 1000 / 7;
	of_fragmento_Base->aceleracion[0] = 0;
	of_fragmento_Base->aceleracion[1] = 0;
	of_fragmento_Base->velocidad[0] = 0;
	of_fragmento_Base->velocidad[1] = 0;
	of_fragmento_Base->rotacion = 0;

	of_fragmento_Motor_Izda = (struct objetoFisico*)malloc(sizeof(struct objetoFisico));
	of_fragmento_Motor_Izda->objeto = crear_dibujable(&Fragmento_Motor_Izda);
	of_fragmento_Motor_Izda->masa = 1000 / 7;
	of_fragmento_Motor_Izda->aceleracion[0] = 0;
	of_fragmento_Motor_Izda->aceleracion[1] = 0;
	of_fragmento_Motor_Izda->velocidad[0] = 0;
	of_fragmento_Motor_Izda->velocidad[1] = 0;
	of_fragmento_Motor_Izda->rotacion = 0;

	of_fragmento_Motor_Dcha = (struct objetoFisico*)malloc(sizeof(struct objetoFisico));
	of_fragmento_Motor_Dcha->objeto = crear_dibujable(&Fragmento_Motor_Dcha);
	of_fragmento_Motor_Dcha->masa = 1000 / 7;
	of_fragmento_Motor_Dcha->aceleracion[0] = 0;
	of_fragmento_Motor_Dcha->aceleracion[1] = 0;
	of_fragmento_Motor_Dcha->velocidad[0] = 0;
	of_fragmento_Motor_Dcha->velocidad[1] = 0;
	of_fragmento_Motor_Dcha->rotacion = 0;

	of_fragmento_Motor_Base = (struct objetoFisico*)malloc(sizeof(struct objetoFisico));
	of_fragmento_Motor_Base->objeto = crear_dibujable(&Fragmento_Motor_Base);
	of_fragmento_Motor_Base->masa = 1000 / 7;
	of_fragmento_Motor_Base->aceleracion[0] = 0;
	of_fragmento_Motor_Base->aceleracion[1] = 0;
	of_fragmento_Motor_Base->velocidad[0] = 0;
	of_fragmento_Motor_Base->velocidad[1] = 0;
	of_fragmento_Motor_Base->rotacion = 0;
}


void establecer_velocidades(float vx, float vy) {
	of_fragmento_Pata_Izda->velocidad[0] = vx-2.5;
	of_fragmento_Pata_Dcha->velocidad[0] = vx+3;
	of_fragmento_Cupula->velocidad[0] = vx;
	of_fragmento_Base->velocidad[0] = vx-0.5;
	of_fragmento_Motor_Izda->velocidad[0] = vx-1;
	of_fragmento_Motor_Dcha->velocidad[0] = vx+1;
	of_fragmento_Motor_Base->velocidad[0] = vx-0.5;

	of_fragmento_Pata_Izda->velocidad[1] = vy+3;
	of_fragmento_Pata_Dcha->velocidad[1] = vy-2.5;
	of_fragmento_Cupula->velocidad[1] = vy;
	of_fragmento_Base->velocidad[1] = vy-0.5;
	of_fragmento_Motor_Izda->velocidad[1] = vy+1;
	of_fragmento_Motor_Dcha->velocidad[1] = vy-1;
	of_fragmento_Motor_Base->velocidad[1] = vy+0.5;
}

void establecer_aceleraciones(float ax, float ay) {
	of_fragmento_Pata_Izda->aceleracion[0] = ax;
	of_fragmento_Pata_Dcha->aceleracion[0] = ax;
	of_fragmento_Cupula->aceleracion[0] = ax;
	of_fragmento_Base->aceleracion[0] = ax;
	of_fragmento_Motor_Izda->aceleracion[0] = ax;
	of_fragmento_Motor_Dcha->aceleracion[0] = ax;
	of_fragmento_Motor_Base->aceleracion[0] = ax;

	of_fragmento_Pata_Izda->aceleracion[1] = ay;
	of_fragmento_Pata_Dcha->aceleracion[1] = ay;
	of_fragmento_Cupula->aceleracion[1] = ay;
	of_fragmento_Base->aceleracion[1] = ay;
	of_fragmento_Motor_Izda->aceleracion[1] = ay;
	of_fragmento_Motor_Dcha->aceleracion[1] = ay;
	of_fragmento_Motor_Base->aceleracion[1] = ay;
}

void colocar_fragmentos(struct Punto punto) {
	colocar_dibujable(of_fragmento_Pata_Izda->objeto, punto);
	colocar_dibujable(of_fragmento_Pata_Dcha->objeto, punto);
	colocar_dibujable(of_fragmento_Cupula->objeto, punto);
	colocar_dibujable(of_fragmento_Base->objeto, punto);
	colocar_dibujable(of_fragmento_Motor_Izda->objeto, punto);
	colocar_dibujable(of_fragmento_Motor_Dcha->objeto, punto);
	colocar_dibujable(of_fragmento_Motor_Base->objeto, punto);
}

void establecer_fragmentos_al_colisionar(float v_vertical, float v_horizontal, struct Punto punto_colision) {
	colocar_fragmentos(punto_colision);
    establecer_velocidades(-v_horizontal, -v_vertical);
}

void fisicas_fragmentos(){
	struct Punto traslacion = {0};

	//Actualizar valor de la transicion para el fragmento fragmento_Pata_Izda
	traslacion = (struct Punto) {of_fragmento_Pata_Izda->velocidad[0] * intervalo_fisicas_ms / pixels_por_metro,
								 -(of_fragmento_Pata_Izda->velocidad[1] * intervalo_fisicas_ms / pixels_por_metro)};
	trasladarDibujable(of_fragmento_Pata_Izda->objeto, traslacion);

	//Actualizar valor de la transicion para el fragmento fragmento_Pata_Dcha
	traslacion = (struct Punto) {of_fragmento_Pata_Dcha->velocidad[0] * intervalo_fisicas_ms / pixels_por_metro,
								 -(of_fragmento_Pata_Dcha->velocidad[1] * intervalo_fisicas_ms / pixels_por_metro)};
	trasladarDibujable(of_fragmento_Pata_Dcha->objeto, traslacion);

	//Actualizar valor de la transicion para el fragmento fragmento_Cupula
	traslacion = (struct Punto) {of_fragmento_Cupula->velocidad[0] * intervalo_fisicas_ms / pixels_por_metro,
								 -(of_fragmento_Cupula->velocidad[1] * intervalo_fisicas_ms / pixels_por_metro)};
	trasladarDibujable(of_fragmento_Cupula->objeto, traslacion);

	//Actualizar valor de la transicion para el fragmento fragmento_Base
	traslacion = (struct Punto) {of_fragmento_Base->velocidad[0] * intervalo_fisicas_ms / pixels_por_metro,
								 -(of_fragmento_Base->velocidad[1] * intervalo_fisicas_ms / pixels_por_metro)};
	trasladarDibujable(of_fragmento_Base->objeto, traslacion);

	//Actualizar valor de la transicion para el fragmento fragmento_Motor_Izda
	traslacion = (struct Punto) {of_fragmento_Motor_Izda->velocidad[0] * intervalo_fisicas_ms / pixels_por_metro,
								 -(of_fragmento_Motor_Izda->velocidad[1] * intervalo_fisicas_ms / pixels_por_metro)};
	trasladarDibujable(of_fragmento_Motor_Izda->objeto, traslacion);

	//Actualizar valor de la transicion para el fragmento fragmento_Motor_Dcha
	traslacion = (struct Punto) {of_fragmento_Motor_Dcha->velocidad[0] * intervalo_fisicas_ms / pixels_por_metro,
								 -(of_fragmento_Motor_Dcha->velocidad[1] * intervalo_fisicas_ms / pixels_por_metro)};
	trasladarDibujable(of_fragmento_Motor_Dcha->objeto, traslacion);

	//Actualizar valor de la transicion para el fragmento fragmento_Motor_Base
	traslacion = (struct Punto) {of_fragmento_Motor_Base->velocidad[0] * intervalo_fisicas_ms / pixels_por_metro,
								 -(of_fragmento_Motor_Base->velocidad[1] * intervalo_fisicas_ms / pixels_por_metro)};
	trasladarDibujable(of_fragmento_Motor_Base->objeto, traslacion);	
}


void dibujar_fragmentos(HDC hdc) {
	dibujar_dibujable(hdc, of_fragmento_Pata_Izda->objeto);
	dibujar_dibujable(hdc, of_fragmento_Pata_Dcha->objeto);
	dibujar_dibujable(hdc, of_fragmento_Cupula->objeto);
	dibujar_dibujable(hdc, of_fragmento_Base->objeto);
	dibujar_dibujable(hdc, of_fragmento_Motor_Izda->objeto);
	dibujar_dibujable(hdc, of_fragmento_Motor_Dcha->objeto);
	dibujar_dibujable(hdc, of_fragmento_Motor_Base->objeto);
}