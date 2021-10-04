#include "MenuSelInput.h"
#include "helpers.h"

// na podstawie pol oblicza pozycje elementow
MenuSelInput& MenuSelInput::setpos() {
	scr->clearrect(scr->obszar);
	if (tytul.length()) {
		offset.Y += 1;
	}
	if (entry) {
		entry_coord = offset;
		offset.Y += 1;

	}
	if (entry&&komunikat.length()) {
		komunikat_coord = coord2rect(offset, COORD{ wymiary.X, offset.Y });
		offset.Y += 1;
	}
	return *this;
}

// wyswietla elementy
MenuSelInput& MenuSelInput::wyswietl() {
	scr->clearrect(scr->obszar);
	COORD wsp = offset;
	//wsp.Y -= 1;
	if (entry&&komunikat.length()) {
		wsp.Y -= 1;
	}
	if (entry) {
		wsp.Y -= 1;
		scr->insert_text(wsp, buffer);
	}
	if (tytul.length()) {
		wsp.Y -= 1;
		scr->insert_text(wsp, tytul);
	}
	
	ElementListy<string>* wsk = pozycje.glowa;
	COORD wsk_coord = offset;
	//wsk_coord.Y += 1;
	while (wsk) {
		scr->insert_text(wsk_coord, wsk->wartosc);
		wsk = wsk->nast;
		wsk_coord.Y += 1;
	}
	if (pozycje.glowa) {
		zaznacz();
	}
	return *this;
}

// obsluga zczytywania wyboru uzytkownika
// i zczytywania tekstu
MenuSelInput& MenuSelInput::execute() {
	// ustawiam kursor tam, gdzie konczy sie zawartosc bufora
	int ile = 0;
	for (; buffer[ile] != '\0'; ile++) {}
	entry_coord.X += ile;
	SetConsoleCursorPosition(scr->hout, entry_coord);

	// ustawiam kursor
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = (entry)?25:1;
	cci.bVisible = (entry) ? TRUE : FALSE;
	SetConsoleCursorInfo(scr->hout, &cci);

	// zmienne do wczytywania
	bool wczytano = false;
	INPUT_RECORD InputRecord;
	DWORD Events = 0;
	Sleep(100);
	FlushConsoleInputBuffer(scr->hin);
	while (!wczytano) {
		ReadConsoleInput(scr->hin, &InputRecord, 1, &Events);
		if (InputRecord.EventType == MOUSE_EVENT
			&& InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED
			&& pozycje.glowa) {
			if (handlemouse(InputRecord.Event.MouseEvent.dwMousePosition)) {
				wczytano = true;
				odznacz();
			}
		} else if (InputRecord.EventType == KEY_EVENT
			&& (InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_UP
				|| InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
			&& pozycje.glowa) {
			handlekey(InputRecord.Event.KeyEvent.wVirtualKeyCode);
			Sleep(200);
			FlushConsoleInputBuffer(scr->hin);
		} else if (entry && InputRecord.EventType == KEY_EVENT && entry
			&& InputRecord.Event.KeyEvent.uChar.AsciiChar >'\0') {
			buffer[ile] = InputRecord.Event.KeyEvent.uChar.AsciiChar;
			if (buffer[ile] == '\b' && ile > 0) {
				scr->clearrect(komunikat_coord);
				buffer[ile] = '\0';
				ile--;
				buffer[ile] = '\0';
				entry_coord.X--;
				SetConsoleCursorPosition(scr->hout, entry_coord);
				scr->wpisz_znak(entry_coord, ' ');
			} else if (buffer[ile] == '\b' && ile == 0) {
				buffer[ile] = '\0';
			} else if (buffer[ile] == '\r') {
				scr->clearrect(komunikat_coord);
				wczytano = true;
				buffer[ile] = '\0';
			} else if (ile == wymiary.X) {
				scr->insert_text(komunikat_coord, komunikat);
			} else if (isalnum(buffer[ile]) || buffer[ile] == ' ' || buffer[ile] == '_' || buffer[ile] == '.') {
				scr->clearrect(komunikat_coord);
				scr->wpisz_znak(entry_coord, buffer[ile]);
				entry_coord.X += 1;
				SetConsoleCursorPosition(scr->hout, entry_coord);
				ile++;
			}
			Sleep(200);
			FlushConsoleInputBuffer(scr->hin);
		} else if (InputRecord.EventType == KEY_EVENT
			&& InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
			wczytano = true;
			Sleep(200);
			FlushConsoleInputBuffer(scr->hin);
		} else if (InputRecord.EventType == WINDOW_BUFFER_SIZE_EVENT) {
			scr->setconsoleprop();
			// ustawiam kursor
			cci.dwSize = (entry) ? 25 : 1;
			cci.bVisible = (entry) ? TRUE : FALSE;
			SetConsoleCursorInfo(scr->hout, &cci);
		} else {// focus itp
			FlushConsoleInputBuffer(scr->hin);
		}// if
	} // while
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(scr->hout, &cci);
	if (pozycje.glowa) {
		odznacz();
	}
	scr->clearrect(scr->obszar);
	return *this;
}


MenuSelInput& MenuSelInput::dodaj(string s) {
	pozycje.dodaj(s);
	ilepoz++;
	return *this;
}


MenuSelInput& MenuSelInput::usun_liste() {
	scr->clearrect(scr->obszar);
	pozycje.usun_liste();
	return *this;
}