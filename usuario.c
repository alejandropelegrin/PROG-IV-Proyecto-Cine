#include "usuario.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Función para crear un usuario y asignar valores a los campos
void crearUsuario(Usuario *u, int id, const char *nombre, const char *correo, const char *contrasenya, const char *telefono, int tipo) {
    u->id = id;
    u->nombre = strdup(nombre);      // Duplica la cadena
    u->correo = strdup(correo);      // Duplica la cadena
    u->contrasenya = strdup(contrasenya); // Duplica la cadena
    u->telefono = strdup(telefono); // Duplica la cadena
    u->tipo = tipo;
}

// Función para imprimir un usuario
void imprimirUsuario(const Usuario *u) {
    printf("ID: %d\n", u->id);
    printf("Nombre: %s\n", u->nombre);
    printf("Correo: %s\n", u->correo);
    printf("Contraseña: %s\n", u->contrasenya);
    printf("Teléfono: %s\n", u->telefono);
    printf("Tipo: %d\n", u->tipo);
}

// Función para añadir un usuario al arreglo
void anyadirUsuario(Usuario usuarios[], int *numUsuarios, Usuario nuevo) {
    usuarios[*numUsuarios] = nuevo;
    (*numUsuarios)++;
}

void anyadirUsuario2(sqlite3 *db, Usuario *usuario) {
    sqlite3_stmt *stmt;
    char sql[256];

    // Inserción de usuario en la base de datos
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


// Función para eliminar un usuario del arreglo
void eliminarUsuario(Usuario usuarios[], int *numUsuarios, int id) {
    int i, j;
    for (i = 0; i < *numUsuarios; i++) {
        if (usuarios[i].id == id) {
            // Liberar memoria dinámica
            free(usuarios[i].nombre);
            free(usuarios[i].correo);
            free(usuarios[i].contrasenya);
            free(usuarios[i].telefono);

            // Desplazar los elementos para eliminar el usuario
            for (j = i; j < *numUsuarios - 1; j++) {
                usuarios[j] = usuarios[j + 1];
            }

            (*numUsuarios)--;
            return;
        }
    }
    printf("Usuario con ID %d no encontrado.\n", id);
}

// Función para modificar un usuario en el arreglo
void modificarUsuario(Usuario usuarios[], int numUsuarios, int id, const char *nombre, const char *correo, const char *contrasenya, const char *telefono, int tipo) {
    for (int i = 0; i < numUsuarios; i++) {
        if (usuarios[i].id == id) {
            // Liberar memoria actual de las cadenas
            free(usuarios[i].nombre);
            free(usuarios[i].correo);
            free(usuarios[i].contrasenya);
            free(usuarios[i].telefono);

            // Asignar nuevos valores
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

// Función para liberar la memoria dinámica de un usuario
void liberarUsuario(Usuario *u) {
    free(u->nombre);
    free(u->correo);
    free(u->contrasenya);
    free(u->telefono);
}
