#include "ia.h"
#include "../fisicas.h"
#include "../lunar_lander.h"
#include "../../data/constantes.h"
#include "../../data/variables_juego.h"

#include <stdio.h>
#include <time.h>
#include <math.h>

float A = 0.f;
float B = 0.f;
float C_pos = 0.f;
float C_neg = 0.f;
float D_pos = 0.f;
float D_neg = 0.f;
float E = 50.f;

float delta_A = 0;
float delta_B = 0;
float delta_C = 0;
float delta_D = 0;
float delta_E = 0;

int vx_positiva = 1;
int vy_positiva = 0;

int timer_recalcular = 0;
int timer_aceleracion = 0;

float angulo_grados = 0;
struct Punto aceleracion = {0, 0};
struct Punto objetivo = {0,0};

int plataforma_objetivo = 0;

float random_offset(float max_offset) {
    return ((float)rand() / RAND_MAX * 2 - 1) * max_offset;
}

void obtener_posicion_objetivo() {
    struct Punto inicio = plataformas_0[plataforma_objetivo].linea->puntos[0];
    struct Punto final = plataformas_0[plataforma_objetivo].linea->puntos[1];
    printf("Plataforma con bonificador %d: (%f, %f)\n", plataformas_0[plataforma_objetivo].bonificador, plataformas_0[plataforma_objetivo].linea->origen.x, plataformas_0[plataforma_objetivo].linea->origen.y);
    objetivo = (struct Punto) {inicio.x + ((final.x - inicio.x) / 2), tam_ventana_y-inicio.y};
}

// Asumiendo gravedad vertical (0, g)
struct Punto calcular_aceleracion(struct Punto v0, struct Punto p0) {
    printf("En calcular_aceleracion\n");

    obtener_posicion_objetivo();

    // Tiempo aproximado con simplificación
    delta_A = A + random_offset(0.1f);
    delta_B = B + random_offset(0.1f);
    float g = -gravedad_m_ms / pixels_por_metro;
    if(g == 0) g += -0.000000000001;

    //float t = (2 * v0.y * objetivo.y + g * p0.y + delta_A) / g;
    //t += delta_B;
    float t;
    float discriminante = pow(v0.y, 2) + 2 * g * p0.y;
    if (discriminante >= 0) {
        t = (v0.y + sqrt(discriminante)) / g;
    } else {
        t = -1.0f;
    }
    t += delta_B;

    // Aceleraciones "ideales"
    float ax = 2.0f * (objetivo.x - p0.x - v0.x * t) / (t * t);
    float ay = 2.0f * (objetivo.y - p0.y - v0.y * t) / (t * t);

    // Aplicar offset por peso según signo de velocidad
    vx_positiva = (v0.x >= 0) ? 1 : 0;
    vy_positiva = (v0.y >= 0) ? 1 : 0;
    delta_C = (vx_positiva == 1) ? (C_pos + random_offset(0.000005f)) : (C_neg + random_offset(0.000005f));
    delta_D = (vy_positiva == 1) ? (D_pos + random_offset(0.000005f)) : (D_neg + random_offset(0.000005f));
    ax += delta_C;
    ay += delta_D;

    // Offset para la siguiente llamada
    delta_E = E + random_offset(0.1f);
    timer_recalcular = delta_E;

    // Calculos para el input
    if(ay < 0) ay = 0;
    angulo_grados = atan2(ax, ay) * (180.0 / 3.141592);
    
    float mod_ax = ax < 0 ? -ax : ax;
    float angulo_grados_pos = angulo_grados < 0 ? angulo_grados + 360 : angulo_grados;
    float sen_abs = SIN_TABLA[(int) angulo_grados_pos] < 0 ? -SIN_TABLA[(int) angulo_grados_pos]: SIN_TABLA[(int) angulo_grados_pos];
    sen_abs = sen_abs == 0 ? 0.00001 : sen_abs;
    timer_aceleracion = (mod_ax * pixels_por_metro) / (propulsor_m_ms * sen_abs);
    
