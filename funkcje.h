#ifndef FUNKCJE_H
#define FUNKCJE_H

/* Struktura obrazu */
typedef struct t_obraz {
    int wymx, wymy, odcieni, typ_obr;
    int **obraz_pgm;
    int **czer, **ziel, **nieb;
} t_obraz;

/* strukura do zapamietywania opcji podanych w wywolaniu programu */
typedef struct {
  FILE *plikwej, *plikwyj;        /* uchwyty do pliku wej. i wyj. */
  char *nazwa_wej, *nazwa_wyj;
  int negatyw,progowanie,konturowanie,gamma,wyswietlenie;      /* opcje */
  int prog;              /* wartosc progu dla opcji progowanie */ 
  int czer, ziel, nieb, szar;
} t_opcje;

int czytaj(FILE *plik_we, t_obraz *obraz);
void zapisz(FILE *plik_wy, t_obraz *obraz);

void wyswietl(char *n_pliku);

void negatyw(t_obraz *obraz, t_opcje *opcje);

int progowanie(t_obraz *obraz, t_opcje *opcje);

int roznica(int glowna, int x1, int x2);
void konturowanie(t_obraz *obraz);

void polprogowanie(t_obraz *obraz);

void wyzeruj_opcje(t_opcje *wybor);

int przetwarzaj_opcje(int argc, char **argv, t_opcje *wybor);


#endif