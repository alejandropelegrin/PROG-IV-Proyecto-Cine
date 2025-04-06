#include "database.h"
#include "usuario.h"
#include "pelicula.h"
#include "sala.h"
#include "asiento.h"
#include "sesion.h"
#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "entrada.h"

#define FICHERO_DATOS "usuarios.txt"
#define NOMBRE_BBDD "Cine.db"

void mostrarMenuPrincipal();
void mostrarMenuUsuario();
void mostrarMenuAdministrador();
void gestionarPeliculas(sqlite3 *db);
void menuSalas(sqlite3 *db);
int iniciarSesion(sqlite3 *db);
void gestionarUsuarios(sqlite3 *db);
void registrarUsuario(sqlite3 *db);
void listarSesionesConPeliculas(sqlite3 *db);
void comprarEntrada(sqlite3 *db, ListaEntradas *lista, int usuario_id);

ListaEntradas listaEntradas;

int main() {
    sqlite3 *db;
    int opcion;

    inicializarListaEntradas(&listaEntradas);

    if (inicializarBBDD(&db) != SQLITE_OK) {
        printf("Error al conectar con la base de datos.\n");
        return 1;
    }

    crearTablas(db);

    volcarFicheroALaBBDD(FICHERO_DATOS, db);

    // Menú principal
    while (1) {
        mostrarMenuPrincipal();
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                if (iniciarSesion(db)) {
                }
                break;
            case 2:
                registrarUsuario(db);
                break;
            case 3:
                printf("Saliendo del sistema...\n");
                sqlite3_close(db);
                return 0;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    }

    return 0;
}

void mostrarMenuPrincipal() {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1. Iniciar sesion\n");
    printf("2. Registrarse\n");
    printf("3. Salir\n");
}

void mostrarMenuUsuario() {
    printf("\n=== MENU USUARIO ===\n");
    printf("1. Ver peliculas disponibles\n");
    printf("2. Comprar entrada\n");
    printf("3. Ver mis entradas\n");
    printf("4. Cerrar sesion\n");
    printf("Seleccione una opcion: ");
}

void mostrarMenuAdministrador() {
    printf("\n=== MENU ADMINISTRADOR ===\n");
    printf("1. Gestionar usuarios\n");
    printf("2. Gestionar peliculas\n");
    printf("3. Gestionar salas\n");
    printf("4. Gestionar sesion\n");
    printf("5. Ver estadisticas\n");
    printf("6. Cerrar sesion\n");
    printf("Seleccione una opcion: ");
}

void listarSesionesConPeliculas(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT s.id, p.titulo, s.sala_id, s.hora_inicio, s.hora_fin "
                      "FROM Sesion s JOIN Pelicula p ON s.pelicula_id = p.id "
                      "ORDER BY s.hora_inicio";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al obtener las sesiones: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\n=== SESIONES DISPONIBLES ===\n");
    printf("ID  Pelicula\t\tSala  Hora Inicio  Hora Fin\n");
    printf("--------------------------------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *titulo = (const char *)sqlite3_column_text(stmt, 1);
        int sala_id = sqlite3_column_int(stmt, 2);
        const char *hora_inicio = (const char *)sqlite3_column_text(stmt, 3);
        const char *hora_fin = (const char *)sqlite3_column_text(stmt, 4);

        printf("%2d  %-20s %4d  %11s  %8s\n", 
               id, titulo, sala_id, hora_inicio, hora_fin);
    }

    sqlite3_finalize(stmt);
}

