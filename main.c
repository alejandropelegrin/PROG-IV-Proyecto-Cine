#include "database.h"
#include "usuario.h"
#include "pelicula.h"
#include "sala.h"
#include "asiento.h"
#include "sesion.h"
#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

#define FICHERO_DATOS "usuarios.txt"
#define NOMBRE_BBDD "Cine.db"

// Prototipos de funciones
void mostrarMenuPrincipal();
void mostrarMenuUsuario();
void mostrarMenuAdministrador();
void gestionarUsuarios(sqlite3 *db);
void gestionarPeliculas(sqlite3 *db);
void anadirPelicula(sqlite3 *db);
void eliminarPelicula(sqlite3 *db);
void modificarUsuario(sqlite3 *db);
void anadirUsuario(sqlite3 *db);
void eliminarUsuario(sqlite3 *db);
void listarPeliculas(sqlite3 *db);
void menuSalas(sqlite3 *db);
void crearSala(sqlite3 *db, int id, int numero_asientos);
void imprimirSala(sqlite3 *db, int id);
void anyadirSala(sqlite3 *db, Sala nueva);
void eliminarSala(sqlite3 *db, int id);
void modificarSala(sqlite3 *db, int id, int numero_asientos);
int iniciarSesion(sqlite3 *db);
void registrarUsuario(sqlite3 *db);

int main() {
    sqlite3 *db;
    int opcion;

    // Inicializar la base de datos
    if (inicializarBBDD(&db) != SQLITE_OK) {
        printf("Error al conectar con la base de datos.\n");
        return 1;
    }

    // Crear las tablas necesarias
    crearTablas(db);

    // Cargar datos iniciales
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
    printf("4. Ver estadisticas\n");
    printf("5. Cerrar sesion\n");
    printf("Seleccione una opcion: ");
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


int iniciarSesion(sqlite3 *db) {
    char nombre[50], contrasena[50];
    sqlite3_stmt *stmt;
    const char *sql;
    int opcion;

    printf("\n=== INICIAR SESION ===\n");
    printf("Nombre de usuario: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    printf("Contrasenya: ");
    fgets(contrasena, sizeof(contrasena), stdin);
    contrasena[strcspn(contrasena, "\n")] = '\0';

    // Verificar credenciales especiales de administrador
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
                    printf("\nEstadisticas\n");
                    
                    break;
                case 5:
                    printf("Cerrando sesion de administrador...\n");
                    return 1;
                default:
                    printf("Opcion no valida.\n");
            }
        }
    }

    // Verificar credenciales en la base de datos
    sql = "SELECT tipo FROM Usuario WHERE nombre = ? AND contrasenya = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al verificar credenciales: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
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
                    
                    break;
                case 3:
                    printf("\nMis entradas\n");
                    
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
        printf("1. Añadir pelicula\n");
        printf("2. Eliminar pelicula\n");
        printf("3. Volver\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                anadirPelicula(db);
                break;
            case 2:
                eliminarPelicula(db);
                break;
            case 3:
                return;
            default:
                printf("Opcion no valida.\n");
        }
    }
}

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

    // Insertar la pelicula en la base de datos
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
        printf("Pelicula añadida correctamente.\n");
    }

    sqlite3_finalize(stmt);
}

void eliminarPelicula(sqlite3 *db) {
    char titulo[50];

    printf("Introduce el titulo de la pelicula a eliminar: ");
    fgets(titulo, sizeof(titulo), stdin);
    titulo[strcspn(titulo, "\n")] = '\0';

    // Borrar la pelicula de la base de datos
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

void gestionarUsuarios(sqlite3 *db) {
    int opcion;
    while (1) {
        printf("\n=== GESTIONAR USUARIOS ===\n");
        printf("1. Añadir usuario\n");
        printf("2. Eliminar usuario\n");
        printf("3. Modificar usuario\n");
        printf("4. Volver\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                anadirUsuario(db);
                break;
            case 2:
                eliminarUsuario(db);
                break;
            case 3:
                modificarUsuario(db);
                break;
            case 4:
                return;
            default:
                printf("Opcion no valida.\n");
        }
    }
}

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

    // Insertar el usuario en la base de datos
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
    int id;

    printf("Introduce el ID del usuario a eliminar: ");
    scanf("%d", &id);
    getchar();

    // Borrar el usuario de la base de datos
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

    // Modificar el usuario en la base de datos
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

void menuSalas(sqlite3 *db) {
    int opcion, id, numero_asientos;
    Sala nueva;

    do {
        printf("\n--- Gestión de Salas ---\n");
        printf("1. Anyadir Sala\n");
        printf("2. Eliminar Sala\n");
        printf("3. Modificar Sala\n");
        printf("4. Ver Sala\n");
        printf("5. Volver al menu principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Ingrese ID de la sala: ");
                scanf("%d", &nueva.id);
                printf("Ingrese numero de asientos: ");
                scanf("%d", &nueva.numero_asientos);
                anyadirSala(db, nueva);
                break;
            case 2:
                printf("Ingrese ID de la sala a eliminar: ");
                scanf("%d", &id);
                eliminarSala(db, id);
                break;
            case 3:
                printf("Ingrese ID de la sala a modificar: ");
                scanf("%d", &id);
                printf("Ingrese nuevo numero de asientos: ");
                scanf("%d", &numero_asientos);
                modificarSala(db, id, numero_asientos);
                break;
            case 4:
                printf("Ingrese ID de la sala a ver: ");
                scanf("%d", &id);
                imprimirSala(db, id);
                break;
            case 5:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Opción no valida, intente de nuevo.\n");
        }
    } while (opcion != 5);
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

    // Crear el nuevo usuario
    crearUsuario(&nuevo, 0, nombre, correo, contrasenya, telefono);

    // Añadir a la base de datos
    anyadirUsuario2(db, &nuevo);

    // Liberar memoria
    liberarUsuario(&nuevo);

    printf("\nUsuario registrado con exito!\n");

    // Guardar también en el archivo
    volcarBBDDAlFichero(FICHERO_DATOS, db);
}
