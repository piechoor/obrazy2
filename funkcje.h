#ifndef FUNKCJE_H
#define FUNKCJE_H

#define MAX 1024

/* Struktura obrazu */
typedef struct t_obraz {
    int wymx, wymy, szarosci;
    int **obraz_pgm;
} t_obraz;

/* Struktura parametrow funkcji */

int czytaj(FILE *plik_we,int obraz_pgm[][MAX],int *wymx,int *wymy, int *szarosci);
void zapisz(FILE *plik_wy, int obraz_pgm[][MAX], int *wymx, int *wymy, int *szarosci, char *nazwa);

void wyswietl(char *n_pliku);

void negatyw(int obraz_pgm[][MAX], int *wymx, int *wymy, int *szarosci);

void progowanie(int obraz_pgm[][MAX], int *wymx, int *wymy, int *szarosci);

int roznica(int glowna, int x1, int x2);
void konturowanie(int obraz_pgm[][MAX], int *wymx, int *wymy, int *szarosci);

void polprogowanie(int obraz_pgm[][MAX], int *wymx, int *wymy, int *szarosci);


#endif