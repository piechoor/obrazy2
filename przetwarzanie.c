#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funkcje.h"

/* Funkcja zmieniajaca kazdy element tablicy na jej negatyw. Funckja pobiera wskaznik do tablicy, jej wymiary i liczbe odcieni */
void negatyw(t_obraz *obraz, int **warstwa) {
  int i, j;
  for (i=0;i<obraz->wymy;i++) {
    for (j=0;j<obraz->wymx;j++)
        warstwa[i][j]=obraz->odcieni-warstwa[i][j];  /* Przypisanie kazdemu elementowi liczby odcieni pomniejszonej o jego wartosc */
  }
}

/* Funkcja przeprowadza operacje progowania na tablicy, tzn. pobiera wartosc progu od uzytkownika i elementom mniejszym od progu
 przypisuje kolor czarny (0), a wiekszym bialy (obraz->odcieni). Funckja pobiera wskaznik do tablicy, jej wymiary i liczbe odcieni */
void progowanie(t_obraz *obraz, t_opcje *wybor, int **warstwa) {
  int i, j;
  int w_progu;

  /*if (wybor->prog>100||wybor->prog<0) {
    printf("Podano nieprawidlowa wartosc progu.\n");
    return 0;
  }*/
  w_progu = (wybor->prog*obraz->odcieni)/100;
  
  for (i=0;i<obraz->wymy;i++) {
    for (j=0;j<obraz->wymx;j++) {
      if (warstwa[i][j]<=w_progu)
        warstwa[i][j]=0;
      else 
        warstwa[i][j]=w_progu;
    }
  }
}

/* Funkcja przeprowadza operacje konturowania na elementach tablicy. Do tego celu stworzone zostaja kopie przedostatniego wiersza i przedostatniej kolumny.
Konturowanie przeprowadzane jest w czterech obszarach: 
1. W calej tablicy bez ostatniej kolumny i wiersza. 2. W ostatniej kolumnie bez ostatiego elementu. 3. W ostatnim wierszu bez ostatniego elementu. 4. W ostatnim elemencie.
Pobiera ona wskaznik do tablicy, jej wymiary i liczbe odcieni */
void konturowanie(t_obraz *obraz, int **warstwa) {
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

void konwersjaPGM(t_obraz *obraz, t_opcje *opcje) {
  int i,j;

  /* Dynamiczne zaalokowanie pamieci dla obrazu PGM */
  obraz->obraz_pgm=(int**)malloc(obraz->wymy*sizeof(int*)); /* Alokacja miejsca na tablice wskaznikow do poszczegolnych wierszy i przypisanie wskanika do obraz_pgm */
  for (i=0;i<obraz->wymy;i++) {
    obraz->obraz_pgm[i]=(int*)malloc(obraz->wymx*sizeof(int)); /* Stworzenie wsaznikow na wiersze i alokacja miejsca na ich elementy */
  }

  /* Konwersja PPM do PGM */
  for (i=0;i<obraz->wymy;i++) {
    for (j=0;j<obraz->wymx;j++)
      obraz->obraz_pgm[i][j]=((obraz->czer[i][j]+obraz->ziel[i][j]+obraz->nieb[i][j])/3);
  }

  /* Zwolnienie pamieci obrazu PPM */
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