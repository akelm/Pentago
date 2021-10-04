#include "BoardSel.h"

BoardSel & BoardSel::odznacz() {
	//COORD tmp = lewygorny;
	if (lewygorny >= 3 * wymiary || lewygorny < 3 * wymiary) {
		odznaczony = ODZN2;
	} else {
		odznaczony = ODZN1;
	}
	Selection::odznacz();
	return *this;
}

bool BoardSel::handlemouse(COORD mysz) {
	mysz = mysz - offset;
	// poza zakresem
	if (mysz.X < 0 || mysz.X >  3 * 6 - 1 || mysz.Y < 0 || mysz.Y>  5) {
		return false;
	}
	mysz.X -= mysz.X % 3;
	if (czypusta(mysz)) {
		moveto(mysz);
	}
	return true;
}

bool BoardSel::handlekey(WORD wVirtualKeyCode) {
	COORD wector = { 0,0 };
	switch (wVirtualKeyCode) {
		case VK_LEFT: {
			wector = { -1,0 };
			break;
		}
		case VK_RIGHT: {
			wector = { 1,0 };
			break;
		}
		case VK_UP: {
			wector = { 0,-1 };
			break;
		}
		case VK_DOWN: {
			wector = { 0,1 };
			break;
		}
		default: return false;
	}

	bool czyznal = false;
	COORD mysz = lewygorny;
	COORD start = lewygorny;
	while (!czyznal) {
		do {
			mysz += wector * wymiary;
			correctpos(mysz, wector);
		} while (!czypusta(mysz) && mysz != start);

		if (mysz == start) {
			mysz.X += wector.Y*wymiary.X;
			mysz.Y += wector.X*wymiary.Y;
			correctpos(mysz, wector);
			start = mysz;
		} else {
			czyznal = true;
		}
	}
	moveto(mysz);
	return false;
}

BoardSel& BoardSel::zaznacz_poz(char symb) {
	scr->wpisz_znak(offset + lewygorny + COORD{ 1, 0 }, symb);
	odznacz();
	return *this;
}

BoardSel& BoardSel::odznacz_poz() {
	scr->wpisz_znak(offset + lewygorny + COORD{ 1, 0 }, '.');
	odznacz();
	return *this;
}

void BoardSel::correctpos(COORD& mysz, COORD& wektor) {
	COORD bsize = 6 * wymiary;
	while (mysz.X < 0) {
		mysz.X += bsize.X;
	}
	while (mysz.X >= bsize.X) {
		mysz.X -= bsize.X;
	}
	while (mysz.Y < 0) {
		mysz.Y += bsize.Y;
	}
	while (mysz.Y >= bsize.Y) {
		mysz.Y -= bsize.Y;
	}
}

bool BoardSel::movetoemptypos() {
	if (czypusta(lewygorny)) {
		return true;
	}
	SHORT i = 0, j = 0;
	COORD wsp = { 0, 0 };
	for (SHORT i = 0; i < 6; i++) {
		for (SHORT j = 0; j < 6; j++) {
			wsp = COORD{ j,i } *wymiary;
			if (czypusta(wsp)) {
				lewygorny = wsp;
				return true;
			}
		}
	}
	return false;
}

bool BoardSel::czypusta(const COORD wsp) const {
	return (scr->pobierz_znak(offset + wsp + COORD{ 1,0 }) == '.');
}

char BoardSel::evaluate(char zn1, char zn2) {
	if (sprawdzwygr(zn1)) {
		return zn1;
	}
	if (sprawdzwygr(zn2)) {
		return zn2;
	}
	return '\0';
}

int BoardSel::zliczlinie(char symb, COORD start, COORD kierunek) {
	COORD pozycja = start;
	int ile_max = 0;
	int ile = 0;

	for (int i = 0; i < 6; i++) {
		if (pozycja.X < 1 || pozycja.Y < 0 || pozycja.X > 16 || pozycja.Y > 5) { continue; }

		if (scr->pobierz_znak(pozycja + offset) == symb) {
			ile++;
		} else {
			if (ile_max < ile) {
				ile_max = ile;
			}
			ile = 0;
		}

		pozycja += kierunek;
	}

	if (ile_max < ile) {
		return ile;
	} else {
		return ile_max;
	}
}

