#include "sesion.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void crearSesion(Sesion *s, int id, int pelicula_id, int sala_id, const char *hora_inicio, const char *hora_fin) {
    s->id = id;
    s->pelicula_id = pelicula_id;
    s->sala_id = sala_id;
    strncpy(s->hora_inicio, hora_inicio, sizeof(s->hora_inicio) - 1);
    s->hora_inicio[sizeof(s->hora_inicio) - 1] = '\0';
    strncpy(s->hora_fin, hora_fin, sizeof(s->hora_fin) - 1);
    s->hora_fin[sizeof(s->hora_fin) - 1] = '\0';
}

void imprimirSesion(const Sesion *s) {
    printf("Sesion ID: %d\n", s->id);
    printf("Pelicula ID: %d\n", s->pelicula_id);
    printf("Sala ID: %d\n", s->sala_id);
    printf("Hora Inicio: %s\n", s->hora_inicio);
    printf("Hora Fin: %s\n", s->hora_fin);
}

void listarSesiones(sqlite3 *db) {
    const char *sql = "SELECT s.id, p.titulo, s.sala_id, s.hora_inicio, s.hora_fin "
                     "FROM Sesion s JOIN Pelicula p ON s.pelicula_id = p.id "
                     "ORDER BY s.hora_inicio";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    printf("\n=== LISTADO DE SESIONES ===\n");
    printf("ID  Pelicula\t\tSala  Hora Inicio  Hora Fin\n");
    printf("--------------------------------------------\n");
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%2d  %-20s %4d  %11s  %8s\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_int(stmt, 2),
               sqlite3_column_text(stmt, 3),
               sqlite3_column_text(stmt, 4));
    }
    
    sqlite3_finalize(stmt);
}

void anyadirSesionDB(sqlite3 *db) {
    Sesion nueva;
    printf("\n=== ANYADIR NUEVA SESION ===\n");
    
    printf("ID de la sesion: ");
    scanf("%d", &nueva.id);
    getchar();
    
    printf("ID de la pelicula: ");
    scanf("%d", &nueva.pelicula_id);
    getchar();
    
    printf("ID de la sala: ");
    scanf("%d", &nueva.sala_id);
    getchar();
    
    printf("Hora de inicio (HH:MM): ");
    fgets(nueva.hora_inicio, sizeof(nueva.hora_inicio), stdin);
    nueva.hora_inicio[strcspn(nueva.hora_inicio, "\n")] = '\0';
    
    printf("Hora de fin (HH:MM): ");
    fgets(nueva.hora_fin, sizeof(nueva.hora_fin), stdin);
    nueva.hora_fin[strcspn(nueva.hora_fin, "\n")] = '\0';
    
    char *sql = "INSERT INTO Sesion (id, pelicula_id, sala_id, hora_inicio, hora_fin) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, nueva.id);
    sqlite3_bind_int(stmt, 2, nueva.pelicula_id);
    sqlite3_bind_int(stmt, 3, nueva.sala_id);
    sqlite3_bind_text(stmt, 4, nueva.hora_inicio, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, nueva.hora_fin, -1, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al insertar la sesion: %s\n", sqlite3_errmsg(db));
    } else {
        printf("\nSesion añadida con exito.\n");
        imprimirSesion(&nueva);
    }
    
    sqlite3_finalize(stmt);
}

void modificarSesionDB(sqlite3 *db) {
    int id;
    printf("\n=== MODIFICAR SESIÓN ===\n");
    listarSesiones(db);
    
    printf("\nID de la sesion a modificar: ");
    scanf("%d", &id);
    getchar();
    
    const char *sql_select = "SELECT * FROM Sesion WHERE id = ?;";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        printf("Sesión no encontrada.\n");
        sqlite3_finalize(stmt);
        return;
    }
    
    Sesion s;
    s.id = id;
    s.pelicula_id = sqlite3_column_int(stmt, 1);
    s.sala_id = sqlite3_column_int(stmt, 2);
    strncpy(s.hora_inicio, (const char*)sqlite3_column_text(stmt, 3), sizeof(s.hora_inicio));
    strncpy(s.hora_fin, (const char*)sqlite3_column_text(stmt, 4), sizeof(s.hora_fin));
    
    sqlite3_finalize(stmt);
    
    printf("\nDeje en blanco los campos que no desea modificar\n");
    
    char input[50];
    printf("Nuevo ID de película (actual %d): ", s.pelicula_id);
    fgets(input, sizeof(input), stdin);
    if (strlen(input) > 1) {
        s.pelicula_id = atoi(input);
    }
    
    printf("Nuevo ID de sala (actual %d): ", s.sala_id);
    fgets(input, sizeof(input), stdin);
    if (strlen(input) > 1) {
        s.sala_id = atoi(input);
    }
    
    printf("Nueva hora de inicio (actual %s): ", s.hora_inicio);
    fgets(input, sizeof(input), stdin);
    if (strlen(input) > 1) {
        strncpy(s.hora_inicio, input, sizeof(s.hora_inicio));
        s.hora_inicio[strcspn(s.hora_inicio, "\n")] = '\0';
    }
    
    printf("Nueva hora de fin (actual %s): ", s.hora_fin);
    fgets(input, sizeof(input), stdin);
    if (strlen(input) > 1) {
        strncpy(s.hora_fin, input, sizeof(s.hora_fin));
        s.hora_fin[strcspn(s.hora_fin, "\n")] = '\0';
    }
    
    const char *sql_update = "UPDATE Sesion SET pelicula_id = ?, sala_id = ?, hora_inicio = ?, hora_fin = ? WHERE id = ?;";
    
    if (sqlite3_prepare_v2(db, sql_update, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la actualización: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, s.pelicula_id);
    sqlite3_bind_int(stmt, 2, s.sala_id);
    sqlite3_bind_text(stmt, 3, s.hora_inicio, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, s.hora_fin, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, s.id);
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al modificar la sesion: %s\n", sqlite3_errmsg(db));
    } else {
        printf("\nSesion modificada con exito.\n");
        imprimirSesion(&s);
    }
    
    sqlite3_finalize(stmt);
}

