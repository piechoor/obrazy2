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

  if (opcje.szar==1)
    konwersjaPGM(&obraz,&opcje);

  if (opcje.negatyw==1) {
    if (obraz.typ_obr==2)
      negatyw(&obraz, obraz.obraz_pgm);
    else if (obraz.typ_obr==3) {
        if (opcje.czer==1)
          negatyw(&obraz,obraz.czer);
        if (opcje.ziel==1)
          negatyw(&obraz,obraz.ziel);
        if (opcje.nieb==1)
          negatyw(&obraz,obraz.nieb);
    }
  }

  if (opcje.progowanie==1) {
    if (opcje.prog>=0&&opcje.prog<=100) {
      if (obraz.typ_obr==2)
        progowanie(&obraz,&opcje,obraz.obraz_pgm);
      else if (obraz.typ_obr==3) {
          if (opcje.czer==1)
            progowanie(&obraz,&opcje,obraz.czer);
          if (opcje.ziel==1)
            progowanie(&obraz,&opcje,obraz.ziel);
          if (opcje.nieb==1)
            progowanie(&obraz,&opcje,obraz.nieb);
      }
    }
    else 
      printf("Podano nieprawidlowa wartosc progu - progowanie nie powiodlo sie.\n");
  }

  if (opcje.konturowanie==1) {
    if (obraz.typ_obr==2)
      konturowanie(&obraz,obraz.obraz_pgm);
    else if (obraz.typ_obr==3) {
        if (opcje.czer==1)
          konturowanie(&obraz,obraz.czer);
        if (opcje.ziel==1)
          konturowanie(&obraz,obraz.ziel);
        if (opcje.nieb==1)
          konturowanie(&obraz,obraz.nieb);
    }
  }
  
  zapisz(opcje.plikwyj, &obraz);
  fclose(opcje.plikwyj);

  if (opcje.wyswietlenie==1) {
    wyswietl(opcje.nazwa_wyj);
  }
  return 0;
}