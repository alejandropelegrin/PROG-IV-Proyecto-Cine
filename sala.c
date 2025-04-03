#include "sala.h"
#include "database.h"
#include <stdio.h>

void crearSala(sqlite3 *db, int id, int numero_asientos) {
    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO Sala (id, numero_asientos) VALUES (%d, %d);", id, numero_asientos);

    if (sqlite3_exec(db, sql, 0, 0, NULL) != SQLITE_OK) {
        printf("Error al crear la sala.\n");
    } else {
        printf("Sala creada correctamente.\n");
    }
}

void imprimirSala(sqlite3 *db, int id) {
    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT id, numero_asientos FROM Sala WHERE id = %d;", id);

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("Sala ID: %d\n", sqlite3_column_int(stmt, 0));
            printf("Número de asientos: %d\n", sqlite3_column_int(stmt, 1));
        } else {
            printf("No se encontró la sala con ID %d.\n", id);
        }
    }
    sqlite3_finalize(stmt);
}

void anyadirSala(sqlite3 *db, Sala nueva) {
    crearSala(db, nueva.id, nueva.numero_asientos);
}

void eliminarSala(sqlite3 *db, int id) {
    char sql[256];
    snprintf(sql, sizeof(sql), "DELETE FROM Sala WHERE id = %d;", id);

    if (sqlite3_exec(db, sql, 0, 0, NULL) != SQLITE_OK) {
        printf("Error al eliminar la sala.\n");
    } else {
        printf("Sala eliminada correctamente.\n");
    }
}

void modificarSala(sqlite3 *db, int id, int numero_asientos) {
    char sql[256];
    snprintf(sql, sizeof(sql), "UPDATE Sala SET numero_asientos = %d WHERE id = %d;", numero_asientos, id);

    if (sqlite3_exec(db, sql, 0, 0, NULL) != SQLITE_OK) {
        printf("Error al modificar la sala.\n");
    } else {
        printf("Sala modificada correctamente.\n");
    }
}