int iniciarSesion(sqlite3 *db) {
    char nombre[50], contrasena[50];
    sqlite3_stmt *stmt;
    const char *sql;
    int opcion;
    int usuario_id = 0;

    printf("\n=== INICIAR SESION ===\n");
    printf("Nombre de usuario: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    printf("Contrasenya: ");
    fgets(contrasena, sizeof(contrasena), stdin);
    contrasena[strcspn(contrasena, "\n")] = '\0';

    if (strcmp(nombre, "admin") == 0 && strcmp(contrasena, "admin") == 0) {
        printf("\nBienvenido, Administrador!\n");

        // Menú de administrador
        while (1) {
            mostrarMenuAdministrador();
            scanf("%d", &opcion);
            getchar();

            switch (opcion) {
                case 1:
                    printf("\nGestion de usuarios\n");
                    gestionarUsuarios(db);                    
                    break;
                case 2:
                    printf("\nGestion de peliculas\n");
                    gestionarPeliculas(db);                    
                    break;
                case 3:
                    printf("\nGestion de salas\n");
                    menuSalas(db);                   
                    break;
                case 4:
                    printf("\nGestion de sesiones\n");
                    //gestionarSesiones(db);                    
                    break;
                case 5:
                    printf("\nEstadisticas\n");
                    
                    break;
                case 6:
                    printf("Cerrando sesion de administrador...\n");
                    return 1;
                default:
                    printf("Opcion no valida.\n");
            }
        }
    }

    sql = "SELECT tipo FROM Usuario WHERE nombre = ? AND contrasenya = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al verificar credenciales: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        usuario_id = sqlite3_column_int(stmt, 0);
        int tipo = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        printf("\nBienvenido, %s!\n", nombre);

        // Menú de usuario
        while (1) {
            mostrarMenuUsuario();
            scanf("%d", &opcion);
            getchar();

            switch (opcion) {
                case 1:
                    printf("\nPeliculas disponibles:\n");
                    listarPeliculas(db);                    
                    break;
                case 2:
                    printf("\nCompra de entradas\n");
                    comprarEntrada(db, &listaEntradas, usuario_id);
                    break;
                case 3:
                    printf("\nMis entradas\n");
                    listarEntradasUsuario(&listaEntradas, usuario_id);
                    break;
                case 4:
                    printf("Cerrando sesion...\n");
                    return 1;
                default:
                    printf("Opcion no valida.\n");
            }
        }  
    } else {
        printf("Credenciales incorrectas. Intente de nuevo.\n");
        sqlite3_finalize(stmt);
        return 0;
    }

    return 1;
}

void gestionarPeliculas(sqlite3 *db) {
    int opcion;
    while (1) {
        printf("\n=== GESTIONAR PELICULAS ===\n");
        printf("1. Ver peliculas\n");
        printf("2. Anyadir pelicula\n");
        printf("3. Eliminar pelicula\n");
        printf("4. Volver\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                listarPeliculas(db);
                break;
            case 2:
                anadirPelicula(db);
                break;
            case 3:
                eliminarPelicula(db);
                break;
            case 4:
                return;
            default:
                printf("Opcion no valida.\n");
        }
    }
}

void gestionarUsuarios(sqlite3 *db) {
    int opcion;
    while (1) {
        printf("\n=== GESTIONAR USUARIOS ===\n");
        printf("1. Ver usuarios\n");
        printf("2. Anyadir usuario\n");
        printf("3. Eliminar usuario\n");
        printf("4. Modificar usuario\n");
        printf("5. Volver\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                listarUsuarios(db);
                break;
            case 2:
                anadirUsuario(db);
                break;
            case 3:
                eliminarUsuario(db);
                break;
            case 4:
                modificarUsuario(db);
                break;
            case 5:
                return;
            default:
                printf("Opcion no valida.\n");
        }
    }
}

