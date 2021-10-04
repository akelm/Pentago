#ifndef ROTSEL_H
#define ROTSEL_H
#include "Selection.h"

// zaznaczenie przycisku z obrotem
class RotSel : public Selection {
public:
	// przerwa pomiedzy przyciskami
	COORD separation;

	RotSel(COORD wym = { 3,1 }, COORD offs = { 0,0 }, Screen* s = nullptr, COORD sep = { 0,0 }) :
		Selection(wym, offs, s), separation(sep) {}

	// implementacja metod z klasy Selection
	// czy kliknieto w obiekt
	bool handlemouse(COORD mysz);
	// klawisze
	bool handlekey(WORD wVirtualKeyCode);
	
};
#endif // !ROTSEL_H


