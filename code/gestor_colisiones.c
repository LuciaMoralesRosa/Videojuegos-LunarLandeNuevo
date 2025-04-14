#include "gestor_colisiones.h"
#include <stdio.h>


/**
 * @brief Funcion que comprueba si un punto esta en un segmento
 * 
 * @param p Punto 1 del segmento
 * @param q Punto a comprobar
 * @param r Punto 2 del segmento
 * 
 * @return 1 si esta en el segmento, 0 si no lo esta
 */
uint8_t punto_en_segmento(struct Punto p, struct Punto q, struct Punto r) { 
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && 
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) 
       return 1; 
  
    return 0; 
} 
  
/**
 * @brief Funcion que devuelve la orientacion de tres puntos
 * 
 * @param p Punto 1
 * @param q Punto 2
 * @param r Punto 3
 * 
 * @return 0 si son colineales,
 *         1 si es en sentido de las agujas del reloj,
 *         2 si es sentido contrario
 */
int orientacion(struct Punto p, struct Punto q, struct Punto r) { 
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y); 
    if (val == 0) return 0;  // Colineal
    return (val > 0)? 1: 2; // Sentido agujas del reloj o contrario
} 
  
/**
 * @brief Funcion que comprueba si dos segmentos de recta intersectan
 * 
 * @param p1 Punto 1 del segmento 1
 * @param p2 Punto 2 del segmento 1
 * @param p1 Punto 1 del segmento 2
 * @param p2 Punto 2 del segmento 2
 * 
 * @return 1 si hay interseccion, 0 si no la hay
 */
uint8_t hay_interseccion(struct Punto p1, struct Punto q1, struct Punto p2, struct Punto q2) { 
    // Encontrar orientaciones de los segmentos
    int o1 = orientacion(p1, q1, p2); 
    int o2 = orientacion(p1, q1, q2); 
    int o3 = orientacion(p2, q2, p1); 
    int o4 = orientacion(p2, q2, q1); 
  
    // Caso general -> distintas orientaciones
    if (o1 != o2 && o3 != o4) 
        return 1; 
  
    // Caso especial -> los puntos son colineales
    // p1, q1 y p2 son colineales y p2 esta en el segmento p1q1 
    if (o1 == 0 && punto_en_segmento(p1, p2, q1)) return 1; 
  
    // p1, q1 y q2 son colineales y  q2 esta en el segmento p1q1 
    if (o2 == 0 && punto_en_segmento(p1, q2, q1)) return 1; 
  
    // p2, q2 and p1 son colineales y p1 esta en el segmento p2q2 
    if (o3 == 0 && punto_en_segmento(p2, p1, q2)) return 1; 
  
     // p2, q2 and q1 son colineales y q1 esta en el segmento p2q2 
    if (o4 == 0 && punto_en_segmento(p2, q1, q2)) return 1; 
  
    return 0; // No hay interseccion
} 

uint8_t hay_colision(struct Dibujable* objeto1, struct Dibujable* objeto2, struct Arista* arista_colision) {
    if(!objeto1->puntos || !objeto2->puntos) return 0;
    if(!objeto1->aristas || !objeto2->aristas) return 0;
    
    for(uint16_t i = 0; i < objeto2->num_aristas; i++){
        struct Punto p1 = *objeto2->aristas[i].origen;
        struct Punto p2 = *objeto2->aristas[i].destino;
        *arista_colision = objeto2->aristas[i];
        for(uint16_t j = 0; j < objeto1->num_aristas; j++){
            struct Punto p3 = *objeto1->aristas[j].origen;
            struct Punto p4 = *objeto1->aristas[j].destino;
            if(hay_interseccion(p1, p2, p3, p4)){
                return 1;
            }
        }
    }
    return 0; // No hay colision
}
