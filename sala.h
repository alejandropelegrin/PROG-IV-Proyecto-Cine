#ifndef SALA_H
#define SALA_H

typedef struct {
    int id;
    int numero_asientos;
} Sala;

void crearSala(Sala *s, int id, int numero_asientos);
void imprimirSala(const Sala *s);
void anyadirSala(Sala salas[], int *numSalas, Sala nueva);
void eliminarSala(Sala salas[], int *numSalas, int id);
void modificarSala(Sala salas[], int numSalas, int id, int numero_asientos);

#endif
