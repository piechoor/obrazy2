#include <stdio.h>
#include <stdlib.h>

#ifndef FUNKCJE_H
#define FUNKCJE_H

/* Struktura obrazu */
typedef struct t_obraz {
    int wymx, wymy, odcieni, typ_obr;   /* Informacje o obrazie: wymiary, liczba odcieni i typ */
    int **obraz_pgm;    /* Wskaznik na obraz pgm */
    int **czer, **ziel, **nieb;   /* Wskazniki na poszczegolne warstwy obrazu ppm */
} t_obraz;

/* Strukura opcji wywolania programu */
typedef struct {
  FILE *plikwej, *plikwyj;    /* Uchwyty do pliku wejsciowego i wyjsciowego */
  char *nazwa_wej, *nazwa_wyj;    /* Wskazniki na nazwy plikow */
  int negatyw, progowanie, konturowanie, wyswietlenie;      /* opcje */
  int prog;   /* Wartosc progu dla opcji progowanie */ 
  int czer, ziel, nieb, szar;   /* Flagi wyboru poszczegolnych warstw */
} t_opcje;


/* Modul obsluga.c */

/**************************************************************************************/
/* Funkcja wczytuje informacje o obrazie oraz sam obraz do struktury. W zaleznosci od */
/* formatu obrazu przepisuje jedna tablice pikseli (dla formatu PGM) albo trzy        */
/* poszczegolnych kolorow (dla formatu PPM). Funckja alokuje dynamicznie pamiec dla   */
/* potrzebnych tablic na piksele.                                                     */
/* PRE: plik_we i obraz musza wskazywac na konkretne obiekty                          */
/* POST: odpowiednie pola struktury obraz zostaja uzupenione                          */
/**************************************************************************************/
int czytaj(FILE *plik_we, t_obraz *obraz);

/**************************************************************************************/
/* Funkcja zapisuje zmodyfikowane tablice do pliku o podanej nazwie, a nastepnie      */
/* zwalnia miejsce zaalokowane dla potrzebnych tablic.                                */
/* PRE: plik_we i obraz musza wskazywac na konkretne obiekty                          */
/* POST: zostaje stworzony plik ze zmodyfikowanym obrazem                             */                       
/**************************************************************************************/
void zapisz(FILE *plik_wy, t_obraz *obraz);

/**************************************************************************************/
/* Funkcja wyswietla obraz za pomoca programu "display"                               */
/* PRE: n_pliku musi wskazywac na nazwe pliku                                         */
/* POST: zostaje wyswietlony obraz                                                    */                       
/**************************************************************************************/
void wyswietl(char *n_pliku);


/* Modul przetwarzanie.c */

/**************************************************************************************/
/* Funkcja zamienia podana warstwe na jej negatyw modyfikujac kazdy jej element       */
/* PRE: 'obraz' musi wskazywac na strukture, 'warstwa' musi wskazywac na tablice 2d   */
/* POST: kazdy element tablicy zostaje zamieniony na swoj negatyw                     */                       
/**************************************************************************************/
void negatyw(t_obraz *obraz, int **warstwa);

/**************************************************************************************/
/* Funkcja dokonuje operacji progowania na danej warstwie zgodnie z wczytanym progiem */
/* PRE: 'obraz' i 'wybor' musza wskazywac na strukture a 'warstwa' musi wskazywac na  */
/* tablice 2d, ponadto prog musi miescic sie w zakresie <0;100>                       */
/* POST: elementy tablicy zmieniane sa na wartosc 0, jezeli sa mniejsze badz rowne    */
/* od progu, albo na maksymalna mozliwa wartosc, jezeli sa od niego wieksze.          */                       
/**************************************************************************************/
void progowanie(t_obraz *obraz, t_opcje *wybor, int **warstwa);

/**************************************************************************************/
/* Funkcja przeprowadza operacje konturowania na elementach tablicy podanej warstwy.  */
/* Stworzone zostaja kopie przedostatniego wiersza i przedostatniej kolumny           */ 
/* warstwy. Konturowanie przeprowadzane jest w czterech obszarach:                    */
/* 1. W calej tablicy bez ostatniej kolumny i wiersza.                                */
/* 2. W ostatniej kolumnie bez ostatiego elementu.                                    */
/* 3. W ostatnim wierszu bez ostatniego elementu.                                     */
/* 4. Na ostatnim elemencie.                                                          */
/* PRE: 'obraz' musi wskazywac na strukture, 'warstwa' musi wskazywac na tablice 2d   */
/* POST: warstwa zostaje zkonturowana                                                 */   
/**************************************************************************************/
void konturowanie(t_obraz *obraz, int **warstwa);

/**************************************************************************************/
/* Funckja przeprowadza konwersje obrazu PGM na PPM                                   */
/* PRE: 'obraz' musi wskazywac na strukture                                           */
/* POST: typ obrazu zostaje zmieniony na PGM, zostaje zwolniona pamiec obrazu PPM     */
/* i zaalokowana pamiec na obraz PGM. Kazdy nowy piksel obrazu PGM to srednia         */
/* arytmetyczna odpowiadajacych pikseli obrazu PPM                                    */
/**************************************************************************************/
void konwersjaPGM(t_obraz *obraz);


/* Modul opcje.c */

/******************************************************************/
/* Funkcja inicjuje strukture wskazywana przez wskaznik 'wybor'   */
/* PRE: Poprawnie zainicjowany argument 'wybor' (tzn. !=NULL)     */
/* POST: "Wyzerowana" struktura wybor wybranych opcji             */
/******************************************************************/
void wyzeruj_opcje(t_opcje *wybor);

/*********************************************************************************************************/
/* Funkcja rozpoznaje opcje wywolania programu zapisane w tablicy argv i zapisuje je w strukturze wybor  */
/* Skladnia opcji wywolania programu:                                                                    */
/* program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-d] [-k] [-m (-[r][g][b])|(-s)]}                     */
/* Argumenty funkcji:                                                                                    */
/*         argc  -  liczba argumentow wywolania wraz z nazwa programu                                    */
/*         argv  -  tablica argumentow wywolania                                                         */
/*         wybor -  struktura z informacjami o wywolanych opcjach                                        */
/* PRE: Brak                                                                                             */
/* POST: Funkcja otwiera odpowiednie pliki, zwraca uchwyty do nich w strukturze wybór, do tego ustawia   */
/* na 1 pola dla opcji, ktore poprawnie wystapily w linii wywolania programu, pola opcji                 */
/* nie wystepujacych w wywolaniu ustawione sa na 0; zwraca wartosc W_OK (0), gdy wywolanie bylo poprawne */
/* lub kod bledu zdefiniowany stalymi B_* (<0)                                                           */
/* UWAGA: Funkcja nie sprawdza istnienia i praw dostepu do plikow, w takich przypadkach zwracane uchwyty */
/* maja wartosc NULL.                                                                                    */
/*********************************************************************************************************/
int przetwarzaj_opcje(int argc, char **argv, t_opcje *wybor);

#endif