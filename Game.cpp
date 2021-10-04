#include "Game.h"

	Game::Game(string filestr) :
		scr(Screen(filestr)),
		boardsel(BoardSel(COORD{ 3,1 }, scr.boardpos, &scr)),
		menusel(MenuSel(COORD{ 15,1 }, scr.menupos, &scr)),
		rotsel(RotSel(COORD{ 4,3 }, scr.rotpos, &scr)),
		qsel(QSel(COORD{ 9,3 }, scr.boardpos, &scr)),
		activesel(&menusel),
		ifexit(false),
		ifplay(false),
		playero(Player('O')),
		playerx(Player('X')),
		gracz(&playero) {
	// wskazniki pomiedzy obiektami, rozmiary pol
	srand((unsigned)time(nullptr));
	boardsel.nast = &menusel;
	menusel.nast = &boardsel;
	qsel.nast = &rotsel;
	rotsel.nast = &menusel;
	playero.nast = &playerx;
	playerx.nast = &playero;
	playero.wsp = &scr.playeropos;
	playerx.wsp = &scr.playerxpos;
	menusel.ilepoz = 6;
	rotsel.separation = { 8,0 };
	activesel->zaznacz();
	scr.obszar = coord2rect(menusel.offset + COORD{ 0, (SHORT)menusel.ilepoz + 1 }, scr.rightbottom);
}

Game::~Game() {
	delete ai[0];
	delete ai[1];
}

void Game::resetgame() {
	ifplay = false;
	boardsel.moveto(boardsel.lewygorny);
	boardsel.odznacz();
	activesel->odznacz(); 
	activesel = &menusel;
	qsel.koloruj();
	activesel->zaznacz(); 
}

void Game::setrotation() {
	activesel->odznacz(); // board i rot/q nie wygaszaja sie, menu sie wygasza
	boardsel.zaznacz_poz(gracz->symbol);
	boardsel.movetoemptypos();
	menusel.nast = &qsel;
	activesel = &qsel;
	qsel.zaznacz();
	rotsel.zaznacz();
}

void Game::setboard() {
	activesel->odznacz();
	rotsel.odznacz();
	qsel.odznacz();
	activesel = &boardsel;
	boardsel.movetoemptypos();
	menusel.nast = activesel;
	scr.clearrect(coord2rect(scr.curplayerpos, scr.curplayerpos + COORD{ menusel.wymiary.X,0 }));
	scr.insert_text(scr.curplayerpos, gracz->name);
	activesel->zaznacz();
}

void Game::ruchkomputera() {
	RuchAI ruchAi = gracz->ai->ruch();

	scr.clearrect(coord2rect(scr.curplayerpos, COORD{ scr.rightbottom.X / 2, scr.curplayerpos.Y }));
	scr.insert_text(scr.curplayerpos, gracz->name);
	boardsel.odznacz().moveto(ruchAi.ruch * boardsel.wymiary);
	Sleep(gracz->delay);
	boardsel.odznacz();
	boardsel.zaznacz_poz(gracz->symbol);
	rotsel.moveto(COORD{ ruchAi.rot.X * (rotsel.wymiary.X + rotsel.separation.X) , 0 });
	qsel.moveto(qsel.wymiary * ruchAi.quart);
	Sleep(gracz->delay);
	rotsel.odznacz();
	qsel.odznacz();
	activesel = &qsel;
	zawierdz_ruch();
}

void Game::zawierdz_ruch() {
	if (!ifplay) {
		return;
	}
	if (activesel == &menusel) {
		activesel = activesel->nast;
	}
	if (activesel == &rotsel || activesel == &qsel) {
		boardsel.rotate(qsel.lewygorny, rotsel.lewygorny.X == 0);
		// jesli wygrana, wyswietli komunikat i zmieni stan na brak gry
		char zwyciezca = boardsel.evaluate(gracz->symbol,gracz->nast->symbol);
		if (zwyciezca != '\0') {
			string wygrany = "Wygral(a): ";
			scr.insert_text(COORD{ scr.obszar.Left,scr.obszar.Top }, wygrany);

			if (gracz->symbol == zwyciezca) {
				wygrany = gracz->name +" (" + gracz->symbol + ")";
			} else {
				wygrany = gracz->nast->name + " (" + gracz->nast->symbol + ")";
			}
			wygrany += "!";
			scr.insert_text(COORD{ scr.obszar.Left,scr.obszar.Top + 1 }, wygrany);
			qsel.odznacz();
			rotsel.odznacz();
			boardsel.odznacz();
			activesel = &menusel;
			activesel->zaznacz();
			ifplay = false;
		} else if (!boardsel.movetoemptypos()) { // plansza zapelniona - remis
			scr.insert_text(COORD{ scr.obszar.Left,scr.obszar.Top }, "Remis!");
			qsel.odznacz();
			rotsel.odznacz();
			boardsel.odznacz();
			activesel = &menusel;
			activesel->zaznacz();
			ifplay = false;
		} else {
			gracz = gracz->nast;
			if (gracz->typ == 'C') {
				setboard();
			}
		}
	} else if (activesel == &boardsel) {
		//} else if (state&BOARDFLAG) {
		setrotation();
	}
}

void Game::handlekey(const WORD key) {
	if (key == VK_TAB) {
		if (ifplay) {
			if (activesel == &menusel) {
				activesel->odznacz(); // board i rot/q nie wygaszaja sie, menu sie wygasza
			}
			activesel = activesel->nast;
			activesel->zaznacz(); // wszystkie sie zapalaja
		} else {
			activesel->odznacz(); // board i rot/q nie wygaszaja sie, menu sie wygasza
			activesel = &menusel;
			activesel->zaznacz(); // wszystkie sie zapalaja
		}
	} else if (key == VK_RETURN) {
		if (activesel == &menusel) {
			menuaction();
		} else {
			zawierdz_ruch();
		}
	} else {
		activesel->handlekey(key); // ruchy po plnaszy/menu itp, ogolnie strzalki
	}
}

void Game::handlemouse(const COORD mysz) {
	if (activesel->handlemouse(mysz)) {
		if (activesel == &menusel) {
			menuaction();
		}
		return;
	}
	Selection* tmp = activesel->nast;
	while (activesel != tmp) {
		if (tmp->handlemouse(mysz)) {
			if (activesel == &menusel) {
				activesel->odznacz();
			}
			activesel = tmp;
			activesel->zaznacz();
			if (activesel == &menusel) {
				menuaction();
			}
			break;
		} else {
			tmp = tmp->nast;
		}
	}
}

int Game::mainloop() {
	resetgame();
	INPUT_RECORD InputRecord;
	DWORD Events;

	// blokuje zmiane rozm okna
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	while (!ifexit) {
		if (ifplay && gracz->typ == 'K') {
			ruchkomputera();
		} else {
			ReadConsoleInput(scr.hin, &InputRecord, 1, &Events);
			if (InputRecord.EventType == KEY_EVENT) {
				handlekey(InputRecord.Event.KeyEvent.wVirtualKeyCode);
				Sleep(200);
				FlushConsoleInputBuffer(scr.hin);
			} else if (InputRecord.EventType == MOUSE_EVENT && InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				handlemouse(InputRecord.Event.MouseEvent.dwMousePosition);
				Sleep(200);
				FlushConsoleInputBuffer(scr.hin);
			} else if (InputRecord.EventType == WINDOW_BUFFER_SIZE_EVENT) {
				scr.setconsoleprop();
			} else { // focus event, menuevent
				FlushConsoleInputBuffer(scr.hin);
			}

		}
	}
	return 0;
}


