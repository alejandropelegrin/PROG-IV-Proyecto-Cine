#ifndef SALA_H
#define SALA_H
#include "sqlite3.h"

typedef struct {
    int id;
    int numero_asientos;
} Sala;

void crearSala(sqlite3 *db, int id, int numero_asientos);
void imprimirSala(sqlite3 *db, int id);
void anyadirSala(sqlite3 *db, Sala nueva);
void eliminarSala(sqlite3 *db, int id);
void modificarSala(sqlite3 *db, int id, int numero_asientos);
void listarSalas(sqlite3 *db);

#endif
