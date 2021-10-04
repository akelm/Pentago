#include "QSel.h"

QSel & QSel::odznacz() {
	//COORD tmp = lewygorny;
	if (lewygorny >= wymiary || lewygorny < wymiary) {
		odznaczony = ODZN2;
	} else {
		odznaczony = ODZN1;
	}
	Selection::odznacz();
	return *this;
}

QSel& QSel::koloruj() {
	COORD tmp = lewygorny;
	lewygorny = COORD{ 0,0 };
	odznacz();
	lewygorny = wymiary;
	odznacz();
	lewygorny = COORD{ wymiary.X, 0 };
	odznacz();
	lewygorny = COORD{ 0, wymiary.Y };
	odznacz();
	lewygorny = tmp;
	return *this;
}



bool QSel::handlemouse(COORD mysz) {
	mysz = mysz - offset;
	// poza zakresem
	if (mysz.X < 0 || mysz.X >=  2*wymiary.X + separation.X || mysz.Y < 0 || mysz.Y>=  2*wymiary.Y) {
		return false;
	}
	mysz = (wymiary + separation) *(mysz / (wymiary + separation));
	moveto(mysz);
	return true;
}

bool QSel::handlekey(WORD wVirtualKeyCode) {
	COORD vector = { 0,0 };
	switch (wVirtualKeyCode) {
		case VK_LEFT:
		case VK_RIGHT: {
			vector = { 1,0 };
			break;
		}
		case VK_UP:
		case VK_DOWN: {
			vector = { 0,1 };
			break;
		}
		case VK_RETURN: {
			odznacz();
			return false;
		}
		default: return false;
	}

	COORD mysz = lewygorny + (2 * (!lewygorny) - COORD{ 1,1 }) * vector * (wymiary + separation);
	moveto(mysz);
	return false;
}

