#ifndef PELICULA_H
#define PELICULA_H

typedef struct {
    int id;
    char titulo[100];
    int duracion; // en minutos
    char genero[50];
} Pelicula;

void crearPelicula(Pelicula *p, int id, const char *titulo, int duracion, const char *genero);
void imprimirPelicula(const Pelicula *p);
void anyadirPelicula(Pelicula peliculas[], int *numPeliculas, Pelicula nueva);
void eliminarPelicula(Pelicula peliculas[], int *numPeliculas, int id);
void modificarPelicula(Pelicula peliculas[], int numPeliculas, int id, const char *titulo, int duracion, const char *genero);

#endif
