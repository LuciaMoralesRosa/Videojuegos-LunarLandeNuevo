#ifndef NAVE_H
#define NAVE_H

#include "../code/dibujable.h"

#define CENTRO_CONST_NAVE {9, 5}
#define ANCHURA_NAVE 18
#define ALTURA_NAVE 18

// nave.h
extern const struct DibujableConstante Nave_Base;
extern const struct DibujableConstante Nave_Propulsion_Maxima;
extern const struct DibujableConstante Nave_Propulsion_Media;
extern const struct DibujableConstante Nave_Propulsion_Minima;


// nave fragmentada
extern const struct DibujableConstante Fragmento_Pata_Izda;
extern const struct DibujableConstante Fragmento_Pata_Dcha;
extern const struct DibujableConstante Fragmento_Cupula;
extern const struct DibujableConstante Fragmento_Base;
extern const struct DibujableConstante Fragmento_Motor_Izda;
extern const struct DibujableConstante Fragmento_Motor_Dcha;
extern const struct DibujableConstante Fragmento_Motor_Base;



#endif // NAVE_H
