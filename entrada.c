#include "entrada.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void inicializarListaEntradas(ListaEntradas *lista) {
    lista->num_entradas = 0;
}

void crearEntrada(Entrada *e, int id, int usuario_id, const char *nombre_pelicula, 
                 int sesion_id, const char *hora_sesion, int sala_id, 
                 int numero_asiento, float precio) {
    e->id = id;
    e->usuario_id = usuario_id;
    strncpy(e->nombre_pelicula, nombre_pelicula, sizeof(e->nombre_pelicula));
    e->sesion_id = sesion_id;
    strncpy(e->hora_sesion, hora_sesion, sizeof(e->hora_sesion));
    e->sala_id = sala_id;
    e->numero_asiento = numero_asiento;
    e->precio = precio;
}

void imprimirEntrada(const Entrada *e) {
    printf("\n=== ENTRADA ===\n");
    printf("ID: %d\n", e->id);
    printf("Pelicula: %s\n", e->nombre_pelicula);
    printf("Sesion ID: %d | Hora: %s\n", e->sesion_id, e->hora_sesion);
    printf("Sala: %d | Asiento: %d\n", e->sala_id, e->numero_asiento);
    printf("Precio: %.2f€\n", e->precio);
    printf("================\n");
}

void anyadirEntrada(ListaEntradas *lista, const Entrada *entrada) {
    if (lista->num_entradas < MAX_ENTRADAS) {
        lista->entradas[lista->num_entradas] = *entrada;
        lista->num_entradas++;
    } else {
        printf("Error: No se pueden añadir mas entradas. Limite alcanzado.\n");
    }
}

void listarEntradasUsuario(const ListaEntradas *lista, int usuario_id) {
    printf("\n=== TUS ENTRADAS ===\n");
    int encontradas = 0;
    
    for (int i = 0; i < lista->num_entradas; i++) {
        if (lista->entradas[i].usuario_id == usuario_id) {
            imprimirEntrada(&lista->entradas[i]);
            encontradas++;
        }
    }
    
    if (encontradas == 0) {
        printf("No tienes entradas compradas.\n");
    }
}

int generarAsientoDisponible(const ListaEntradas *lista, int sala_id, int sesion_id, int total_asientos) {
    int asientos_ocupados[total_asientos + 1];
    memset(asientos_ocupados, 0, sizeof(asientos_ocupados));
    
    for (int i = 0; i < lista->num_entradas; i++) {
        if (lista->entradas[i].sesion_id == sesion_id) {
            if (lista->entradas[i].numero_asiento <= total_asientos) {
                asientos_ocupados[lista->entradas[i].numero_asiento] = 1;
            }
        }
    }
    
    int todos_ocupados = 1;
    for (int i = 1; i <= total_asientos; i++) {
        if (!asientos_ocupados[i]) {
            todos_ocupados = 0;
            break;
        }
    }
    
    if (todos_ocupados) {
        printf("Lo sentimos, no hay asientos disponibles para esta sesion.\n");
        return -1;
    }
    
    srand(time(NULL));
    int intentos = 0;
    const int MAX_INTENTOS = 100;
    
    while (intentos < MAX_INTENTOS) {
        int asiento = (rand() % total_asientos) + 1;
        
        if (!asientos_ocupados[asiento]) {
            return asiento;
        }
        
        intentos++;
    }
    
    for (int i = 1; i <= total_asientos; i++) {
        if (!asientos_ocupados[i]) {
            return i;
        }
    }
    
    return -1;
}