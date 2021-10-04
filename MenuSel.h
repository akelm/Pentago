#ifndef MENUSEL_H
#define MENUSEL_H
#include "Selection.h"


// klasa obslugujaca glowne menu
class MenuSel : public Selection {
public:
	// ile pozycji w menu
	int ilepoz;

	MenuSel(COORD wym = { 3,1 }, COORD offs = { 0,0 }, Screen* s = nullptr) :
		Selection(wym, offs,s), ilepoz(0) {}
	// obsluga myszy - true jesli kliknieto
	bool handlemouse(COORD mysz);
	// ruch klawiszami
	bool handlekey(WORD wVirtualKeyCode);

};
#endif // !MENUSEL_H


