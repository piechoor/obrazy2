#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funkcje.h"

/* Funkcja zmieniajaca kazdy element tablicy na jej negatyw. Funckja pobiera wskaznik do tablicy, jej wymiary i liczbe odcieni */
void negatyw(t_obraz *obraz) {
  int i, j;
  for (i=0;i<obraz->wymy;i++) {
    for (j=0;j<obraz->wymx;j++)
        obraz->obraz_pgm[i][j]=obraz->szarosci-obraz->obraz_pgm[i][j];  /* Przypisanie kazdemu elementowi liczby odcieni pomniejszonej o jego wartosc */
  }
}

/* Funkcja przeprowadza operacje progowania na tablicy, tzn. pobiera wartosc progu od uzytkownika i elementom mniejszym od progu
 przypisuje kolor czarny (0), a wiekszym bialy (obraz->szarosci). Funckja pobiera wskaznik do tablicy, jej wymiary i liczbe odcieni */
void progowanie(t_obraz *obraz) {
  int i, j;
  int prog; /* Zmienna przechowujaca wartosc progu */

  printf("Podaj wartosc progowania od 0 do %d: ", obraz->szarosci); /* Komunikat o poprawnym zakresie wartosci progu */
  scanf("%d", &prog); /* Pobranie od uzytkownika wartosci progu */

  if (prog>=0&&prog<=obraz->szarosci) { /* Jezeli prog miesci sie w prawidlowym przedziale wykonaj progowanie */
    for (i=0;i<obraz->wymy;i++) {
      for (j=0;j<obraz->wymx;j++) {
          if (obraz->obraz_pgm[i][j]<=prog)
            obraz->obraz_pgm[i][j]=0;
          else 
            obraz->obraz_pgm[i][j]=obraz->szarosci;
      }
    }
  }
  else
    printf("Podano nieprawidlowy prog.\n"); /* Inaczej wyswietl komunikat o bledzie */
}

/* Funkcja roznica jest funkcja pomocnicza do fuckji konturowania. Pobiera wartosc glowna i dwie wartosci wzgledne x1 i x2.
Zwraca wartosc |glowna-x1|+|glowna-x2| */
int roznica(int glowna, int x1, int x2) {
  if (glowna<x1)
    x1=x1-glowna;
  else x1=glowna-x1;

  if (glowna<x2)
    x2=x2-glowna;
  else x2=glowna-x2;
  return x1+x2;
}

/* Funkcja przeprowadza operacje konturowania na elementach tablicy. Do tego celu stworzone zostaja kopie przedostatniego wiersza i przedostatniej kolumny.
Konturowanie przeprowadzane jest w czterech obszarach: 
1. W calej tablicy bez ostatniej kolumny i wiersza. 2. W ostatniej kolumnie bez ostatiego elementu. 3. W ostatnim wierszu bez ostatniego elementu. 4. W ostatnim elemencie.
Pobiera ona wskaznik do tablicy, jej wymiary i liczbe odcieni */
void konturowanie(t_obraz *obraz) {
  int i,j;
  int kopiakolumny[obraz->wymy],kopiawiersza[obraz->wymx];

  for (i=0;i<obraz->wymy;i++) /* Stworzenie kopii przedostatniej kolumny */
    kopiakolumny[i]=obraz->obraz_pgm[i][obraz->wymx-2];
  for (i=0;i<obraz->wymx;i++) /* Stworzenie kopii przedostatniego wiersza */
    kopiawiersza[i]=obraz->obraz_pgm[obraz->wymy-2][i];

  /* Wyliczenie wartosci kolejnych elementow (pomijajac ostatni wiersz i ostatnia kolumne)
  jako polowe sumy roznic wartosci biezacego elementu oraz elementow z kolejnego wiersza i kolejnej kolumny */
  for (i=0;i<obraz->wymy-1;i++) { 
    for (j=0;j<obraz->wymx-1;j++)
      obraz->obraz_pgm[i][j]=(roznica(obraz->obraz_pgm[i][j],obraz->obraz_pgm[i+1][j],obraz->obraz_pgm[i][j+1])/2);
  }
  /* Wylicznie wartosci dla elementow ostatniej kolumny (bez jej ostatniego elementu)
  jako polowe sumy roznic wartosci biezacego elementu oraz elementu kolejnego wiersza i kopii elementu poprzedniej kolumny */
  for (i=0;i<obraz->wymy-1;i++)
    obraz->obraz_pgm[i][obraz->wymx-1]=(roznica(obraz->obraz_pgm[i][obraz->wymx-1],kopiakolumny[i],obraz->obraz_pgm[i+1][obraz->wymx-1])/2);

  /* Wylicznie wartosci dla elementow ostatniego wiersza (bez jego ostatniego elementu)
  jako polowe sumy roznic wartosci biezacego elementu oraz elementu kolejnej kolumny i kopii elementu poprzedniego wiersza */
  for (i=0;i<obraz->wymx-1;i++)
    obraz->obraz_pgm[obraz->wymy-1][i]=(roznica(obraz->obraz_pgm[obraz->wymy-1][i],kopiawiersza[i],obraz->obraz_pgm[obraz->wymy-1][i+1])/2);

  /* Wylicznie wartosci dla elementu ostatniego wiersza ostatniej kolumny
  jako polowe sumy roznic wartosci biezacego elementu oraz elementu poprzedniej kolumny i poprzedniego wiersza */
  obraz->obraz_pgm[obraz->wymy-1][obraz->wymx-1]=(roznica(obraz->obraz_pgm[obraz->wymy-1][obraz->wymx-1],kopiawiersza[obraz->wymx-1],kopiakolumny[obraz->wymy-1])/2);
}

/* Funkcja przeprowadza operacje polprogowania na elementach tablicy. Pobiera wskaznik do tablicy, jej wymiary i liczbe odcieni */
void polprogowanie(t_obraz *obraz) {
  int i, j;
  int zakresdolny, zakresgorny; /* Zmienne przechwoujace wartosci zakresow podanych przez uzytwkownika */
  printf("Podaj wartosc procentowa progowania do czerni: ");
  scanf("%d",&zakresdolny); /* Pobranie dolnego zakresu */
  printf("Podaj wartosc procentowa progowania do bieli: ");
  scanf("%d",&zakresgorny); /* Pobranie gornego zakresu */
  if(zakresdolny>zakresgorny||zakresgorny>obraz->szarosci)  /* Sprawdzenie poprawnosci podanych zakresow */
    printf("Podano nieprawidlowe wartosci.\n"); /* Jezeli sa nieprawidowe wyswietli sie komunikat o bledzie */
  else {
    zakresdolny=((obraz->szarosci*zakresdolny)/100);  /* Zamiana wartosci procentowych na wartosci liczbowe*/
    zakresgorny=((obraz->szarosci*zakresgorny)/100);
    for (i=0;i<obraz->wymy;i++) {
        for (j=0;j<obraz->wymx;j++) {
            if (obraz->obraz_pgm[i][j]<zakresdolny)  /* Jezeli dany element jest mniejszy od zakresu, zamieniamy go na wartosc czerni */
                obraz->obraz_pgm[i][j]=0;
            if (obraz->obraz_pgm[i][j]>zakresgorny)  /* Jezeli dany element jest wiekszy, zmieniamy na wartosc bieli */
                obraz->obraz_pgm[i][j]=obraz->szarosci;
        }
    }
  }
}