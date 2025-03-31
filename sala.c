#include "sala.h"
#include <stdio.h>
#include <string.h>

void crearSala(Sala *s, int id, int numero_asientos) {
    s->id = id;
    s->numero_asientos = numero_asientos;
}

void imprimirSala(const Sala *s) {
    printf("Sala ID: %d\n", s->id);
    printf("Número de asientos: %d\n", s->numero_asientos);
}

void anyadirSala(Sala salas[], int *numSalas, Sala nueva) {
    salas[*numSalas] = nueva;
    (*numSalas)++;
}

void eliminarSala(Sala salas[], int *numSalas, int id) {
    for (int i = 0; i < *numSalas; i++) {
        if (salas[i].id == id) {
            for (int j = i; j < *numSalas - 1; j++) {
                salas[j] = salas[j + 1];
            }
            (*numSalas)--;
            return;
        }
    }
    printf("Sala con ID %d no encontrada.\n", id);
}

void modificarSala(Sala salas[], int numSalas, int id, int numero_asientos) {
    for (int i = 0; i < numSalas; i++) {
        if (salas[i].id == id) {
            salas[i].numero_asientos = numero_asientos;
            return;
        }
    }
    printf("Sala con ID %d no encontrada.\n", id);
}
