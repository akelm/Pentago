#ifndef SELECTION_H
#define SELECTION_H
#include "helpers.h"
#include "Screen.h"




// bazowa klasa do zaznaczen
class Selection {
	// bufor do wczytywania/wypisywania zawartosci zaznaczenia
	CHAR_INFO* buffer;
	// robi zaznaczenie, ustawienia kolorow sa w WORD attrib
	// opis kodow jest tutaj: https://docs.microsoft.com/en-us/windows/console/char-info-str
	void select(WORD attrib) const;
protected:
	// wskaznik do aktywnego Screena, udostepnie wskazniki do konsoli i kilka metod
	Screen* scr;
	// ustawienia pola w stanie odznaczony
	WORD odznaczony;
	// ustawienia pola w stanie zaznaczony
	WORD zaznaczony;

public:
	// wymiary pojedyczego elementu zaznaczenia
	COORD wymiary;

	// polozenie lewego gornego rogu zaznaczenia wzgledem 
	// lewego gornego rogu obiektu
	// to pole sluzy do sprawdzania, co zostalo zaznaczone
	COORD lewygorny;
	// lewy gorny rog calego obiektu
	COORD offset;
	// nastepne zaznaczenie
	Selection* nast;
	// zaznacze
	Selection & zaznacz();
	// odznacza
	virtual Selection & odznacz();
	// odznacza, przesuwa o wymiary*wektor, zaznacza
	Selection&  move(COORD wektor);
	// odznacza, przesuwa do pozycji, zaznacza
	Selection&  moveto(COORD wector);

	// metody do zaimplementowania w klasach pochodnych

	// obsluga myszy, zwraca prawde, jesli COORD mysz jest w obiekcie
	virtual bool handlemouse(COORD mysz) = 0;
	// obsluga strzalek 
	virtual bool handlekey(WORD wVirtualKeyCode) = 0;

	Selection(COORD wym = { 3,1 }, COORD offs = { 0,0 }, Screen* s = nullptr,
		WORD odzn = ODZN, WORD zazn = ZAZN);

	virtual ~Selection();

};
#endif // !SELECTION_H