void menuSalas(sqlite3 *db) {
    int opcion, id, numero_asientos;
    Sala nueva;

    do {
        printf("\n--- Gestion de Salas ---\n");
        printf("1. Ver salas\n");
        printf("2. Anyadir sala\n");
        printf("3. Eliminar sala\n");
        printf("4. Modificar sala\n");
        printf("5. Ver sala\n");
        printf("6. Volver al menu principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                listarSalas(db);
                break;
            case 2:
                printf("Ingrese ID de la sala: ");
                scanf("%d", &nueva.id);
                printf("Ingrese numero de asientos: ");
                scanf("%d", &nueva.numero_asientos);
                anyadirSala(db, nueva);
                break;
            case 3:
                listarSalas(db);
                printf("Ingrese ID de la sala a eliminar: ");
                scanf("%d", &id);
                eliminarSala(db, id);
                break;
            case 4:
                listarSalas(db);
                printf("Ingrese ID de la sala a modificar: ");
                scanf("%d", &id);
                printf("Ingrese nuevo numero de asientos: ");
                scanf("%d", &numero_asientos);
                modificarSala(db, id, numero_asientos);
                break;
            case 5:
                listarSalas(db);
                printf("Ingrese ID de la sala a ver: ");
                scanf("%d", &id);
                imprimirSala(db, id);
                break;
            case 6:
                printf("Volviendo al menu principal...\n");
                return;
            default:
                printf("Opcion no valida, intente de nuevo.\n");
        }
    } while (opcion != 5);
}

void comprarEntrada(sqlite3 *db, ListaEntradas *lista, int usuario_id) {
    listarSesionesConPeliculas(db);
    
    int sesion_id;
    printf("\nIntroduce el ID de la sesion para la que quieres comprar entrada: ");
    scanf("%d", &sesion_id);
    getchar();
    
    sqlite3_stmt *stmt;
    const char *sql = "SELECT p.titulo, s.hora_inicio, s.sala_id, sa.numero_asientos "
                      "FROM Sesion s "
                      "JOIN Pelicula p ON s.pelicula_id = p.id "
                      "JOIN Sala sa ON s.sala_id = sa.id "
                      "WHERE s.id = ?";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al obtener la sesion: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, sesion_id);
    
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        printf("Sesion no encontrada.\n");
        sqlite3_finalize(stmt);
        return;
    }
    
    char nombre_pelicula[100], hora_inicio[10];
    int sala_id, total_asientos;
    
    strncpy(nombre_pelicula, (const char *)sqlite3_column_text(stmt, 0), sizeof(nombre_pelicula));
    strncpy(hora_inicio, (const char *)sqlite3_column_text(stmt, 1), sizeof(hora_inicio));
    sala_id = sqlite3_column_int(stmt, 2);
    total_asientos = sqlite3_column_int(stmt, 3);
    sqlite3_finalize(stmt);
    
    int asiento = generarAsientoDisponible(lista, sala_id, sesion_id, total_asientos);
    if (asiento == -1) {
        return;
    }
    
    Entrada nueva;
    int nuevo_id = lista->num_entradas + 1;
    crearEntrada(&nueva, nuevo_id, usuario_id, nombre_pelicula, sesion_id, hora_inicio, sala_id, asiento, 7.0);
    
    anyadirEntrada(lista, &nueva);
    
    printf("\n¡Compra realizada con exito!\n");
    printf("Entrada para: %s\n", nombre_pelicula);
    printf("Sesion ID: %d | Hora: %s\n", sesion_id, hora_inicio);
    printf("Sala: %d | Asiento: %d\n", sala_id, asiento);
    printf("Precio: 7.00€\n");
    
    imprimirEntrada(&nueva);
}

void registrarUsuario(sqlite3 *db) {
    Usuario nuevo;
    char nombre[50], correo[50], contrasenya[50], telefono[20];
    int tipo;

    printf("\n=== REGISTRO DE USUARIO ===\n");

    printf("Nombre de usuario: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    printf("Correo electronico: ");
    fgets(correo, sizeof(correo), stdin);
    correo[strcspn(correo, "\n")] = '\0';

    printf("Contrasenya: ");
    fgets(contrasenya, sizeof(contrasenya), stdin);
    contrasenya[strcspn(contrasenya, "\n")] = '\0';

    printf("Telefono: ");
    fgets(telefono, sizeof(telefono), stdin);
    telefono[strcspn(telefono, "\n")] = '\0';

    crearUsuario(&nuevo, 0, nombre, correo, contrasenya, telefono);

    anyadirUsuario(db, &nuevo);

    liberarUsuario(&nuevo);

    printf("\nUsuario registrado con exito!\n");

    volcarBBDDAlFichero(FICHERO_DATOS, db);
}
