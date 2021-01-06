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

  if (opcje.progowanie==1) {
    if (opcje.prog>=0&&opcje.prog<=100)
      progowanie(&obraz, &opcje);
    else 
      printf("Podano nieprawidlowa wartosc progu - progowanie nie powiodlo sie.\n");
  }

  if (opcje.konturowanie==1) {
    konturowanie(&obraz, &opcje);
  }
  
  zapisz(opcje.plikwyj, &obraz);
  fclose(opcje.plikwyj);

  if (opcje.wyswietlenie==1) {
    wyswietl(opcje.nazwa_wyj);
  }
  return 0;
}