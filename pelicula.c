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

void anyadirPelicula(Pelicula peliculas[], int *numPeliculas, Pelicula nueva) {
    peliculas[*numPeliculas] = nueva;
    (*numPeliculas)++;
}

void eliminarPelicula(Pelicula peliculas[], int *numPeliculas, int id) {
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
