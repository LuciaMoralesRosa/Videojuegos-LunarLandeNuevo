#ifndef TABLAS_TRIGONOMETRICAS_H
#define TABLAS_TRIGONOMETRICAS_H


// Valores para las distintas misiones
#define GRAVEDAD_TRAINING -0.00020625
#define GRAVEDAD_CADET -0.00040625
#define GRAVEDAD_PRIME  -0.0007625
#define GRAVEDAD_COMMAND -0.0010625

#define FRICCION_ATMOSFERA 0.001

// Angulo de la rotacion de la nave por cada pulsacion de las teclas
#define ANGULO_ROTACION 10 

// Declaración de las tablas de seno y coseno para las rotaciones de angulos
extern const float SIN_TABLA[360];
extern const float COS_TABLA[360];

extern const float valor_inicial_nave_x;

#define tamano_inicial_pantalla_X 1024
#define tamano_inicial_pantalla_Y 768
#define anchura_minima_ventana 512
#define altura_minima_ventana 384
#define combustible_motor 1

#endif // TABLAS_TRIGONOMETRICAS_H
