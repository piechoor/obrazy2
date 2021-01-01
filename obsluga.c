#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funkcje.h"

#define DL_LINII 1024       /* Dlugosc buforow pomocniczych */
#define DL_WIERSZA 65       /* Dlugosc wiersza elementow w zapisanym obrazie */

/************************************************************************************
 * Funkcja wczytuje obraz PGM z pliku do tablicy       	       	       	       	    *
 *										    *
 * \param[in] plik_we uchwyt do pliku z obrazem wejsciowym w formacie PGM			    *
 * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany obraz		    *
 * \param[out] wymx szerokosc obrazka						    *
 * \param[out] wymy wysokosc obrazka						    *
 * \param[out] szarosci liczba odcieni szarosci					    *
 * \return liczba wczytanych pikseli						    *
 ************************************************************************************/

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

  /* Sprawdzenie "numeru magicznego" dla pliku PGM/PPM - powinien byc P2/P3 */
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

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(plik_we,"%d %d %d",&(obraz->wymx),&(obraz->wymy),&(obraz->szarosci))!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }
  
  /* Dynamiczne zaalokowanie pamieci dla obrazu PGM */
  if (obraz->typ_obr==2) {
    obraz->obraz_pgm=(int**)malloc(obraz->wymy*sizeof(int*)); /* Alokacja miejsca na tablice wskaznikow do poszczegolnych wierszy i przypisanie wskanika do obraz_pgm */
    for (i=0;i<obraz->wymy;i++) {
      obraz->obraz_pgm[i]=(int*)malloc(obraz->wymx*sizeof(int)); /* Stworzenie wsaznikow na wiersze i alokacja miejsca na ich elementy */
    }
    for (i=0;i<obraz->wymy;i++) {
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
    obraz->obraz_pgm=(int**)malloc(obraz->wymy*sizeof(int*)); /* Alokacja miejsca na tablice wskaznikow do poszczegolnych wierszy i przypisanie wskanika do obraz_pgm */
    for (i=0;i<obraz->wymy;i++) {
      obraz->obraz_pgm[i]=(int*)malloc(obraz->wymx*sizeof(int)); /* Stworzenie wsaznikow na wiersze i alokacja miejsca na ich elementy */
    }
    for (i=0;i<obraz->wymy;i++) {
      for (j=0;j<obraz->wymx;j++) {
        if (fscanf(plik_we,"%d",&(obraz->obraz_pgm[i][j]))!=1) {
	        fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	        return(0);
       }
      }
    }
  }  
  return obraz->wymx*obraz->wymy;   /* Czytanie zakonczone sukcesem    */
}                       /* Zwroc liczbe wczytanych pikseli */

/************************************************************************************
 * Funkcja zapisuje obraz PGM z pliku do tablicy       	       	       	       	    *
 *										    *
 * \param[in] plik_wy uchwyt do pliku z obrazem wyjsciowym w formacie PGM			    *
 * \param[in] obraz_pgm tablica, do ktorej zostanie zapisany obraz		    *
 * \param[in] wymx szerokosc obrazka						    *
 * \param[in] wymy wysokosc obrazka						    *
 * \param[in] szarosci liczba odcieni szarosci					    *
 ************************************************************************************/
void zapisz(FILE *plik_wy, t_obraz *obraz, char *nazwa) {
  int i, j;
  plik_wy=fopen(nazwa,"w");   /* Otworzenie pliku w celu zapisu */
  fprintf(plik_wy,"P2\n%d %d\n%d\n",obraz->wymx,obraz->wymy,obraz->szarosci);   /* Wpsianie do pliku numeru magicznego, wymiarow i liczby odcieni */

  for (i=0;i<obraz->wymy;i++) {
    for (j=0;j<obraz->wymx;j++) {
        fprintf(plik_wy,"%d ",obraz->obraz_pgm[i][j]); /* Przepisanie tablicy pikseli do pliku wyjsciowego */
        if ((j+1)%DL_WIERSZA==0)
            fprintf(plik_wy,"\n"); /* Wprowadzenie znaku nowej linii, aby liczba elementow w wierszu nie przekroczyla ustalonej wartosci */ 
    }
    fprintf(plik_wy,"\n"); /* Zakonczenie kazdego wiersza znakiem nowej linii */
  }
  fclose(plik_wy); /* Zamkniecie pliku */
  
  /* Zwolnienie zaalokowanej pamieci */
  for (i=0;i<obraz->wymy;i++)
    free(obraz->obraz_pgm[i]);
  free(obraz->obraz_pgm);
  
}
 
/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display" */
void wyswietl(char *n_pliku) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  //printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
}