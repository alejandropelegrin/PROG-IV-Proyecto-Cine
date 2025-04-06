#include "usuario.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void crearUsuario(Usuario *u, int id, const char *nombre, const char *correo, const char *contrasenya, const char *telefono) {
    u->id = id;
    u->nombre = strdup(nombre); 
    u->correo = strdup(correo);
    u->contrasenya = strdup(contrasenya);
    u->telefono = strdup(telefono);
}

void imprimirUsuario(const Usuario *u) {
    printf("ID: %d\n", u->id);
    printf("Nombre: %s\n", u->nombre);
    printf("Correo: %s\n", u->correo);
    printf("Contrasenya: %s\n", u->contrasenya);
    printf("Telefono: %s\n", u->telefono);
}

/*void anyadirUsuario(Usuario usuarios[], int *numUsuarios, Usuario nuevo) {
    usuarios[*numUsuarios] = nuevo;
    (*numUsuarios)++;
}*/

void anadirUsuario(sqlite3 *db) {
    char nombre[50], correo[50], contrasenya[50], telefono[20];

    printf("Introduce el nombre del usuario: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    printf("Introduce el correo del usuario: ");
    fgets(correo, sizeof(correo), stdin);
    correo[strcspn(correo, "\n")] = '\0';

    printf("Introduce la contrasenya del usuario: ");
    fgets(contrasenya, sizeof(contrasenya), stdin);
    contrasenya[strcspn(contrasenya, "\n")] = '\0';

    printf("Introduce el telefono del usuario: ");
    fgets(telefono, sizeof(telefono), stdin);
    telefono[strcspn(telefono, "\n")] = '\0';

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Usuario (nombre, correo, contrasenya, telefono) VALUES (?, ?, ?, ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, correo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, contrasenya, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, telefono, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al insertar el usuario: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Usuario anyadido correctamente.\n");
    }

    sqlite3_finalize(stmt);
}

void eliminarUsuario(sqlite3 *db) {
    listarUsuarios(db);
    int id;

    printf("Introduce el ID del usuario a eliminar: ");
    scanf("%d", &id);
    getchar();

    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM Usuario WHERE id = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al eliminar el usuario: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Usuario eliminado correctamente.\n");
    }

    sqlite3_finalize(stmt);
}

void modificarUsuario(sqlite3 *db) {
    listarUsuarios(db);
    int id;
    char nombre[50], correo[50], contrasenya[50], telefono[20];

    printf("Introduce el ID del usuario a modificar: ");
    scanf("%d", &id);
    getchar();

    printf("Introduce el nuevo nombre: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    printf("Introduce el nuevo correo: ");
    fgets(correo, sizeof(correo), stdin);
    correo[strcspn(correo, "\n")] = '\0';

    printf("Introduce la nueva contrasenya: ");
    fgets(contrasenya, sizeof(contrasenya), stdin);
    contrasenya[strcspn(contrasenya, "\n")] = '\0';

    printf("Introduce el nuevo telefono: ");
    fgets(telefono, sizeof(telefono), stdin);
    telefono[strcspn(telefono, "\n")] = '\0';

    sqlite3_stmt *stmt;
    const char *sql = "UPDATE Usuario SET nombre = ?, correo = ?, contrasenya = ?, telefono = ? WHERE id = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, correo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, contrasenya, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, telefono, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al modificar el usuario: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Usuario modificado correctamente.\n");
    }

    sqlite3_finalize(stmt);
}

void anyadirUsuario(sqlite3 *db, Usuario *usuario) {
    sqlite3_stmt *stmt;
    char sql[256];

    snprintf(sql, sizeof(sql), "INSERT INTO Usuario (nombre, correo, contrasenya, telefono) VALUES ('%s', '%s', '%s', '%s')",
            usuario->nombre, usuario->correo, usuario->contrasenya, usuario->telefono);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al insertar el usuario: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}


/*void eliminarUsuario(Usuario usuarios[], int *numUsuarios, int id) {
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

void modificarUsuario(Usuario usuarios[], int numUsuarios, int id, const char *nombre, const char *correo, const char *contrasenya, const char *telefono) {
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
            return;
        }
    }
    printf("Usuario con ID %d no encontrado.\n", id);
}*/

void listarUsuarios(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, nombre, correo, telefono FROM Usuario";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al listar usuarios: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\n=== LISTADO DE USUARIOS ===\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *nombre = (const char *)sqlite3_column_text(stmt, 1);
        const char *correo = (const char *)sqlite3_column_text(stmt, 2);
        const char *telefono = (const char *)sqlite3_column_text(stmt, 3);

        printf("ID: %d | Nombre: %s | Correo: %s | Telefono: %s\n", id, nombre, correo, telefono);
    }

    sqlite3_finalize(stmt);
}

void liberarUsuario(Usuario *u) {
    free(u->nombre);
    free(u->correo);
    free(u->contrasenya);
    free(u->telefono);
}
