#include "MenuSel.h"

bool MenuSel::handlemouse(COORD mysz) {
	mysz = mysz - offset;
	// poza zakresem
	if (mysz.X < 0 || mysz.X >= wymiary.X  || mysz.Y < 0 || mysz.Y >= ilepoz) {
		return false;
	}
	mysz.X = 0;
	moveto(mysz);
	return true;
}

bool MenuSel::handlekey(WORD wVirtualKeyCode) {
	COORD vector = { 0,0 };
	switch (wVirtualKeyCode) {
		case VK_UP: {
			vector = { 0,-1 };
			break;
		}
		case VK_DOWN: {
			vector = { 0,1 };
			break;
		}
		default: return false;
	}
	COORD mysz = lewygorny + vector * wymiary;
	if (mysz.Y >= ilepoz) {
		mysz.Y = 0;
	}
	if (mysz.Y < 0) {
		mysz.Y = ilepoz-1;
	}
	moveto(mysz);
	return true;
}