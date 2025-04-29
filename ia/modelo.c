// modelo.c
#include "modelo.h"
#include "pesos.h"

// Función de capa fully connected (Linear)
void linear(float* salida, const float* entrada, float pesos[][300], float* bias, int out_features, int in_features) {
    for (int i = 0; i < out_features; i++) {
        salida[i] = bias[i];
        for (int j = 0; j < in_features; j++) {
            salida[i] += pesos[i][j] * entrada[j];
        }
    }
}

// Overload de linear para otras dimensiones
void linear2(float* salida, const float* entrada, float pesos[][512], float* bias, int out_features, int in_features) {
    for (int i = 0; i < out_features; i++) {
        salida[i] = bias[i];
        for (int j = 0; j < in_features; j++) {
            salida[i] += pesos[i][j] * entrada[j];
        }
    }
}

void linear3(float* salida, const float* entrada, float pesos[][256], float* bias, int out_features, int in_features) {
    for (int i = 0; i < out_features; i++) {
        salida[i] = bias[i];
        for (int j = 0; j < in_features; j++) {
            salida[i] += pesos[i][j] * entrada[j];
        }
    }
}

// Función ReLU
void relu(float* array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] < 0.0f)
            array[i] = 0.0f;
    }
}

// Función de inferencia completa
void inferencia(float* entrada, float* salida) {
    static float capa1[512];
    static float capa2[256];

    // Capa 1
    linear(capa1, entrada, pesos1, bias1, 512, 300);
    relu(capa1, 512);

    // Capa 2
    linear2(capa2, capa1, pesos2, bias2, 256, 512);
    relu(capa2, 256);

    // Capa 3
    linear3(salida, capa2, pesos3, bias3, 32, 256);
    // No activación final
}
