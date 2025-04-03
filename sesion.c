#include "sesion.h"
#include <stdio.h>
#include <string.h>

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

void anyadirSesion(Sesion sesiones[], int *numSesiones, Sesion nueva) {
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