    printf("Angulo grados %f, angulo_grados_pos = %f\n", angulo_grados, angulo_grados_pos);
    printf("ax = %f, mod_ax = %f\n", ax, mod_ax);
    printf("sen_abs = %f\n", sen_abs);
    printf("Timer aceleracion %d\n", timer_aceleracion);
    printf("Aceleración necesaria: %f, %f\n\n", ax, ay);
    
    struct Punto a = { ax, ay };
    return a;
}

void recompensar(int recompensa_general, int recompensa_C, int recompensa_D) {
    float alpha = 0.01f;

    A += alpha * recompensa_general * delta_A;
    B += alpha * recompensa_general * delta_B;

    if (vx_positiva == 1) C_pos += alpha * recompensa_C * delta_C;
    else C_neg += alpha * recompensa_C * delta_C;

    if (vy_positiva == 1) D_pos += alpha * recompensa_D * delta_D;
    else D_neg += alpha * recompensa_D * delta_D;
}

void guardar_pesos() {
    FILE * f = fopen("pesos.txt", "w");
    if (f) {
        fprintf(f, "%f %f %f %f %f %f %f\n",
            A, B,
            C_pos, C_neg,
            D_pos, D_neg,
            E);
        fclose(f);
    }
}

void cargar_pesos() {
    FILE * f = fopen("pesos.txt", "r");
    if (f) {
        fscanf(f, "%f %f %f %f %f %f %f",
            &A, &B,
            &C_pos, &C_neg,
            &D_pos, &D_neg,
            &E);
        fclose(f);
    }
}


void inicializar_ia(){
    srand(time(NULL));
    cargar_pesos();
    timer_recalcular = E;
    plataforma_objetivo = rand() % numero_plataformas;
}

struct Input {
    int propulsor;
    int derecha;
    int izquierda;
};

struct Input calcular_input(){
    timer_aceleracion--;
    int rotacion = nave->rotacion;
    if(rotacion > 90) rotacion -= 360;

    //printf("Timer aceleracion: %d\n", timer_aceleracion);
    //printf("Rotacion Nave: %d\n", rotacion);

    if(rotacion < angulo_grados - 5){ // Derecha
        struct Input resultado = {timer_aceleracion, 1, 0};
        //printf("Input Direccion: Derecha\n");
        return resultado;
    }
    if(rotacion > angulo_grados + 5){ // Izquierda
        struct Input resultado = {timer_aceleracion, 0, 1};
        //printf("Input Direccion: Izquierda\n");
        return resultado;
    }
    // No moverse
    struct Input resultado = {timer_aceleracion, 0, 0};
    return resultado;
}

void manejar_instante_ia(struct Punto v0, struct Punto p0){
    timer_recalcular--;
    timer_aceleracion--;

    struct Punto posicion_coord_clasicas = (struct Punto){p0.x, tam_ventana_y-p0.y};

    //printf("------------------------------------\n");
    //printf("Velocidad Nave: %f, %f\n", v0.x, v0.y);
    //printf("Posición Nave: %f, %f\n", posicion_coord_clasicas.x, posicion_coord_clasicas.y);
    //printf("Objetivo: %f, %f\n", objetivo.x, objetivo.y);

    if(timer_recalcular <= 0){
        aceleracion = calcular_aceleracion(v0, posicion_coord_clasicas);
    }

    //printf("Aceleración necesaria: %f, %f\n", aceleracion.x, aceleracion.y);
    //printf("Angulo objetivo: %f\n", angulo_grados);

    struct Input input = calcular_input();
    if(input.propulsor > 0) {
        pulsar_tecla(ARRIBA);
    }
    if(input.izquierda > 0) {
        pulsar_tecla(IZQUIERDA);
        //printf("Izquierda Pulsada\n");
    }
    if(input.derecha > 0) {
        pulsar_tecla(DERECHA);
        //printf("Derecha Pulsada\n");
    }
    manejar_teclas();
}
