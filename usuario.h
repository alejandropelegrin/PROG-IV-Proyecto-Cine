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
    int tipo;
} Usuario;

void crearUsuario(Usuario *u, int id, const char *nombre, const char *correo, const char *contrasenya, const char *telefono, int tipo);
void imprimirUsuario(const Usuario *u);
void anyadirUsuario(Usuario usuarios[], int *numUsuarios, Usuario nuevo);
void anyadirUsuario2(sqlite3 *db, Usuario *usuario);
void eliminarUsuario(Usuario usuarios[], int *numUsuarios, int id);
void modificarUsuario(Usuario usuarios[], int numUsuarios, int id, const char *nombre, const char *correo, const char *contrasenya, const char *telefono, int tipo);
void liberarUsuario(Usuario *u);

#endif

