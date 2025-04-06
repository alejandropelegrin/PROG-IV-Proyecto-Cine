#ifndef ENTRADA_H
#define ENTRADA_H

#include "pelicula.h"
#include "sesion.h"
#include "sala.h"

#define MAX_ENTRADAS 1000

typedef struct {
    int id;
    int usuario_id;
    char nombre_pelicula[100];
    int sesion_id;
    char hora_sesion[10];
    int sala_id;
    int numero_asiento;
    float precio;
} Entrada;

typedef struct {
    Entrada entradas[MAX_ENTRADAS];
    int num_entradas;
} ListaEntradas;

void inicializarListaEntradas(ListaEntradas *lista);
void crearEntrada(Entrada *e, int id, int usuario_id, const char *nombre_pelicula,int sesion_id, const char *hora_sesion, int sala_id, int numero_asiento, float precio);
void imprimirEntrada(const Entrada *e);
void anyadirEntrada(ListaEntradas *lista, const Entrada *entrada);
void listarEntradasUsuario(const ListaEntradas *lista, int usuario_id);
int generarAsientoDisponible(const ListaEntradas *lista, int sala_id, int sesion_id, int total_asientos);

#endif