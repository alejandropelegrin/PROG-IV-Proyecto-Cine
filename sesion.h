#ifndef SESION_H
#define SESION_H

typedef struct {
    int id;
    int pelicula_id;
    int sala_id;
    char hora_inicio[10];
    char hora_fin[10];
} Sesion;

void crearSesion(Sesion *s, int id, int pelicula_id, int sala_id, const char *hora_inicio, const char *hora_fin);
void imprimirSesion(const Sesion *s);
void anyadirSesion(Sesion sesiones[], int *numSesiones, Sesion nueva);
void eliminarSesion(Sesion sesiones[], int *numSesiones, int id);
void modificarSesion(Sesion sesiones[], int numSesiones, int id, const char *hora_inicio, const char *hora_fin);

#endif
