#include "RotSel.h"

bool RotSel::handlemouse(COORD mysz) {
	mysz = mysz - offset;
	// poza zakresem
	if (mysz.X < 0 || mysz.X >= 2*wymiary.X + separation.X || mysz.Y < 0 || mysz.Y >= wymiary.Y) {
		return false;
	}
	// przerwa
	if (mysz.X >= wymiary.X && mysz.X < wymiary.X + separation.X) {
		return true;
	}
	mysz.Y = 0;
	mysz.X = (mysz.X > wymiary.X) ? wymiary.X + separation.X : 0;
	moveto(mysz);
	return true;
}

bool RotSel::handlekey(WORD wVirtualKeyCode) {
	COORD vector = { 0,0 };
	switch (wVirtualKeyCode) {
		case VK_LEFT:
		case VK_RIGHT: {
			vector = { 1,0 };
			break;
		}
		default: return false;
	}
	COORD mysz = lewygorny + (2 * (!lewygorny) - COORD{ 1,1 }) * vector * (wymiary + separation);
	moveto(mysz);
	return true;
}


