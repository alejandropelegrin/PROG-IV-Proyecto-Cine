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
    printf("4. Ver estadísticas\n");
    printf("5. Cerrar sesion\n");
    printf("Seleccione una opcion: ");
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
                    
                    break;
                case 2:
                    printf("\nGestion de peliculas\n");
                    
                    break;
                case 3:
                    printf("\nGestion de salas\n");
                    
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

        if (tipo == 1) { // Usuario normal
            printf("\nBienvenido, %s!\n", nombre);

            // Menú de usuario
            while (1) {
                mostrarMenuUsuario();
                scanf("%d", &opcion);
                getchar();

                switch (opcion) {
                    case 1:
                        printf("\nPeliculas disponibles:\n");
                        
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
        } else if (tipo == 2) { // Administrador
            printf("\nBienvenido, %s (Administrador)!\n", nombre);

            // Menú de administrador
            while (1) {
                mostrarMenuAdministrador();
                scanf("%d", &opcion);
                getchar();

                switch (opcion) {
                    case 1:
                        printf("\nGestion de usuarios\n");
                        break;
                    case 2:
                        printf("\nGestion de peliculas\n");
                        break;
                    case 3:
                        printf("\nGestion de salas\n");
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
    } else {
        printf("Credenciales incorrectas. Intente de nuevo.\n");
        sqlite3_finalize(stmt);
        return 0;
    }

    return 1;
}

void registrarUsuario(sqlite3 *db) {
    Usuario nuevo;
    char nombre[50], correo[50], contrasenya[50], telefono[20];
    int tipo;

    printf("\n=== REGISTRO DE USUARIO ===\n");

    printf("Nombre de usuario: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    printf("Correo electrónico: ");
    fgets(correo, sizeof(correo), stdin);
    correo[strcspn(correo, "\n")] = '\0';

    printf("Contraseña: ");
    fgets(contrasenya, sizeof(contrasenya), stdin);
    contrasenya[strcspn(contrasenya, "\n")] = '\0';

    printf("Telefono: ");
    fgets(telefono, sizeof(telefono), stdin);
    telefono[strcspn(telefono, "\n")] = '\0';

    printf("Tipo de usuario (1=Usuario normal, 2=Administrador): ");
    scanf("%d", &tipo);
    getchar();

    // Crear el nuevo usuario
    crearUsuario(&nuevo, 0, nombre, correo, contrasenya, telefono, tipo);

    // Añadir a la base de datos
    anyadirUsuario2(db, &nuevo);

    // Liberar memoria
    liberarUsuario(&nuevo);

    printf("\nUsuario registrado con exito!\n");

    // Guardar también en el archivo
    volcarBBDDAlFichero(FICHERO_DATOS, db);
}
