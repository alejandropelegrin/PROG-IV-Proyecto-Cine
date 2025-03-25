#include "usuario.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void crearUsuario(Usuario *u, int id, const char *nombre, const char *correo, const char *contrasenya, const char *telefono, int tipo) {
    u->id = id;
    u->nombre = strdup(nombre); 
    u->correo = strdup(correo);
    u->contrasenya = strdup(contrasenya);
    u->telefono = strdup(telefono);
    u->tipo = tipo;
}

void imprimirUsuario(const Usuario *u) {
    printf("ID: %d\n", u->id);
    printf("Nombre: %s\n", u->nombre);
    printf("Correo: %s\n", u->correo);
    printf("Contraseña: %s\n", u->contrasenya);
    printf("Teléfono: %s\n", u->telefono);
    printf("Tipo: %d\n", u->tipo);
}

void anyadirUsuario(Usuario usuarios[], int *numUsuarios, Usuario nuevo) {
    usuarios[*numUsuarios] = nuevo;
    (*numUsuarios)++;
}

void anyadirUsuario2(sqlite3 *db, Usuario *usuario) {
    sqlite3_stmt *stmt;
    char sql[256];

    snprintf(sql, sizeof(sql), "INSERT INTO Usuario (nombre, correo, contrasenya, telefono, tipo) VALUES ('%s', '%s', '%s', '%s', %d)",
            usuario->nombre, usuario->correo, usuario->contrasenya, usuario->telefono, usuario->tipo);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al insertar el usuario: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}


void eliminarUsuario(Usuario usuarios[], int *numUsuarios, int id) {
    int i, j;
    for (i = 0; i < *numUsuarios; i++) {
        if (usuarios[i].id == id) {
            free(usuarios[i].nombre);
            free(usuarios[i].correo);
            free(usuarios[i].contrasenya);
            free(usuarios[i].telefono);

            for (j = i; j < *numUsuarios - 1; j++) {
                usuarios[j] = usuarios[j + 1];
            }

            (*numUsuarios)--;
            return;
        }
    }
    printf("Usuario con ID %d no encontrado.\n", id);
}

void modificarUsuario(Usuario usuarios[], int numUsuarios, int id, const char *nombre, const char *correo, const char *contrasenya, const char *telefono, int tipo) {
    for (int i = 0; i < numUsuarios; i++) {
        if (usuarios[i].id == id) {
            free(usuarios[i].nombre);
            free(usuarios[i].correo);
            free(usuarios[i].contrasenya);
            free(usuarios[i].telefono);

            usuarios[i].nombre = strdup(nombre);
            usuarios[i].correo = strdup(correo);
            usuarios[i].contrasenya = strdup(contrasenya);
            usuarios[i].telefono = strdup(telefono);
            usuarios[i].tipo = tipo;
            return;
        }
    }
    printf("Usuario con ID %d no encontrado.\n", id);
}

void liberarUsuario(Usuario *u) {
    free(u->nombre);
    free(u->correo);
    free(u->contrasenya);
    free(u->telefono);
}
