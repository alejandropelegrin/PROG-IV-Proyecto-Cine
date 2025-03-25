#include "database.h"
#include "usuario.h"
#include "pelicula.h"
#include "sala.h"
#include "asiento.h"
#include "sesion.h"
#include <stdio.h>
#include <stdlib.h>

int inicializarBBDD(sqlite3 **db) {
    /*int result;
    result = sqlite3_open(NOMBRE_BBDD, db);
    return result;*/
	int result = sqlite3_open(NOMBRE_BBDD, db);
	    if (result != SQLITE_OK) {
	        fprintf(stderr, "No se pudo abrir la base de datos: %s\n", sqlite3_errmsg(*db));
	        return result;
	    }
	    printf("Base de datos abierta correctamente en: %s\n", NOMBRE_BBDD);
	    return SQLITE_OK;
}

void crearTablas(sqlite3 *db) {
    sqlite3_stmt *stmt;
    char sql[256];

    // Tabla Usuarios
    sprintf(sql, "CREATE TABLE IF NOT EXISTS Usuario (id INTEGER PRIMARY KEY, nombre TEXT, correo TEXT UNIQUE, contrasenya TEXT, telefono TEXT, tipo INTEGER)");
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Tabla Peliculas
    sprintf(sql, "CREATE TABLE IF NOT EXISTS Pelicula (id INTEGER PRIMARY KEY, titulo TEXT, duracion INTEGER, genero TEXT)");
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Tabla Salas
    sprintf(sql, "CREATE TABLE IF NOT EXISTS Sala (id INTEGER PRIMARY KEY, numero_asientos INTEGER)");
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Tabla Asientos
    sprintf(sql, "CREATE TABLE IF NOT EXISTS Asiento (id INTEGER PRIMARY KEY, sala_id INTEGER, numero INTEGER, estado INTEGER, FOREIGN KEY(sala_id) REFERENCES Sala(id))");
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Tabla Sesiones
    sprintf(sql, "CREATE TABLE IF NOT EXISTS Sesion (id INTEGER PRIMARY KEY, pelicula_id INTEGER, sala_id INTEGER, hora_inicio TEXT, hora_fin TEXT, FOREIGN KEY(pelicula_id) REFERENCES Pelicula(id), FOREIGN KEY(sala_id) REFERENCES Sala(id))");
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void volcarFicheroALaBBDD(char *nomfich, sqlite3 *db) {
    FILE *pf;
    char line[256];
    sqlite3_stmt *stmt;
    pf = fopen(nomfich, "r");  // Abrir en modo texto para lectura

    if (pf == NULL) {
        printf("Error al abrir el archivo %s\n", nomfich);
        return;
    }

    // Saltar la primera línea (encabezados)
    fgets(line, sizeof(line), pf);

    while (fgets(line, sizeof(line), pf) != NULL) {
        int id, tipo;
        char nombre[50], correo[50], contrasenya[50], telefono[50];

        // Leer los datos del archivo (formato: ID\tNombre\tCorreo\tContraseña\tTelefono\tTipo)
        if (sscanf(line, "%d\t%49[^\t]\t%49[^\t]\t%49[^\t]\t%49[^\t]\t%d",
                  &id, nombre, correo, contrasenya, telefono, &tipo) != 6) {
            printf("Error al leer la línea: %s\n", line);
            continue;
        }

        // Insertar en la base de datos
        char sql[256];
        snprintf(sql, sizeof(sql),
                "INSERT INTO Usuario (id, nombre, correo, contrasenya, telefono, tipo) VALUES (%d, '%s', '%s', '%s', '%s', %d)",
                id, nombre, correo, contrasenya, telefono, tipo);

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
            printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
            continue;
        }

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error al insertar el usuario: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    }

    fclose(pf);
}

void volcarBBDDAlFichero(char *nomfich, sqlite3 *db) {
    sqlite3_stmt *stmt;
    char sql[] = "SELECT * FROM Usuario";  // Consulta para obtener todos los usuarios
    FILE *pf = fopen(nomfich, "w");  // Abre el archivo en modo texto de escritura (se crea si no existe)

    if (pf == NULL) {
        printf("Error al abrir el archivo para escribir\n");
        return;
    }

    // Prepara la consulta SQL para obtener los usuarios
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        fclose(pf);
        return;
    }

    // Escribe los encabezados en el archivo
    fprintf(pf, "ID\tNombre\tCorreo\tContraseña\tTelefono\tTipo\n");

    // Extrae los datos de los usuarios y escribe cada uno en el archivo
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *nombre = (const char *)sqlite3_column_text(stmt, 1);
        const char *correo = (const char *)sqlite3_column_text(stmt, 2);
        const char *contrasenya = (const char *)sqlite3_column_text(stmt, 3);
        const char *telefono = (const char *)sqlite3_column_text(stmt, 4);
        int tipo = sqlite3_column_int(stmt, 5);

        // Escribe los datos de los usuarios en formato texto en el archivo
        fprintf(pf, "%d\t%s\t%s\t%s\t%s\t%d\n", id, nombre, correo, contrasenya, telefono, tipo);
    }

    sqlite3_finalize(stmt);  // Finaliza la consulta SQL
    fclose(pf);  // Cierra el archivo después de escribir
}
