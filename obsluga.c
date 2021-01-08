#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funkcje.h"

#define DL_LINII 1024       /* Dlugosc buforow pomocniczych */
#define DL_WIERSZA 15       /* Dlugosc wiersza elementow w zapisanym obrazie */

int czytaj(FILE *plik_we, t_obraz *obraz) {
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j;

  /*Sprawdzenie czy podano prawidlowy uchwyt pliku */
  if (plik_we==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  }

  if (fgets(buf,DL_LINII,plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  /* Ustalenie formatu obrazu PGM/PPM - oznaczenie poczatkowe powinno byc P2/P3 */
  if (buf[0]=='P' && buf[1]=='2')
    obraz->typ_obr=2;

  else if (buf[0]=='P' && buf[1]=='3')
    obraz->typ_obr=3;

  else {
    fprintf(stderr,"Blad: To nie jest plik PGM/PPM\n");
    return(0);
  }

  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(plik_we))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,plik_we)==NULL)  /* Przeczytaj ja do bufora                */
	koniec=1;                   /* Zapamietaj ewentualny koniec danych */
    }  else {
      ungetc(znak,plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni odcieni */
  if (fscanf(plik_we,"%d %d %d",&(obraz->wymx),&(obraz->wymy),&(obraz->odcieni))!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni odcieni\n");
    return(0);
  }
  
  /* Dynamiczne zaalokowanie pamieci dla obrazu PGM */
  if (obraz->typ_obr==2) {
    obraz->obraz_pgm=(int**)malloc(obraz->wymy*sizeof(int*)); /* Alokacja miejsca na tablice wskaznikow do poszczegolnych wierszy i przypisanie wskanika do obraz_pgm */
    for (i=0;i<obraz->wymy;i++) {
      obraz->obraz_pgm[i]=(int*)malloc(obraz->wymx*sizeof(int)); /* Stworzenie wsaznikow na wiersze i alokacja miejsca na ich elementy */
    }
    for (i=0;i<obraz->wymy;i++) {   /* Zapisanie tablicy wartosciami pikseli obrazu PGM */
      for (j=0;j<obraz->wymx;j++) {
        if (fscanf(plik_we,"%d",&(obraz->obraz_pgm[i][j]))!=1) {
	        fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	        return(0);
       }
      }
    }
  }

  /* Dynamiczne zaalokowanie pamieci dla obrazu PPM */
  if (obraz->typ_obr==3) {
    obraz->czer=(int**)malloc(obraz->wymy*sizeof(int*));  /* Alokacja miejsca na tablice wskaznikow do poszczegolnych wierszy i przypisanie wskanika do czer, ziel i nieb */
    obraz->ziel=(int**)malloc(obraz->wymy*sizeof(int*));
    obraz->nieb=(int**)malloc(obraz->wymy*sizeof(int*));

    for (i=0;i<obraz->wymy;i++) {
      obraz->czer[i]=(int*)malloc(obraz->wymx*sizeof(int));   /* Stworzenie wsaznikow na wiersze i alokacja miejsca na ich elementy */
      obraz->ziel[i]=(int*)malloc(obraz->wymx*sizeof(int));
      obraz->nieb[i]=(int*)malloc(obraz->wymx*sizeof(int));
    }

    for (i=0;i<obraz->wymy;i++) {   /* Pobranie wartosci pikseli do poszczegolnych tablic */
      for (j=0;j<obraz->wymx;j++)
        fscanf(plik_we,"%d %d %d",&(obraz->czer[i][j]),&(obraz->ziel[i][j]),&(obraz->nieb[i][j]));
    }
  }  
  return obraz->wymx*obraz->wymy;   /* Czytanie zakonczone sukcesem    */
}                       /* Zwroc liczbe wczytanych pikseli */

void zapisz(FILE *plik_wy, t_obraz *obraz) {
  int i, j;

  if (obraz->typ_obr==2) {
    fprintf(plik_wy,"P2\n%d %d\n%d\n",obraz->wymx,obraz->wymy,obraz->odcieni);   /* Wpsianie do pliku PGM numeru magicznego, wymiarow i liczby odcieni */
    for (i=0;i<obraz->wymy;i++) {
      for (j=0;j<obraz->wymx;j++) {
        fprintf(plik_wy,"%d ",obraz->obraz_pgm[i][j]); /* Przepisanie tablicy pikseli do pliku wyjsciowego */
        if ((j+1)%DL_WIERSZA==0)
            fprintf(plik_wy,"\n"); /* Wprowadzenie znaku nowej linii, aby liczba elementow w wierszu nie przekroczyla ustalonej wartosci */ 
      }
      fprintf(plik_wy,"\n"); /* Zakonczenie kazdego wiersza znakiem nowej linii */
    }
  }
  else if (obraz->typ_obr==3) {
    fprintf(plik_wy,"P3\n%d %d\n%d\n",obraz->wymx,obraz->wymy,obraz->odcieni);   /* Wpsianie do pliku PPM numeru magicznego, wymiarow i liczby odcieni */
    for (i=0;i<obraz->wymy;i++) {
      for (j=0;j<obraz->wymx;j++) {
        fprintf(plik_wy,"%d %d %d ",obraz->czer[i][j],obraz->ziel[i][j],obraz->nieb[i][j]); /* Przepisanie tablicy pikseli do pliku wyjsciowego */
        if ((j+1)%DL_WIERSZA==0)
            fprintf(plik_wy,"\n"); /* Wprowadzenie znaku nowej linii, aby liczba elementow w wierszu nie przekroczyla ustalonej wartosci */ 
      }
      fprintf(plik_wy,"\n"); /* Zakonczenie kazdego wiersza znakiem nowej linii */
    }
  }
  
  /* Zwolnienie zaalokowanej pamieci dla obrazu PGM: */
  if (obraz->typ_obr==2) {
    for (i=0;i<obraz->wymy;i++)
      free(obraz->obraz_pgm[i]);
    free(obraz->obraz_pgm);
  }

  /* Zwolnienie zaalokowanej pamieci dla obrazu PPM: */
  if (obraz->typ_obr==3) {
    for (i=0;i<obraz->wymy;i++)
      free(obraz->czer[i]);
    free(obraz->czer);
    for (i=0;i<obraz->wymy;i++)
      free(obraz->ziel[i]);
    free(obraz->ziel);
    for (i=0;i<obraz->wymy;i++)
      free(obraz->nieb[i]);
    free(obraz->nieb);
  }
  
}

void wyswietl(char *n_pliku) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  //printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
}