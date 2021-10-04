#include "Game.h"

// obsluga menu

// glowa funkcja obslugujaca menu
void Game::menuaction() {
	scr.clearrect(scr.obszar);
	switch (menusel.lewygorny.Y) {
		case 0: { //nowa gra
			if (!ifplay || czy_wykonac("Zakonczyc gre?")) {
				ifplay = nowa_gra();
			}
			break;
		}
		case 1: { // zapisz gre
			if (ifplay) {
				if (!gra_do_pliku(pobierz_nazwe())) {
					scr.insert_text(COORD{ scr.obszar.Left,scr.obszar.Top }, "Nie udalo sie!");
				}
			}
			break;
		}
		case 2: { // wczytaj gre
			if (!ifplay || czy_wykonac("Zakonczyc gre?")) {
				if (!gra_z_pliku(pobierz_nazwe())) {
					scr.insert_text(COORD{ scr.obszar.Left,scr.obszar.Top }, "Nie udalo sie!");
					resetgame();
				} else {
					ifplay = true;
				}
			}
			break;
		}
		case 3: { // zakoncz gre
			if (!ifplay || czy_wykonac("Zakonczyc gre?")) {
				scr.board2screen();
				resetgame();
			}
			break;
		}
		case 4: { // zamknij program
			if (!ifplay || czy_wykonac("Zakonczyc gre?")) {
				scr.clearscreen();
				exit(0);
			}
			break;
		}
		case 5: { // zatwierdz
			zawierdz_ruch();
		}
	}
}

// konfiguruje nowa gre -- pobiera typ graczy i imiona
bool Game::nowa_gra() {
	scr.board2screen();
	resetgame();
	MenuSelInput playerinput(menusel.wymiary, COORD{ scr.obszar.Left,scr.obszar.Top }, &scr);
	playerinput.dodaj("Czlowiek").dodaj("Komputer").dodaj("Powrot");
	playerinput.komunikat = "";
	memset(playerinput.buffer, '\0', 100);
	playerinput.tytul = "Gracz ";
	playerinput.setpos();

	bool playersset = false;
	int i = 0;
	while (i < 2) {
		playerinput.entry = false;
		playerinput.tytul = "Gracz ";
		playerinput.tytul += gracz->symbol;
		playerinput.tytul += ":";
		playerinput.wyswietl().execute();

		if (playerinput.lewygorny.Y == 2) {
			Player *wsk = gracz;
			for (int ii = 0; ii < 2; ii++) {
				wsk->typ = '\0';
				wsk->name = "                   ";
				scr.insert_text(*wsk->wsp, wsk->name);
				wsk = wsk->nast;
				if (wsk->symbol == 'O') {
					gracz = wsk;
				}
			}
			return false;
		} else if (playerinput.lewygorny.Y == 0) {
			gracz->typ = 'C';
			gracz->delay = 0;
			playerinput.odznacz();
			if (!pobierzstr(gracz->name)) {
				continue;
			}
			scr.insert_text(*gracz->wsp, gracz->name);
		} else {
			gracz->typ = 'K';
			gracz->name = "komputer_";
			gracz->name += gracz->symbol;
			gracz->delay = 1000;
			scr.insert_text(*gracz->wsp, gracz->name);

			{
				MenuSelInput skillinput(menusel.wymiary, COORD{ scr.obszar.Left,scr.obszar.Top }, &scr);
				skillinput.dodaj("Banalny").dodaj("Latwy").dodaj("Sredni").dodaj("Trudny");
				skillinput.komunikat = "";
				memset(skillinput.buffer, '\0', 100);
				skillinput.tytul = "Poziom ";
				skillinput.setpos();
				skillinput.entry = false;				
				skillinput.wyswietl().execute();			

				delete ai[i];

				if (skillinput.lewygorny.Y == 0) {
					ai[i] = new RandomAI(boardsel, gracz->symbol, gracz->nast->symbol, sredni);
				}
				if (skillinput.lewygorny.Y == 1) {
					ai[i] = new StrategyAI(boardsel, gracz->symbol, gracz->nast->symbol, latwy);
				}
				if (skillinput.lewygorny.Y == 2) {
					ai[i] = new StrategyAI(boardsel, gracz->symbol, gracz->nast->symbol, sredni);
				}
				if (skillinput.lewygorny.Y == 3) {
					ai[i] = new StrategyAI(boardsel, gracz->symbol, gracz->nast->symbol, trudny);
				}
				
				gracz->ai = ai[i];
			}
		}
		gracz = gracz->nast;
		i++;
	} // while

	if (gracz->typ == 'C') {
		setboard();
	}
	return true;
}

// menu z opcjami tak/nie
bool Game::czy_wykonac(const string s) {
	MenuSelInput taknieinput(menusel.wymiary, COORD{ scr.obszar.Left,scr.obszar.Top }, &scr);
	taknieinput.dodaj("Tak").dodaj("Nie");
	taknieinput.komunikat = "";
	memset(taknieinput.buffer, '\0', 100);
	taknieinput.tytul = s;
	taknieinput.setpos().wyswietl().execute();
	return (taknieinput.lewygorny.Y == 0) ? true : false;
}

// menu do pobierania nazwy pliku
string Game::pobierz_nazwe() {
	MenuSelInput nameinput(menusel.wymiary, COORD{ scr.obszar.Left,scr.obszar.Top }, &scr);
	nameinput.entry = true;
	nameinput.dodaj("Zatwierdz").dodaj("Powrot");
	nameinput.komunikat = "Za duzo znakow";
	nameinput.tytul = "Nazwa pliku:";
	strcpy_s(nameinput.buffer, "savedgame.txt");
	nameinput.setpos().wyswietl().execute();
	if (nameinput.buffer[0] == '\0' || nameinput.lewygorny.Y == 1) {
		return "";
	}
	string nazwapliku = "";
	nazwapliku += nameinput.buffer;
	return nazwapliku;
}

// menu do pobrania imienia gracza
bool Game::pobierzstr(string& dest) {
	MenuSelInput nameinput(menusel.wymiary, COORD{ scr.obszar.Left,scr.obszar.Top }, &scr);
	nameinput.entry = true;
	nameinput.dodaj("Zatwierdz").dodaj("Powrot");
	nameinput.komunikat = "Za duzo znakow";
	nameinput.tytul = "Imie gracza ";
	nameinput.tytul += gracz->symbol;
	nameinput.tytul += ":";
	strcpy_s(nameinput.buffer, "playerZ");
	nameinput.buffer[6] = gracz->symbol;
	nameinput.setpos().wyswietl().execute();
	if (nameinput.buffer[0] == '\0' || nameinput.lewygorny.Y == 1) {
		return false;
	}
	(gracz->name = "") += nameinput.buffer;
	return (nameinput.buffer[0] != '\0');
};