bool BoardSel::sprawdzlinie(char symb, COORD start, COORD kierunek) {
	return zliczlinie(symb, start, kierunek) >= 5;
}

bool BoardSel::sprawdzwygr(char symb) {
	// poziomo
	{
		COORD kierunek = { wymiary.X, 0 };
		for (SHORT i = 0; i < 6; i++) {
			if (sprawdzlinie(symb, COORD{ 1, i*wymiary.Y }, kierunek)) {
				return true;
			}
		}
	}

	// pionowo
	{
		COORD kierunek = { 0, wymiary.Y };
		for (SHORT i = 0; i < 6; i++) {
			if (sprawdzlinie(symb, { 1 + i * wymiary.X, 0 }, kierunek)) {
				return true;
			}
		}
	}

	// przekatna - w dol
	{
		COORD kierunek = { wymiary.X, wymiary.Y };
		COORD startPos[] = { COORD{1,0}, COORD{1,wymiary.Y }, COORD{1 + wymiary.X,0} };
		for (int i = 0; i < 3; i++) {
			COORD start = startPos[i];

			if (sprawdzlinie(symb, start, kierunek)) {
				return true;
			}
		}
	}

	// przekatna - w gore
	{
		COORD kierunek = { wymiary.X, -wymiary.Y };
		COORD startPos[] = { COORD{ 1,wymiary.Y * 5 }, COORD{ 1,wymiary.Y * 4 }, COORD{ 1 + wymiary.X,wymiary.Y * 5 } };
		for (int i = 0; i < 3; i++) {
			COORD start = startPos[i];

			if (sprawdzlinie(symb, start, kierunek)) {
				return true;
			}
		}
	}

	return false;
}

BoardSel& BoardSel::rotate(COORD wsp, bool dir) {
	// wsp to lewygorny qsel
	SMALL_RECT wsp_rect = coord2rect(wsp + offset, wsp + offset + 3 * wymiary - 1);
	CHAR_INFO* buffer = new CHAR_INFO[9 * wymiary.Y*wymiary.X];

	ReadConsoleOutput(scr->hout, buffer, 3 * wymiary, COORD{ 0,0 }, &wsp_rect);

	char znak = '\0';
	COORD localwsp = COORD{ 0,0 };
	COORD newwsp = COORD{ 0,0 };
	for (SHORT i = -1; i < 2; i++) {
		for (SHORT j = -1; j < 2; j++) {
			COORD buffer_coord = COORD{ j + 1,i + 1 }*wymiary + COORD{ 1,0 };
			znak = buffer[buffer_coord.Y * 3 * wymiary.X + buffer_coord.X*wymiary.Y].Char.AsciiChar;
			newwsp = (dir) ? COORD{ -i + 1, j + 1 } : COORD{ i + 1, -j + 1 };
			newwsp *= wymiary;
			newwsp += (wsp + offset + COORD{ 1,0 });
			scr->wpisz_znak(newwsp, znak);
		}
	}

	delete[] buffer;
	return *this;
}


void BoardSel::czytajplansze(char stan[6][6]) const {
	for (SHORT x = 1, indx = 0; indx <= 5; x += wymiary.X, indx++) {
		for (SHORT y = 0, indy = 0; y <= 5; y += wymiary.Y, indy++)
		{
			stan[indx][indy] = scr->pobierz_znak(COORD{ x,y } +offset);
		}
	}
}

const BoardSel& BoardSel::wypiszplansze(const char stan[6][6]) const {
	for (SHORT x = 1, indx = 0; indx <= 5; x += wymiary.X, indx++) {
		for (SHORT y = 0, indy = 0; y <= 5; y += wymiary.Y, indy++)
		{
			scr->wpisz_znak(COORD{ x,y } +offset, stan[indx][indy]);
		}
	}
	return *this;
}