#ifndef FUNKCJE_H
#define FUNKCJE_H

/* Struktura obrazu */
typedef struct t_obraz {
    int wymx, wymy, szarosci;
    int **obraz_pgm;
} t_obraz;

/* Struktura parametrow funkcji */

int czytaj(FILE *plik_we, t_obraz *obraz);
void zapisz(FILE *plik_wy, t_obraz *obraz, char *nazwa);

void wyswietl(char *n_pliku);

void negatyw(t_obraz *obraz);

void progowanie(t_obraz *obraz);

int roznica(int glowna, int x1, int x2);
void konturowanie(t_obraz *obraz);

void polprogowanie(t_obraz *obraz);


#endif