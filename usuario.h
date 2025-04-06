#ifndef USUARIO_H
#define USUARIO_H

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

typedef struct {
    int id;
    char *nombre;
    char *correo;
    char *contrasenya;
    char *telefono;
} Usuario;

void crearUsuario(Usuario *u, int id, const char *nombre, const char *correo, const char *contrasenya, const char *telefono);
void imprimirUsuario(const Usuario *u);
//void anyadirUsuario(Usuario usuarios[], int *numUsuarios, Usuario nuevo);
void anadirUsuario(sqlite3 *db);
void anyadirUsuario(sqlite3 *db, Usuario *usuario);
//void eliminarUsuario(Usuario usuarios[], int *numUsuarios, int id);
//void modificarUsuario(Usuario usuarios[], int numUsuarios, int id, const char *nombre, const char *correo, const char *contrasenya, const char *telefono);
void modificarUsuario(sqlite3 *db);
void eliminarUsuario(sqlite3 *db);
void listarUsuarios(sqlite3 *db);
void liberarUsuario(Usuario *u);

#endif

