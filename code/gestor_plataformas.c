#include "gestor_plataformas.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../resources/caracteres.h"
#include "transformaciones.h"
#include "palabra.h"

void inicializar_aleatoriedad() {
    srand(time(NULL));
}

/**
 * @brief Dado un terreno y un numero maximo de aristas, calcula todas las 
 * plataformas potenciales (es decir, que la arista sea perfectamente
 * horizontal) y las devuelve
 * 
 * @param terreno Terreno sobre el que se quieren obtener las aristas
 * @param num_aristas Numero de aristas que se han calculado
 * 
 * @return todas las aristas que se corresponden a plataformas potenciales
 */
struct Arista* obtener_aristas_posibles(const struct DibujableConstante* terreno, uint8_t* num_aristas) {
    if (!terreno->puntos) return NULL;

    // Reservar espacio para un número máximo de aristas posibles
    struct Arista* aristas_posibles = malloc((terreno->num_puntos - 1) * sizeof(struct Arista));
    if (!aristas_posibles) return NULL; 

    struct Punto puntoAnterior = terreno->puntos[0];

    *num_aristas = 0;
    for (uint8_t i = 1; i < terreno->num_puntos; i++) {
        if (puntoAnterior.y == terreno->puntos[i].y) {
            // Si la altura de dos puntos consecutivos es igual -> posible plataforma
            if((terreno->puntos[i].x - puntoAnterior.x) <= PLATAFORMA_X2) {
                // Si tiene el tamaño minimo necesario
                aristas_posibles[*num_aristas].origen = &terreno->puntos[i - 1];
                aristas_posibles[*num_aristas].destino = &terreno->puntos[i];
                (*num_aristas)++;
            }
        }
        puntoAnterior = terreno->puntos[i];
    }

    // Redimensionar para liberar memoria no utilizada
    aristas_posibles = realloc(aristas_posibles, (*num_aristas) * sizeof(struct Arista));
    return aristas_posibles;
}


/**
 * @brief Calcula el tipo de bonificador correspondiente a la arista dadas las 
 * constantes definidas para cada bonificacion
 * 
 * @param arista Arista a evaluar para obtener su bonificacion
 * @param b Dibujable de la plataforma correspondiente a la arista a evaluar
 * 
 * @return valor de la bonificacion dada a la plataforma
 */
uint8_t calcular_bonificador(struct Arista arista, const struct DibujableConstante** b){
    uint16_t diferencia = arista.destino->x - arista.origen->x;
    if(diferencia <= PLATAFORMA_X5) {
        *b = &Numero_5_Base;
        return 5;
    }
    else if(diferencia <= PLATAFORMA_X4) {
        *b = &Numero_4_Base;
        return 4;
    }
    else if(diferencia <= PLATAFORMA_X3) {
        *b = &Numero_3_Base;
        return 3;
    }
    else {
        *b = &Numero_2_Base;
        return 2;
    }

}

/**
 * @brief Baraja aleatoriamente los elementos de un arreglo usando el algoritmo
 * de Fisher-Yates.
 *
 * @param arr Puntero al arreglo de números que se desea barajar.
 * @param n Tamaño del arreglo (cantidad de elementos en arr).
 *
 * @note Se asume que la semilla del generador de números aleatorios (rand) 
 *       ha sido inicializada previamente con srand().
 */
