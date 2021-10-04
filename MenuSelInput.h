#ifndef MENUSELINPUT_H
#define MENUSELINPUT_H
#include "MenuSel.h"

// menu z kilkoma opcjami, pole do wpisywania tekstu
class MenuSelInput : public MenuSel {
	// pozycje z menu
	Lista2kier<string> pozycje;
	// wspolrzedne kursora w polu tekstowym
	COORD entry_coord;
	// wspolrzedne, gdzie wypisujemy komunikaty
	SMALL_RECT komunikat_coord;
public:
	// czy potrzebne pole do wpisywania tekstu
	bool entry=false;
	// tytul menu
	string tytul;
	// komunikat
	string komunikat;
	// bufor do zczytywania danych wejsciowych
	char buffer[100];

	MenuSelInput(COORD wym = { 3,1 }, COORD offs = { 0,0 }, Screen* s=nullptr) :
		MenuSel(wym, offs,s), pozycje(Lista2kier<string>()), entry(false) {}

	// ustala i wpisuje offset, entry_coord i komunikat_coord na podstawie innych pol klasy
	MenuSelInput& setpos();
	// wyswietla tytul, menu, zostawia pole na wprowadzanie tekstu i komunikat
	MenuSelInput& wyswietl();
	// pobiera wybor uzytkownika
	MenuSelInput& execute();
	// dodaje element do listy pozycje
	MenuSelInput & dodaj(string s);
	// usuwa liste
	MenuSelInput & usun_liste();

};
#endif // !MENUSEL_H

