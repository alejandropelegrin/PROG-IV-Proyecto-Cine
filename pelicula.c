#include "pelicula.h"
#include <stdio.h>
#include <string.h>

void crearPelicula(Pelicula *p, int id, const char *titulo, int duracion, const char *genero) {
    p->id = id;
    strncpy(p->titulo, titulo, sizeof(p->titulo) - 1);
    p->titulo[sizeof(p->titulo) - 1] = '\0';
    p->duracion = duracion;
    strncpy(p->genero, genero, sizeof(p->genero) - 1);
    p->genero[sizeof(p->genero) - 1] = '\0';
}

void imprimirPelicula(const Pelicula *p) {
    printf("ID: %d\n", p->id);
    printf("Titulo: %s\n", p->titulo);
    printf("Duracion: %d minutos\n", p->duracion);
    printf("Genero: %s\n", p->genero);
}

/*void anyadirPelicula(Pelicula peliculas[], int *numPeliculas, Pelicula nueva) {
    peliculas[*numPeliculas] = nueva;
    (*numPeliculas)++;
}*/

/*void eliminarPelicula(Pelicula peliculas[], int *numPeliculas, int id) {
    for (int i = 0; i < *numPeliculas; i++) {
        if (peliculas[i].id == id) {
            for (int j = i; j < *numPeliculas - 1; j++) {
                peliculas[j] = peliculas[j + 1];
            }
            (*numPeliculas)--;
            return;
        }
    }
    printf("Pelicula con ID %d no encontrada.\n", id);
}*/

void anadirPelicula(sqlite3 *db) {
    char titulo[50], genero[50];
    int duracion;
    
    printf("Introduce el titulo de la pelicula: ");
    fgets(titulo, sizeof(titulo), stdin);
    titulo[strcspn(titulo, "\n")] = '\0';

    printf("Introduce el genero de la pelicula: ");
    fgets(genero, sizeof(genero), stdin);
    genero[strcspn(genero, "\n")] = '\0';

    printf("Introduce la duracion de la pelicula en minutos: ");
    scanf("%d", &duracion);
    getchar();

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Pelicula (titulo, duracion, genero) VALUES (?, ?, ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, titulo, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, duracion);
    sqlite3_bind_text(stmt, 3, genero, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al insertar la pelicula: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Pelicula anyadida correctamente.\n");
    }

    sqlite3_finalize(stmt);
}

void eliminarPelicula(sqlite3 *db) {
    listarPeliculas(db);
    char titulo[50];

    printf("Introduce el titulo de la pelicula a eliminar: ");
    fgets(titulo, sizeof(titulo), stdin);
    titulo[strcspn(titulo, "\n")] = '\0';

    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM Pelicula WHERE titulo = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, titulo, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al eliminar la pelicula: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Pelicula eliminada correctamente.\n");
    }

    sqlite3_finalize(stmt);
}

void listarPeliculas(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, titulo, duracion, genero FROM Pelicula";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al obtener las peliculas: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\n=== PELICULAS DISPONIBLES ===\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *titulo = (const char *)sqlite3_column_text(stmt, 1);
        int duracion = sqlite3_column_int(stmt, 2);
        const char *genero = (const char *)sqlite3_column_text(stmt, 3);

        printf("ID: %d | Titulo: %s | Duracion: %d min | Genero: %s\n", id, titulo, duracion, genero);
    }

    sqlite3_finalize(stmt);
}

void modificarPelicula(Pelicula peliculas[], int numPeliculas, int id, const char *titulo, int duracion, const char *genero) {
    for (int i = 0; i < numPeliculas; i++) {
        if (peliculas[i].id == id) {
            strncpy(peliculas[i].titulo, titulo, sizeof(peliculas[i].titulo) - 1);
            peliculas[i].titulo[sizeof(peliculas[i].titulo) - 1] = '\0';
            peliculas[i].duracion = duracion;
            strncpy(peliculas[i].genero, genero, sizeof(peliculas[i].genero) - 1);
            peliculas[i].genero[sizeof(peliculas[i].genero) - 1] = '\0';
            return;
        }
    }
    printf("Pelicula con ID %d no encontrada.\n", id);
}
