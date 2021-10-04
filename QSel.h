#ifndef QSEL_H
#define QSEL_H
#include "Selection.h"

// zaznaczenie dla wyboru cwiartki
class QSel : public Selection {
public:
	QSel(COORD wym = { 3,1 }, COORD offs = { 0,0 },Screen* s=nullptr,
		COORD sep = { 0,0 }) :
		Selection(wym, offs, s), separation(sep) {
		koloruj();
	}

	// ostep pomiedzy cwiartkami
	COORD separation;
	QSel & odznacz();
	QSel & koloruj();
	// obsluga myszy, metoda z Selection
	bool handlemouse(COORD mysz);
	// obsluga klawiatury, metoda z Selection
	bool handlekey(WORD wVirtualKeyCode);



};
#endif // !QSEL_H


