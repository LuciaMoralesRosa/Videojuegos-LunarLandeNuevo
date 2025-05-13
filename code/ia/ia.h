#ifndef IA_H
#define IA_H

#include "../dibujable.h"

void inicializar_ia();

void manejar_instante_ia(struct Punto v0, struct Punto p0);

void recompensar(int recompensa_general);

#endif // IA_H