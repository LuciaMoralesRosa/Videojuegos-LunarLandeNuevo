// pesos.h
#ifndef PESOS_H
#define PESOS_H

// --- CAPA 1 ---
extern float pesos1[512][300];
extern float bias1[512];

// --- CAPA 2 ---
extern float pesos2[256][512];
extern float bias2[256];

// --- CAPA 3 ---
extern float pesos3[32][256];
extern float bias3[32];

#endif // PESOS_H
