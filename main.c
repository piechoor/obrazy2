#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funkcje.h"
 
int main(int argc, char **argv) {
  t_obraz obraz;
  t_opcje opcje;
  int blad;

  blad=przetwarzaj_opcje(argc, argv, &opcje);   /* Jezeli funkcja zwroci blad wyswietl jego numer i zakoncz program */
  if (blad) {
    printf("Blad nr %d\n", blad);
    return blad;
  }

  if(czytaj(opcje.plikwej, &obraz)==0) {   /* Odczyt pliku, jezeli sie nie powiedzie zakoncz program */
    printf("Blad odczytu pliku\n");
    return 0;
  }
  fclose(opcje.plikwej);    /* Zamknij plik wejsciowy */

  if (opcje.szar==1&&obraz.typ_obr==3)    /* Jezeli wybrano konwerscje do szarosci i obraz jest typu PPM, skonwertuj go */
    konwersjaPGM(&obraz);

  if (opcje.negatyw==1) {   /* Wywolanie funkcji negatyw dla wybranych warstw */
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

  if (opcje.progowanie==1) {    /* Wywolanie funkcji progowanie dla wybranych warstw */
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
    else    /* Jezeli prog jest nieprawidlowy, wyswietl komunikat o bledzie */
      printf("Podano nieprawidlowa wartosc progu - progowanie nie powiodlo sie.\n");
  }

  if (opcje.konturowanie==1) {    /* Wywolanie funkcji konturowanie dla wybranych warstw */
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
  
  zapisz(opcje.plikwyj, &obraz);    /* Zapis pliku i jego zamkniecie */
  fclose(opcje.plikwyj);

  if (opcje.wyswietlenie==1) {    /* Wyswietl obraz, jezeli wybrano taka opcje */
    wyswietl(opcje.nazwa_wyj);
  }
  return 0;
}