void eliminarSesionDB(sqlite3 *db) {
    int id;
    printf("\n=== ELIMINAR SESION ===\n");
    listarSesiones(db);
    
    printf("\nID de la sesion a eliminar: ");
    scanf("%d", &id);
    getchar();
    
    printf("\n¿Esta seguro que desea eliminar esta sesion? (s/n): ");
    char confirmacion;
    scanf("%c", &confirmacion);
    
    if (confirmacion != 's' && confirmacion != 'S') {
        printf("Eliminacion cancelada.\n");
        return;
    }
    
    char *sql = "DELETE FROM Sesion WHERE id = ?;";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al eliminar la sesion: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Sesion eliminada con exito.\n");
    }
    
    sqlite3_finalize(stmt);
}

void verDetallesSesionPorPelicula(sqlite3 *db) {
    int pelicula_id;
    printf("\n=== VER SESIONES POR PELICULA ===\n");
    
    printf("\n=== PELICULAS DISPONIBLES ===\n");
    const char *sql_peliculas = "SELECT id, titulo FROM Pelicula ORDER BY titulo;";
    sqlite3_stmt *stmt_peliculas;
    
    if (sqlite3_prepare_v2(db, sql_peliculas, -1, &stmt_peliculas, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta de películas: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    printf("ID  Titulo\n");
    printf("----------\n");
    while (sqlite3_step(stmt_peliculas) == SQLITE_ROW) {
        printf("%2d  %s\n", 
               sqlite3_column_int(stmt_peliculas, 0),
               sqlite3_column_text(stmt_peliculas, 1));
    }
    sqlite3_finalize(stmt_peliculas);
    
    printf("\nID de la pelicula para ver sus sesiones: ");
    scanf("%d", &pelicula_id);
    getchar();
    
    const char *sql = "SELECT s.id, p.titulo, sa.numero_asientos, s.hora_inicio, s.hora_fin, sa.id "
                     "FROM Sesion s "
                     "JOIN Pelicula p ON s.pelicula_id = p.id "
                     "JOIN Sala sa ON s.sala_id = sa.id "
                     "WHERE s.pelicula_id = ? "
                     "ORDER BY s.hora_inicio;";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, pelicula_id);
    
    printf("\n=== SESIONES PARA ESTA PELICULA ===\n");
    int encontradas = 0;
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        encontradas++;
        printf("\n--- Sesion ID: %d ---\n", sqlite3_column_int(stmt, 0));
        printf("Pelicula: %s\n", sqlite3_column_text(stmt, 1));
        printf("Sala: %d (Asientos: %d)\n", sqlite3_column_int(stmt, 5), sqlite3_column_int(stmt, 2));
        printf("Hora Inicio: %s\n", sqlite3_column_text(stmt, 3));
        printf("Hora Fin: %s\n", sqlite3_column_text(stmt, 4));
    }
    
    if (encontradas == 0) {
        printf("No se encontraron sesiones para esta pelicula.\n");
    }
    
    sqlite3_finalize(stmt);
}

/*void anyadirSesion(Sesion sesiones[], int *numSesiones, Sesion nueva) {
    sesiones[*numSesiones] = nueva;
    (*numSesiones)++;
}

void eliminarSesion(Sesion sesiones[], int *numSesiones, int id) {
    for (int i = 0; i < *numSesiones; i++) {
        if (sesiones[i].id == id) {
            for (int j = i; j < *numSesiones - 1; j++) {
                sesiones[j] = sesiones[j + 1];
            }
            (*numSesiones)--;
            return;
        }
    }
}

void modificarSesion(Sesion sesiones[], int numSesiones, int id, const char *hora_inicio, const char *hora_fin) {
    for (int i = 0; i < numSesiones; i++) {
        if (sesiones[i].id == id) {
            strncpy(sesiones[i].hora_inicio, hora_inicio, sizeof(sesiones[i].hora_inicio) - 1);
            sesiones[i].hora_inicio[sizeof(sesiones[i].hora_inicio) - 1] = '\0';
            strncpy(sesiones[i].hora_fin, hora_fin, sizeof(sesiones[i].hora_fin) - 1);
            sesiones[i].hora_fin[sizeof(sesiones[i].hora_fin) - 1] = '\0';
            return;
        }
    }
}
*/