void barajar(uint8_t* arr, uint8_t n) {
    for (uint8_t i = n - 1; i > 0; i--) {
        uint8_t j = rand() % (i + 1);
        // Intercambiar arr[i] y arr[j]
        uint8_t temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

/**
 * @brief Genera una lista de números aleatorios únicos en un rango determinado.
 *
 * @param resultado Puntero al array donde se almacenarán los números generados.
 * @param num_valores Cantidad de números aleatorios a generar.
 * @param max_valor Valor máximo del rango (exclusivo).
 *
 * @note La función asume que el tamaño de resultado es al menos num_valores.
 * @note Se asume que la semilla del generador de números aleatorios ha sido 
 *       inicializada previamente con srand().
 * 
 * @warning Si num_valores es mayor que max_valor, la función no generará
 * valores y retornará sin hacer nada.
 */
void generar_aleatorios(uint8_t* resultado, uint8_t num_valores, uint8_t max_valor) {
    if (num_valores > max_valor) return;

    // Inicializar el arreglo con los números del 0 al MAX-1
    uint8_t* numeros = malloc(max_valor * sizeof(uint8_t));
    for (uint8_t i = 0; i < max_valor; i++) {
        numeros[i] = i; // numeros = {0,1,2,3,...,max_Valor}
    }

    // Barajar el arreglo
    barajar(numeros, max_valor);

    // Copiar los primeros X números en el resultado
    for (uint8_t i = 0; i < num_valores; i++) {
        resultado[i] = numeros[i];
    }

    // Liberar memoria
    free(numeros);
}


/**
 * 
 */
struct Palabra* generar_palabra_plataforma(struct Arista arista, uint8_t* bonificador) {
    const struct DibujableConstante* caracter_bonificador;
    *bonificador = calcular_bonificador(arista, &caracter_bonificador);
    
    // Calcular origen de la palabra (palabra de dos letras)
    int16_t diferencia = arista.destino->x - arista.origen->x;
    int16_t media_arista_x = arista.origen->x + diferencia/2;
    int16_t y = arista.destino->y + 12;
    int8_t tam_media_palabra = (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER) / 2;

    struct Palabra* palabra = crear_palabra((struct Punto){media_arista_x - tam_media_palabra, y});
    agregar_letra(palabra, &Letra_X_Base);
    agregar_letra(palabra, caracter_bonificador);
    escalar_palabra_centrada(palabra, 0.6);

    return palabra;
}

/**
 * @brief Genera una línea para una plataforma basándose en una arista y un punto de origen.
 *
 * @param arista La arista que define la línea.
 * @param linea Puntero a una estructura DibujableConstante donde se almacenará la línea generada.
 * @param origen_terreno El punto de origen para la línea sobre el terreno.
 *
 * @return Un puntero a la estructura DibujableConstante generada, o NULL en caso de error.
 *
 * @note Si ocurre un error en la asignación de memoria, la función retorna NULL y libera los recursos ya asignados.
 */
struct DibujableConstante* generar_linea_plataforma(struct Arista arista, struct DibujableConstante* linea, struct Punto origen_terreno) {
    // Guardar espacio para los componentes de la línea (4 puntos, 2 aristas)
    linea->puntos = malloc(4 * sizeof(struct Punto));
    if (!linea->puntos) {
        fprintf(stderr, "Error al asignar memoria a los puntos de la línea.\n");
        free(linea);  // Liberar la memoria de la estructura 'linea' si la asignación falla
        return NULL;
    }

    linea->unionAristas = malloc(2 * sizeof(struct UnionAristas));
    if (!linea->unionAristas) {
        fprintf(stderr, "Error al asignar memoria a las uniones de aristas de la línea.\n");
        free(linea->puntos);  // Liberar los puntos ya asignados
        free(linea);  // Liberar la estructura 'linea'
        return NULL;
    }
    
    // Asignar valores a la línea
    linea->origen.x = origen_terreno.x;
    linea->origen.y = origen_terreno.y;
    linea->puntos[0] = (struct Punto){arista.origen->x, arista.origen->y};
    linea->puntos[1] = (struct Punto){arista.destino->x, arista.destino->y};
    linea->puntos[2] = (struct Punto){arista.origen->x, arista.origen->y + 1};
    linea->puntos[3] = (struct Punto){arista.destino->x, arista.destino->y + 1};
    linea->unionAristas[0] = (struct UnionAristas){0, 1};
    linea->unionAristas[1] = (struct UnionAristas){2, 3};
    linea->num_puntos = 4;
    linea->num_aristas = 2;

    return linea;  // Retornar la línea generada
}

struct Plataforma* generar_plataforma_dada_arista(struct Plataforma* plataforma, struct Plataforma* plataforma_copia, struct Arista arista, struct Punto origen_terreno, struct Punto origen_terreno_2){
    struct DibujableConstante* linea = malloc(sizeof(struct DibujableConstante));
    if (!linea) {
        free(plataforma);
        return NULL;
    }

    struct DibujableConstante* linea_copia = malloc(sizeof(struct DibujableConstante));
    if (!linea_copia) {
        free(plataforma);
        free(plataforma_copia);
        return NULL;
    }
    
    // Llamar a generar_linea_plataforma y verificar si retorna NULL
    linea = generar_linea_plataforma(arista, linea, origen_terreno);
    linea_copia = generar_linea_plataforma(arista, linea, origen_terreno_2);
    
    // Verificar si la función devolvió NULL y, en caso afirmativo, liberar memoria
    if (linea == NULL) {
        free(linea);  // Liberar la memoria asignada para 'linea'
        free(plataforma);
        free(plataforma_copia);
        return NULL;
    }
    if (linea_copia == NULL) {
        free(linea_copia);  // Liberar la memoria asignada para 'linea'
        free(plataforma_copia);
        free(linea); 
        free(plataforma);
        return NULL;
    }

    // Calculo del bonificador y generacion de la palabra
    uint8_t bonificador;
    struct Palabra* palabra = generar_palabra_plataforma(arista, &bonificador);
    struct Palabra* palabra_copia = generar_palabra_plataforma(arista, &bonificador);
    struct Dibujable* linea_dibujable = crear_dibujable(linea);
    struct Dibujable* linea_dibujable_copia = crear_dibujable(linea_copia);
    // Creacion de la estrcutura plataforma
    plataforma->linea = linea_dibujable;
    plataforma->bonificador = bonificador;
    plataforma->palabra = palabra;

    plataforma_copia->linea = linea_dibujable_copia;
    plataforma_copia->bonificador = bonificador;
    plataforma_copia->palabra = palabra_copia;

    return plataforma;
}

/**
 * @brief Genera una plataforma a partir de una arista.
 *
 * Esta función construye una estructura Plataforma usando una arista como base. 
 * Utiliza un DibujableConstante para crear la línea correspondiente, genera la
 * palabra asociada a la plataforma y calcula el bonificador.
 *
 * @param arista Arista desde la cual se genera la plataforma.
 * @param origen_terreno Punto de referencia para colocar la plataforma en el
 *                       terreno.
 * 
 * @return Puntero a la estructura Plataforma generada. Retorna NULL si ocurre
 *         algún error de memoria.
 */
void generar_plataformas(struct Plataforma** plataformas_1, struct Plataforma** plataformas_2, const struct DibujableConstante* terreno, const struct Punto origen_terreno_2, uint8_t* num_plataformas){
    uint8_t num_aristas_posibles = 0;
    // Obtener las aristas correspondientes a plataformas potenciales
    struct Arista* aristas_posibles = obtener_aristas_posibles(terreno, &num_aristas_posibles);
    if (aristas_posibles == NULL) return;
    if(num_aristas_posibles == 0) {
        free(aristas_posibles); 
        return;
    }

    // Nos quedamos con el minimo entre max_posibles y MAX_PLATAFORMAS
    *num_plataformas = (MAX_PLATAFORMAS > num_aristas_posibles) ? num_aristas_posibles : MAX_PLATAFORMAS;
    
    // Obtener los indices aleatorios de las plataformas
    uint8_t* indices_plataformas = malloc(num_aristas_posibles * sizeof(uint8_t));
    if (indices_plataformas == NULL) {
        free(aristas_posibles);  // Liberamos las aristas en caso de error
        return;
    }
    generar_aleatorios(indices_plataformas, *num_plataformas, num_aristas_posibles);
    
	*plataformas_1 = malloc(*num_plataformas * sizeof(struct Plataforma));
	*plataformas_2 = malloc(*num_plataformas * sizeof(struct Plataforma));
    
    // Meter en plataformas las plataformas seleccionadas por los indices
    for(uint8_t i = 0; i < *num_plataformas; i++){
        struct Arista arista = aristas_posibles[indices_plataformas[i]];
        struct Plataforma* plataforma = malloc(sizeof(struct Plataforma));
        if (!plataforma) {
            return;
        }
    
        struct Plataforma* plataforma_copia = malloc(sizeof(struct Plataforma));
        if (!plataforma_copia) {
            return;
        }

        generar_plataforma_dada_arista(plataforma, plataforma_copia, arista, terreno->origen, origen_terreno_2);
        if(plataforma == NULL) {
            free(plataformas_1);
            free(plataformas_2);
            free(indices_plataformas);
            free(aristas_posibles);
        }
        (*plataformas_1)[i] = *plataforma;
        (*plataformas_2)[i] = *plataforma_copia;
    }
    
    // Liberar memoria
    free(indices_plataformas);  
    free(aristas_posibles);
}


void dibujar_plataforma(HDC hdc, struct Plataforma plataforma){
    dibujar_palabra(plataforma.palabra, hdc);
    dibujar_dibujable(hdc, plataforma.linea);
}