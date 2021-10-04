#include "Screen.h"

Screen::Screen(string filestr) : nazwaplanszy(filestr) {
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	hin = GetStdHandle(STD_INPUT_HANDLE);
	coordunset();
	board2screen();
	obszar = coord2rect(COORD{ menupos.X, menulast.Y + 2 }, rightbottom);
	setconsoleprop();
}

void Screen::clearscreen() {
	// wpisuje ' ' do konsoli
	COORD pocz = { 0, 0 };
	DWORD ileznakow;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD rozm;

	GetConsoleScreenBufferInfo(hout, &csbi);
	rozm = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hout, ' ', rozm, pocz, &ileznakow);
	FillConsoleOutputAttribute(hout, ODZN, rozm, pocz, &ileznakow);
}

char Screen::pobierz_znak(COORD wsp) {
	DWORD ileznakow;
	char znak;
	ReadConsoleOutputCharacter(hout, &znak, 1, wsp, &ileznakow);
	return znak;
}

void Screen::coordunset() {
	for (int i = 0; i < 7; i++) {
		*pozycje[i] = COORD{ -1, -1 };
	}
}

void Screen::coordassign(string& line, SHORT row) {
	for (int i = 0; i < 7; i++) {
		if (*pozycje[i] == COORD{ -1,-1 }) {
			SHORT ind;
			if ((ind = (SHORT)line.find(znaczniki[i])) > 0) {
				*pozycje[i] = COORD{ ind, row };
				pozycje[i]->X += przesuniecia[i];
			}
		}
	}
}

void Screen::board2screen() {
	clearscreen();
	string line;
	ifstream myfile(nazwaplanszy);
	if (myfile.is_open()) {
		SHORT row = 0;
		SHORT col = 0;
		while (getline(myfile, line)) {
			SHORT tmp = (SHORT)line.find_last_not_of(" \t");
			line = line.substr(0, tmp + 1);
			col = (tmp > col) ? tmp : col;
			coordassign(line, row);
			insert_text(COORD{ 0,row}, line);
			row++;
		}
		myfile.close();
		rightbottom = COORD{ col,row };
		for (int i = 0; i < 7; i++) {
			if (pozycje[i]->X < 0 || pozycje[i]->Y < 0) {
				clearscreen();
				cout << "Nie udalo sie wczytac planszy." << endl;
				exit(0);
			}
		}
	} else {
		clearscreen();
		cout << "Nie udalo sie wczytac planszy." << endl;
		exit(0);
	}
}

void Screen::clearrect(SMALL_RECT wsp) {
	DWORD ilezn;
	for (SHORT j = wsp.Top; j <= wsp.Bottom; j++) {
		for (SHORT i = wsp.Left; i <= wsp.Right; i++) {
			WriteConsoleOutputCharacter(hout, " ", 1, COORD{ i,j }, &ilezn);
		}
	}
}

void Screen::insert_text(COORD wsp, string str) {
	SMALL_RECT sr = coord2rect(wsp, wsp + COORD{ (SHORT)str.length(),0 });
	clearrect(sr);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hout, &csbi);
	DWORD ilezn;
	for (SHORT k = 0; k < (SHORT)str.length(); k++) {
		WriteConsoleOutputCharacter(hout, &str[k], 1, wsp + COORD{ k,0 }, &ilezn);
	}
}

void Screen::insert_text(SMALL_RECT sr, string str) {
	clearrect(sr);
	if ((SHORT)str.length() > sr.Right - sr.Left + 1) {
		str.resize(sr.Right - sr.Left + 1);
	}
	COORD wsp = { sr.Right - (SHORT)str.length() + 1, sr.Bottom };
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hout, &csbi);
	DWORD ilezn;
	for (SHORT k = 0; k < (SHORT)str.length(); k++) {
		WriteConsoleOutputCharacter(hout, &str[k], 1, wsp + COORD{ k,0 }, &ilezn);
	}
}

void Screen::wpisz_znak(COORD wsp, char znak) {
	DWORD ilezn;
	WriteConsoleOutputCharacter(hout, &znak, 1, wsp, &ilezn);

}


void Screen::setconsoleprop() {
	SetConsoleWindowInfo(hout, TRUE, &coord2rect(COORD{ 0,0 }, rightbottom + COORD{ 1,0 }));
	SetConsoleScreenBufferSize(hout, rightbottom + COORD{ 2,1 });
	SetConsoleTitle("PENTAGO");
	SetConsoleMode(hin, ENABLE_EXTENDED_FLAGS);
	SetConsoleMode(hin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hout, &cci);
}