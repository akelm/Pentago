#ifndef BOARDSEL_H
#define BOARDSEL_H
#include "Selection.h"

// klasa obslugujaca pojedyncze zaznaczenie na planszy
class BoardSel : public Selection {

	// liczy ile razy symb kolejno wystepuje w danej linii
	int zliczlinie(char symb, COORD start, COORD kierunek);
	// sprawdza czy symb wystepuje 5 razy (obok siebie) w danej linii
	bool sprawdzlinie(char symb, COORD start, COORD kierunek);
	// sprawdza, czy dany znak wygral
	bool sprawdzwygr(char symb);
	// koryguje pozycje na planszy, jak ruch klawiszem - wektor wyjdzie poza zakres
	void correctpos(COORD & mysz, COORD & wektor);
public:

	BoardSel(COORD wym = { 3,1 }, COORD offs = { 0,0 }, Screen* s = nullptr,  COORD sep = { 0,0 }) :
		Selection(wym, offs,s), separation(sep) {}

	// odstep pomiedzy elementami zaznaczenia
	COORD separation;
	// obsluga myszy, meotda z Selection
	bool handlemouse(COORD mysz);
	// obsluga klawiszy, meotda z Selection
	bool handlekey(WORD wVirtualKeyCode);
	// ustawia znak na planszy
	BoardSel& zaznacz_poz(char symb);
	// usuwa znak z planszy
	BoardSel& odznacz_poz();
	// przesuwa zaznaczenie do pierwszej pustej pozycji
	bool movetoemptypos();
	// sprawdza, ze pole jest puste
	bool czypusta(const COORD wsp) const;
	// sprawdza wygrana
	char evaluate(char zn1, char zn2);
	// obraca cwiartke
	BoardSel& rotate(COORD wsp, bool dir);
	// wczytuje plansze do tablicy
	void czytajplansze(char stan[6][6]) const;
	// wypisuje plansze na ekran
	const BoardSel& wypiszplansze(const char stan[6][6]) const;
	// nadpisuje metode klasy bazowej, zeby uwzglednic kolor planszy
	BoardSel & odznacz();

};
#endif // !BOARDSEL_H


