#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funkcje.h"
 
int main(int argc, char **argv) {
  t_obraz obraz;
  t_opcje opcje;
  int kod_bledu;
  
  int testy;

  kod_bledu=przetwarzaj_opcje(argc, argv, &opcje);
  if (kod_bledu) {
    printf("Blad nr %d\n", kod_bledu);
    return kod_bledu;
  }
  testy=czytaj(opcje.plikwej, &obraz);
  printf("Wczytano %d pikseli\n",testy);
  fclose(opcje.plikwej);

  if (opcje.negatyw==1)
    negatyw(&obraz, &opcje);

  zapisz(opcje.plikwyj, &obraz);
  fclose(opcje.plikwyj);

  if (opcje.wyswietlenie==1) {
    wyswietl(opcje.nazwa_wyj);
  }
  return 0;
}