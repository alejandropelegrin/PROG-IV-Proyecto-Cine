#include "asiento.h"
#include <stdio.h>

void crearAsiento(Asiento *a, int id, int sala_id, int numero, int estado) {
    a->id = id;
    a->sala_id = sala_id;
    a->numero = numero;
    a->estado = estado;
}

void imprimirAsiento(const Asiento *a) {
    printf("Asiento ID: %d\n", a->id);
    printf("Sala ID: %d\n", a->sala_id);
    printf("Numero: %d\n", a->numero);
    printf("Estado: %s\n", a->estado == 0 ? "Libre" : "Ocupado");
}

void anyadirAsiento(Asiento asientos[], int *numAsientos, Asiento nuevo) {
    asientos[*numAsientos] = nuevo;
    (*numAsientos)++;
}

void eliminarAsiento(Asiento asientos[], int *numAsientos, int id) {
    for (int i = 0; i < *numAsientos; i++) {
        if (asientos[i].id == id) {
            for (int j = i; j < *numAsientos - 1; j++) {
                asientos[j] = asientos[j + 1];
            }
            (*numAsientos)--;
            return;
        }
    }
}

void modificarAsiento(Asiento asientos[], int numAsientos, int id, int estado) {
    for (int i = 0; i < numAsientos; i++) {
        if (asientos[i].id == id) {
            asientos[i].estado = estado;
            return;
        }
    }
}
