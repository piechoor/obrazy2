/********************************************************************/
/*                                                                  */
/* ALTERNATYWNA DO PRZEDSTAWIONEJ NA WYKLADZIE WERSJA OPRACOWANIA   */
/* PARAMETROW WYWOLANIA PROGRAMU UWZGLEDNIAJACA OPCJE Z PARAMETRAMI */
/* Z ODPOWIEDNIO ZAPROPONOWANYMI STRUKTURAMI DANYCH PRZEKAZUJACYMI  */
/* WCZYTANE USTAWIENIA                                              */
/*                                    COPYRIGHT (c) 2007-2020 KCiR  */
/*                                                                  */
/* Autorzy udzielaja kazdemu prawa do kopiowania tego programu      */
/* w calosci lub czesci i wykorzystania go w dowolnym celu, pod     */
/* warunkiem zacytowania zrodla                                     */
/*                                                                  */
/********************************************************************/

#include <stdio.h>
#include <string.h>
#include "funkcje.h"
#define W_OK 0                   /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNA_OPCJA -1    /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY   -2
#define B_BRAKWARTOSCI  -3
#define B_BRAKPLIKU   -4
#define B_NIEPOPRAWNA_WARSTWA -5

/*******************************************************/
/* Funkcja inicjuje strukture wskazywana przez wybor   */
/* PRE:                                                */
/*      poprawnie zainicjowany argument wybor (!=NULL) */
/* POST:                                               */
/*      "wyzerowana" struktura wybor wybranych opcji   */
/*******************************************************/

void wyzeruj_opcje(t_opcje * wybor) {
  wybor->plikwej=NULL;
  wybor->plikwyj=NULL;
  wybor->negatyw=0;
  wybor->konturowanie=0;
  wybor->progowanie=0;
  wybor->gamma=0;
  wybor->wyswietlenie=0;
  wybor->czer=0;
  wybor->ziel=0;
  wybor->nieb=0;
  wybor->szar=0;
}

/************************************************************************/
/* Funkcja rozpoznaje opcje wywolania programu zapisane w tablicy argv  */
/* i zapisuje je w strukturze wybor                                     */
/* Skladnia opcji wywolania programu                                    */
/*         program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-r] [-d] }  */
/* Argumenty funkcji:                                                   */
/*         argc  -  liczba argumentow wywolania wraz z nazwa programu   */
/*         argv  -  tablica argumentow wywolania                        */
/*         wybor -  struktura z informacjami o wywolanych opcjach       */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      funkcja otwiera odpowiednie pliki, zwraca uchwyty do nich       */
/*      w strukturze wybór, do tego ustawia na 1 pola dla opcji, ktore  */
/*	poprawnie wystapily w linii wywolania programu,                 */
/*	pola opcji nie wystepujacych w wywolaniu ustawione sa na 0;     */
/*	zwraca wartosc W_OK (0), gdy wywolanie bylo poprawne            */
/*	lub kod bledu zdefiniowany stalymi B_* (<0)                     */
/* UWAGA:                                                               */
/*      funkcja nie sprawdza istnienia i praw dostepu do plikow         */
/*      w takich przypadkach zwracane uchwyty maja wartosc NULL         */
/************************************************************************/

int przetwarzaj_opcje(int argc, char **argv, t_opcje *wybor) {
  int i, j, w_progu;

  wyzeruj_opcje(wybor);
  wybor->plikwyj=stdout;        /* na wypadek gdy nie podano opcji "-o" */

  for (i=1; i<argc; i++) {
    if (argv[i][0] != '-')  /* blad: to nie jest opcja - brak znaku "-" */
        return B_NIEPOPRAWNA_OPCJA;
      
    switch (argv[i][1]) {
      case 'i': {                 /* opcja z nazwa pliku wejsciowego */
        if (++i<argc) {   /* wczytujemy kolejny argument jako nazwe pliku */
	        wybor->nazwa_wej=argv[i];
	        if (strcmp(wybor->nazwa_wej,"-")==0) /* gdy nazwa jest "-"        */
	          wybor->plikwej=stdin;            /* ustwiamy wejscie na stdin */
	        else                               /* otwieramy wskazany plik   */
	          wybor->plikwej=fopen(wybor->nazwa_wej,"r");
        } 
        else 
	        return B_BRAKNAZWY;                   /* blad: brak nazwy pliku */
        break;
      }
      case 'o': {                 /* opcja z nazwa pliku wyjsciowego */
        if (++i<argc) {   /* wczytujemy kolejny argument jako nazwe pliku */
	        wybor->nazwa_wyj=argv[i];
	        if (strcmp(wybor->nazwa_wyj,"-")==0)/* gdy nazwa jest "-"         */
	          wybor->plikwyj=stdout;          /* ustwiamy wyjscie na stdout */
	        else                              /* otwieramy wskazany plik    */
	          wybor->plikwyj=fopen(wybor->nazwa_wyj,"w");
        } 
        else 
	        return B_BRAKNAZWY;                   /* blad: brak nazwy pliku */
        break;
      }
      case 'p': {
        if (++i<argc) { /* wczytujemy kolejny argument jako wartosc progu */
	        if (sscanf(argv[i],"%d",&w_progu)==1) {
	          wybor->progowanie=1;
	          wybor->prog=w_progu;
	        } 
        else
	        return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
        } 
        else 
	      return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
        break;
      }
      case 'm': {
        if (++i<argc) {
          for (j=0; argv[i][j]!='\0'; j++) {
            if (argv[i][j]=='r')
              wybor->czer=1;
            else if (argv[i][j]=='g')
              wybor->ziel=1;
            else if (argv[i][j]=='b')
              wybor->nieb=1;
            else if (argv[i][j]=='s')
              wybor->szar=1;
            else
              return B_NIEPOPRAWNA_WARSTWA;
          }
        }
        else
          return B_BRAKWARTOSCI;
        break;
      }
      case 'n': {                 /* mamy wykonac negatyw */
        wybor->negatyw=1;
        break;
      }
      case 'k': {                 /* mamy wykonac konturowanie */
        wybor->konturowanie=1;
        break;
      }
      case 'd': {                 /* mamy wyswietlic obraz */
        wybor->wyswietlenie=1;
        break;
      }
      case 'g': {
        wybor->gamma=1;
        break;
      }
      default:                    /* nierozpoznana opcja */
        return B_NIEPOPRAWNA_OPCJA;
      } /* koniec switch */
  } /* koniec for */

  if (wybor->plikwej!=NULL)     /* ok: wej. strumien danych zainicjowany */
    return W_OK;
  else 
    return B_BRAKPLIKU;         /* blad:  nie otwarto pliku wejsciowego  */
}
