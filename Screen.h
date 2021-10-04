#ifndef SCREEN_H
#define SCREEN_H
#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include "helpers.h"

// kolory
#define ODZN (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED) // | FOREGROUND_INTENSITY)
#define ZAZN (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)// | BACKGROUND_INTENSITY)
#define ODZN1 (BACKGROUND_BLUE | BACKGROUND_GREEN)
#define ODZN2 (BACKGROUND_BLUE | ODZN)

using namespace std;

class Screen {
	// nazwa pliku z plansza
	string nazwaplanszy = "";
	// pozycje na planszy
	COORD* pozycje[7] = {
	&boardpos ,
	&rotpos ,
	&menupos ,
	&playeropos ,
	&playerxpos ,
	&curplayerpos ,
	&menulast
	};
	// tekst wskazujacy na polozenie pozycji
	string znaczniki[7] = {
		 " . ",
		 " ,~>",
		"Nowa gra",
		 "O ~>",
		 "X ~>",
		 "Gra:",
		"ruch"
	};
	// o ile przesuniety wzgledem znacznikow jest poczatek obiektu
	int przesuniecia[7] = {
		0,
		0,
		-7,
		5,
		5,
		5,
		4
	};
	// ustawia COORD{-1,-1} dla wspolrzednych
	void coordunset();
	// sprawdza, czy w linii nie ma znacznikow, jak sa, to ustawia odpowiednio rozmiar
	void coordassign(string & line, SHORT row);
public:
	// uchwyty do konsoli
	HANDLE hout=nullptr;
	HANDLE hin = nullptr;
	// obszar, gdzie mozna wyswietlac komunikaty
	SMALL_RECT obszar = { 0,0,0,0 };
	// pozycje kluczowych elementow
	COORD boardpos = { 0,0 };
	COORD rotpos = { 0,0 };
	COORD menupos = { 0,0 };
	COORD playeropos = { 0,0 };
	COORD playerxpos = { 0,0 };
	COORD curplayerpos = { 0,0 };
	COORD commandpos = { 0,0 };
	COORD rightbottom = { 0,0 };
	COORD menulast = { 0,0 };



	Screen(string filestr = "board.txt");

	// czysci caly ekran
	void clearscreen();
	// pobiera znak
	char pobierz_znak(COORD wsp);
	// wypisuje plansze
	void board2screen();
	// czysci obszar
	void clearrect(SMALL_RECT wsp);
	// wpisuje tekst w linii o poczatku COORD wsp
	void insert_text(COORD wsp, string str);
	// wpisuje telst wematrz SMALL_RECT sr, wyrownany do prawej
	void insert_text(SMALL_RECT sr, string str);
	// wpisuje znak do konsoli
	void wpisz_znak(COORD wsp, char znak);
	// ustawia parametry konsoli
	void setconsoleprop();

};


#endif // !GAMESCREEN_H
