#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funkcje.h"

int main() {
  t_obraz obraz;
  int odczytano = 0;  /* Zmienna przechowujaca liczbe odczytanych pikseli */
  FILE *plikwej, *plikwyj; /* Uchwyty do pliku wejsciowego i wyjsciowego */
  char nazwa_wej[100], nazwa_wyj[100]; /* Tablice z nazwami plikow */

  /* Wczytanie zawartosci wskazanego pliku do pamieci */
  printf("Podaj nazwe wejsciowego obrazu:\n");
  scanf("%s",nazwa_wej);

  plikwej=fopen(nazwa_wej,"r");

  if (plikwej != NULL) {       /* co spowoduje zakomentowanie tego warunku */
    odczytano = czytaj(plikwej,&obraz);
    if (odczytano==0) {
      printf("Blad odczytu pliku\n");  /* Jezeli funkcja zwroci 0 wyswietl komunikat o bledzie */
      return 0; /* Zakoncz program */
    }
    fclose(plikwej);
  }
  else {
    printf("Błąd odczytu pliku.\n");
    return 1;
  }

  /* Wczytanie nazwy pliku wyjsciowego */
  printf("Podaj nazwe wyjsciowego obrazu:\n");
  scanf("%s",nazwa_wyj);
  
  /* Wyswietlenie menu */
  printf("**********************************************************\n");
  printf("* Wczytany obraz: \"%s\". Wybierz operacje z listy:\n",nazwa_wej);
  printf("* 1.Negatyw.\n* 2.Progowanie.\n* 3.Konturowanie\n* 4.Polprogowanie\n* 5.Wyswietl wyjsciowy obraz.\n* 6.Zakoncz program\n");
  printf("**********************************************************\n");

  int wybor;  /* Stworzenie zmiennej wyboru */
  do {
    printf("Wybor operacji: ");
    scanf("%d", &wybor);  /* Wczytanie wyboru */
    switch (wybor)
    {
        case 1:
            negatyw(&obraz);
            zapisz(plikwyj,&obraz,nazwa_wyj);  
            break;
        case 2:
            progowanie(&obraz);
            zapisz(plikwyj,&obraz,nazwa_wyj);  
            break;
        case 3:
            konturowanie(&obraz);
            zapisz(plikwyj,&obraz,nazwa_wyj);  
            break;
        case 4:
            polprogowanie(&obraz);
            zapisz(plikwyj,&obraz,nazwa_wyj);
            break;
        case 5:
            wyswietl(nazwa_wyj);
            break;
        case 6:
            printf("Koniec pracy programu\n");
            break;
        default:
            printf("Brak takiej pozycji w menu.\n");  
            break;
    }
  } while(wybor!=6); /* Kontynuuj petle az do wybrania 6 */

  /* Komunikat o zapisie pliku */
  printf("Przetworzony obraz zostal zapisany jako \"%s\".\n",nazwa_wyj);

  return odczytano;
}