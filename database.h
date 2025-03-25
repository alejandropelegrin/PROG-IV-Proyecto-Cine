#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"
#include "usuario.h"
#include "pelicula.h"
#include "sala.h"
#include "asiento.h"
#include "sesion.h"
#define NOMBRE_BBDD "Cine.db"

int inicializarBBDD(sqlite3 **db);
void crearTablas(sqlite3 *db);
void volcarFicheroALaBBDD(char *nomfich, sqlite3 *db);
void volcarBBDDAlFichero(char *nomfich, sqlite3 *db);

#endif
