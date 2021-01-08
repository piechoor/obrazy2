#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funkcje.h"

void negatyw(t_obraz *obraz, int **warstwa) {
  if (obraz==NULL||warstwa==NULL)
    printf("Blad asercji: Procedura negatyw.\n");
  else {
    int i, j;
    for (i=0;i<obraz->wymy;i++) {
      for (j=0;j<obraz->wymx;j++)
          warstwa[i][j]=obraz->odcieni-warstwa[i][j];  /* Przypisanie kazdemu elementowi liczby odcieni pomniejszonej o jego wartosc */
    }
  }
}

void progowanie(t_obraz *obraz, t_opcje *wybor, int **warstwa) {
  if (obraz==NULL||wybor==NULL||warstwa==NULL)
    printf("Blad asercji: Procedura progowanie.\n");
  else {
    int i, j;
    int w_progu;

    w_progu = (wybor->prog*obraz->odcieni)/100;   /* zamiana progu z wartosci procentowej na konkretna wartosc */
    for (i=0;i<obraz->wymy;i++) {
      for (j=0;j<obraz->wymx;j++) {
        if (warstwa[i][j]<=w_progu)   /* jezeli element jest mniejszy-rowny wartosci progu ustawiamy go na zero */
          warstwa[i][j]=0;
        else 
          warstwa[i][j]=obraz->odcieni;   /* w przeciwnym wypadku na wartosc liczby odcieni */
      }
    }
  }
}

void konturowanie(t_obraz *obraz, int **warstwa) {
  if (obraz==NULL||warstwa==NULL)
    printf("Blad asercji: Procedura konturowanie.\n");
  else {
    int i,j;
    int kopiakolumny[obraz->wymy],kopiawiersza[obraz->wymx];

    for (i=0;i<obraz->wymy;i++) /* Stworzenie kopii przedostatniej kolumny */
      kopiakolumny[i]=warstwa[i][obraz->wymx-2];
    for (i=0;i<obraz->wymx;i++) /* Stworzenie kopii przedostatniego wiersza */
      kopiawiersza[i]=warstwa[obraz->wymy-2][i];

    /* Wyliczenie wartosci kolejnych elementow (pomijajac ostatni wiersz i ostatnia kolumne)
    jako polowe sumy roznic wartosci biezacego elementu oraz elementow z kolejnego wiersza i kolejnej kolumny */
    for (i=0;i<obraz->wymy-1;i++) { 
      for (j=0;j<obraz->wymx-1;j++)
        warstwa[i][j]=(abs(warstwa[i][j]-warstwa[i+1][j])+abs(warstwa[i][j]-warstwa[i][j+1])/2);
    }
    /* Wylicznie wartosci dla elementow ostatniej kolumny (bez jej ostatniego elementu)
    jako polowe sumy roznic wartosci biezacego elementu oraz elementu kolejnego wiersza i kopii elementu poprzedniej kolumny */
    for (i=0;i<obraz->wymy-1;i++)
      warstwa[i][obraz->wymx-1]=(abs(warstwa[i][obraz->wymx-1]-kopiakolumny[i])+abs(warstwa[i][obraz->wymx-1]-warstwa[i+1][obraz->wymx-1])/2);

    /* Wylicznie wartosci dla elementow ostatniego wiersza (bez jego ostatniego elementu)
    jako polowe sumy roznic wartosci biezacego elementu oraz elementu kolejnej kolumny i kopii elementu poprzedniego wiersza */
    for (i=0;i<obraz->wymx-1;i++)
      warstwa[obraz->wymy-1][i]=(abs(warstwa[obraz->wymy-1][i]-kopiawiersza[i])+abs(warstwa[obraz->wymy-1][i]-warstwa[obraz->wymy-1][i+1])/2);

    /* Wylicznie wartosci dla elementu ostatniego wiersza ostatniej kolumny
    jako polowe sumy roznic wartosci biezacego elementu oraz elementu poprzedniej kolumny i poprzedniego wiersza */
    warstwa[obraz->wymy-1][obraz->wymx-1]=(abs(warstwa[obraz->wymy-1][obraz->wymx-1]-kopiawiersza[obraz->wymx-1])+abs(warstwa[obraz->wymy-1][obraz->wymx-1]-kopiakolumny[obraz->wymy-1])/2);
  }
}

void konwersjaPGM(t_obraz *obraz) {
  if (obraz==NULL)
    printf("Blad asercji: Procedura konwerscjaPGM.\n");
  else {  
    int i,j;

    /* Dynamiczne zaalokowanie pamieci dla obrazu PGM */
    obraz->obraz_pgm=(int**)malloc(obraz->wymy*sizeof(int*)); /* Alokacja miejsca na tablice wskaznikow do poszczegolnych wierszy i przypisanie wskanika do obraz_pgm */
    for (i=0;i<obraz->wymy;i++) {
      obraz->obraz_pgm[i]=(int*)malloc(obraz->wymx*sizeof(int)); /* Stworzenie wsaznikow na wiersze i alokacja miejsca na ich elementy */
    }

    /* Konwersja PPM do PGM */
    for (i=0;i<obraz->wymy;i++) {
      for (j=0;j<obraz->wymx;j++)
        obraz->obraz_pgm[i][j]=((obraz->czer[i][j]+obraz->ziel[i][j]+obraz->nieb[i][j])/3);   /* obliczenie wartosci kazdego elementu tablicy */
    }

    /* Zwolnienie pamieci obrazu PPM zwalniajac pamiec poszczegolnych wierszy a nastepnie calej tablicy dla wszystkich warstw */
    for (i=0;i<obraz->wymy;i++)
      free(obraz->czer[i]);
    free(obraz->czer);
    for (i=0;i<obraz->wymy;i++)
      free(obraz->ziel[i]);
    free(obraz->ziel);
    for (i=0;i<obraz->wymy;i++)
      free(obraz->nieb[i]);
    free(obraz->nieb);

    /* Zmiana typu na obraz PGM */
    obraz->typ_obr=2;
  }
}