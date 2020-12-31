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

  /* Sprawdzenie "numeru magicznego" - powinien byc P2 */
  if (fgets(buf,DL_LINII,plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || (buf[1]!='2') || koniec) {  /* Czy jest magiczne "P2"? */
    fprintf(stderr,"Blad: To nie jest plik PGM\n");
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
}

/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */
void wyswietl(char *n_pliku) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  //printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
}

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