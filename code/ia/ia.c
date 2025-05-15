#include "ia.h"
#include "../fisicas.h"
#include "../lunar_lander.h"
#include "../../data/constantes.h"
#include "../../data/variables_juego.h"

#include "../menus/menu_final_partida.h"

#include <stdio.h>
#include <time.h>
#include <math.h>

float B = 1.f;
float C_pos = 0.f;
float C_neg = 0.f;
float D_pos = 0.f;
float D_neg = 0.f;
float E = 50.f;

float delta_B = 0;
float delta_C_pos = 0;
float delta_C_neg = 0;
float delta_D_pos = 0;
float delta_D_neg = 0;
float delta_E = 0;

int vx_positiva = 1;
int vy_positiva = 0;

int timer_recalcular = 0;
int timer_aceleracion = 0;

float angulo_grados = 0;
struct Punto aceleracion = {0, 0};
struct Punto objetivo = {0,0};

int plataforma_objetivo = 0;
int primera_pulsacion_propulsor = 1;

float random_offset(float max_offset) {
    return ((float)rand() / RAND_MAX * 2 - 1) * max_offset;
}

void obtener_posicion_objetivo() {
    struct Punto inicio = plataformas_0[plataforma_objetivo].linea->puntos[0];
    struct Punto final = plataformas_0[plataforma_objetivo].linea->puntos[1];
    printf("Plataforma con bonificador %d: (%f, %f)\n", plataformas_0[plataforma_objetivo].bonificador, plataformas_0[plataforma_objetivo].linea->origen.x, plataformas_0[plataforma_objetivo].linea->origen.y);
    objetivo = (struct Punto) {inicio.x + ((final.x - inicio.x) / 2), tam_ventana_y-inicio.y};
}

void aleatorizar_pesos(){
    delta_B = B; //+ random_offset(0.1f);
    delta_C_pos = C_pos; //+ random_offset(0.000005f); 
    delta_D_pos = D_pos; //+ random_offset(0.000005f); 
    delta_C_neg = C_neg; //+ random_offset(0.000005f);
    delta_D_neg = D_neg; //+ random_offset(0.000005f);
    delta_E = E; //+ random_offset(0.1f);
}

// Asumiendo gravedad vertical (0, g)
struct Punto calcular_aceleracion(struct Punto v0, struct Punto p0) {
    obtener_posicion_objetivo();

    if (p0.y < objetivo.y){
        angulo_grados = 0;
        struct Punto a = { 1, 0 };
        timer_aceleracion = 3000;
        return a;
    }

    // Tiempo aproximado con simplificación
    float g = -gravedad_m_ms / pixels_por_metro;
    if(g == 0) g += -0.000000000001;

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
    ax = (vx_positiva == 1) ? (delta_C_pos + ax) : (delta_C_neg + ax);
    ay += (vy_positiva == 1) ? (delta_D_pos + ay) : (delta_D_neg + ay);

    // Offset para la siguiente llamada
    timer_recalcular = delta_E;

    // Calculos para el input
    if(ay < 0) ay = 0;
    angulo_grados = atan2(ax, ay) * (180.0 / 3.141592);
    
    float mod_ax = ax < 0 ? -ax : ax;
    float angulo_grados_pos = angulo_grados < 0 ? angulo_grados + 360 : angulo_grados;
    float sen_abs = SIN_TABLA[(int) angulo_grados_pos] < 0 ? -SIN_TABLA[(int) angulo_grados_pos]: SIN_TABLA[(int) angulo_grados_pos];
    sen_abs = sen_abs == 0 ? 0.00001 : sen_abs;
    timer_aceleracion = 0.9 * (mod_ax * pixels_por_metro) / (propulsor_m_ms * sen_abs);
    
    struct Punto a = { ax, ay };
    return a;
}

void guardar_pesos() {
    FILE * f = fopen("pesos.txt", "w");
    if (f) {
        fprintf(f, "%f %f %f %f %f %f\n",
            B,
            C_pos, C_neg,
            D_pos, D_neg,
            E);
        fclose(f);
    }
}

void cargar_pesos() {
    FILE * f = fopen("pesos.txt", "r");
    if (f) {
        fscanf(f, "%f %f %f %f %f %f",
            &B,
            &C_pos, &C_neg,
            &D_pos, &D_neg,
            &E);
        fclose(f);
    }
}

void recompensar(int recompensa_general) {
    float alpha = 0.01f;

    //B += alpha * recompensa_general * delta_B;
    //C_pos += alpha * recompensa_general * delta_C_pos;
    //C_neg += alpha * recompensa_general * delta_C_neg;
    //D_pos += alpha * recompensa_general * delta_D_pos;
    //D_neg += alpha * recompensa_general * delta_D_neg;

    //guardar_pesos();
    aleatorizar_pesos();
}

void inicializar_ia(){
    srand(time(NULL));
    //cargar_pesos();
    timer_recalcular = E;
    plataforma_objetivo = rand() % numero_plataformas;
    aleatorizar_pesos();
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
        if(combustible >= combustible_motor){
            pulsar_tecla(ARRIBA);
            if(primera_pulsacion_propulsor == 1) {
                primera_pulsacion_propulsor = 0;
            }
        }
    }
    else {
        primera_pulsacion_propulsor = 1;
    }
    if(input.izquierda > 0) {
        pulsar_tecla(IZQUIERDA);
    }
    if(input.derecha > 0) {
        pulsar_tecla(DERECHA);
    }
    manejar_teclas();
}
