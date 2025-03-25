#ifndef USUARIO_H
#define USUARIO_H

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

typedef struct {
    int id;
    char *nombre;      // Puntero a cadena (dinámico)
    char *correo;      // Puntero a cadena (dinámico)
    char *contrasenya; // Puntero a cadena (dinámico)
    char *telefono;    // Puntero a cadena (dinámico)
    int tipo;
} Usuario;

// Funciones para trabajar con la estructura Usuario
void crearUsuario(Usuario *u, int id, const char *nombre, const char *correo, const char *contrasenya, const char *telefono, int tipo);
void imprimirUsuario(const Usuario *u);
void anyadirUsuario(Usuario usuarios[], int *numUsuarios, Usuario nuevo);
void anyadirUsuario2(sqlite3 *db, Usuario *usuario);
void eliminarUsuario(Usuario usuarios[], int *numUsuarios, int id);
void modificarUsuario(Usuario usuarios[], int numUsuarios, int id, const char *nombre, const char *correo, const char *contrasenya, const char *telefono, int tipo);

// Función para liberar la memoria dinámica de un usuario
void liberarUsuario(Usuario *u);

#endif // USUARIO_H

