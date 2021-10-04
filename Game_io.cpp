#include "Game.h"

// obsluga plikow


bool Game::gra_do_pliku(const string s) {
	ifstream ifstr(s);
	if (ifstr.good()) {
		ifstr.close();
		return false;
	}
	ifstr.close();
	ofstream ofstr(s);
	if (ofstr.is_open()) {
		ofstr << playero.name << endl;
		ofstr << playero.typ << endl;
		if (playero.typ == 'K') ofstr << playero.ai->id() << endl;
		ofstr << playerx.name << endl;
		ofstr << playerx.typ << endl;
		if (playerx.typ == 'K') ofstr << playerx.ai->id() << endl;
		ofstr << gracz->symbol << endl;
		ofstr << ((activesel->nast == &boardsel) ? "pole" : "rotacja") << endl;
		COORD wsp = boardsel.offset + COORD{ 1, 0 };
		char buffer[6][6];
		boardsel.czytajplansze(buffer);
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				ofstr << buffer[i][j];
			}
			ofstr << endl;
		}
	} else {
		return false;
	}
	ofstr.close();
	return true;
}


bool Game::wczytaj_linie(ifstream& ifstr, string& dest) {
	const char* bialeznaki = " \t\v\r\n";
	string tmp;
	if (getline(ifstr, tmp, '\n')) {
		int pocz = tmp.find_first_not_of(bialeznaki);
		int kon = tmp.find_last_not_of(bialeznaki);
		if (kon - pocz + 1 > menusel.wymiary.X) {
			kon = menusel.wymiary.X + pocz - 1;
		}
		dest = tmp.substr(pocz, kon - pocz + 1);
		return true;
	} else {
		return false;
	}
}

bool Game::wczytaj_linie(ifstream& ifstr, char& dest) {
	const char* bialeznaki = " \t\v\r\n";
	string tmp;
	if (getline(ifstr, tmp, '\n')) {
		int pocz = tmp.find_first_not_of(bialeznaki);
		dest = tmp[pocz];
		return true;
	} else {
		return false;
	}
}

bool Game::sprawdzpopr(const string& linia, char* dest) {
	if (linia.length() < 6) {
		return false;
	}
	for (int i = 0; i < 6; i++) {
		if (linia[i] != 'X' && linia[i] != 'O' && linia[i] != '.') {
			return false;
		} else {
			dest[i] = linia[i];
		}
	}
	return true;
}

bool Game::initAi(string typ, int id, char symbol1, char symbol2, Player * gracz) {
	delete ai[id];
	if (typ == "R") {
		ai[id] = new RandomAI(boardsel, symbol1, symbol2, sredni);
	} else if (typ == "A") {
		ai[id] = new StrategyAI(boardsel, symbol1, symbol2, latwy);
	} else if (typ == "B") {
		ai[id] = new StrategyAI(boardsel, symbol1, symbol2, sredni);
	} else if (typ == "C") {
		ai[id] = new StrategyAI(boardsel, symbol1, symbol2, trudny);
	} else {
		return false;
	}

	gracz->ai = ai[id];
	gracz->delay = 1000;

	return true;
}

bool Game::gra_z_pliku(const string s) {
	if (s.length() < 1) {
		return false;
	}
	ifstream ifstr(s);
	if (!ifstr.good()) {
		return false;
	}
	char ktogra = '\0';
	string typai1 = "";
	string typai2 = "";
	string tmp = "";
	if (!(wczytaj_linie(ifstr, playero.name))) {
		return false;
	}
	if (!wczytaj_linie(ifstr, playero.typ)) {
		return false;
	}
	if (playero.typ == 'K' && !wczytaj_linie(ifstr, typai1)) {
		return false;
	}
	if (!wczytaj_linie(ifstr, playerx.name)) {
		return false;
	}
	if (!wczytaj_linie(ifstr, playerx.typ)) {
		return false;
	}
	if (playerx.typ == 'K' && !wczytaj_linie(ifstr, typai2)) {
		return false;
	}
	if (!wczytaj_linie(ifstr, ktogra)) {
		return false;
	}
	if (!wczytaj_linie(ifstr, tmp)) {
		return false;
	}

	if (!(playero.typ == 'K' || playero.typ == 'C') && !(playerx.typ == 'K' || playerx.typ == 'C')) {
		return false;
	}

	playero.delay = 0;
	playerx.delay = 0;

	scr.insert_text(scr.playeropos, playero.name);
	scr.insert_text(scr.playerxpos, playerx.name);
	if (ktogra == 'O') {
		gracz = &playero;
	} else if (ktogra == 'X') {
		gracz = &playerx;
	} else {
		return false;
	}

	scr.insert_text(scr.curplayerpos, gracz->name);
	if (!tmp.compare("rotacja")) {
		menusel.nast = &qsel;
	} else if (!tmp.compare("pole")) {
		menusel.nast = &boardsel;
	} else {
		return false;
	}

	char plansza[6][6];
	for (int i = 0; i < 6; i++) {
		if (!(wczytaj_linie(ifstr, tmp) && sprawdzpopr(tmp, plansza[i]))) {
			return false;
		}
	}

	boardsel.wypiszplansze(plansza);
	boardsel.movetoemptypos();

	ifstr.close();

	if (playero.typ == 'K') {
		initAi(typai1, 0, 'O', 'X', &playero);
	}

	if (playerx.typ == 'K') {
		initAi(typai2, 1, 'X', 'O', &playerx);
	}

	ifplay = true;
	return true;
}