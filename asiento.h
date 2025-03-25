#ifndef ASIENTO_H
#define ASIENTO_H

typedef struct {
    int id;
    int sala_id;
    int numero;
    int estado; // 0: Libre, 1: Ocupado
} Asiento;

void crearAsiento(Asiento *a, int id, int sala_id, int numero, int estado);
void imprimirAsiento(const Asiento *a);
void anyadirAsiento(Asiento asientos[], int *numAsientos, Asiento nuevo);
void eliminarAsiento(Asiento asientos[], int *numAsientos, int id);
void modificarAsiento(Asiento asientos[], int numAsientos, int id, int estado);

#endif
