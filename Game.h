#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <map>
#include <ctime>
#include "Selection.h"
#include "BoardSel.h"
#include "MenuSel.h"
#include "RotSel.h"
#include "QSel.h"
#include "Screen.h"
#include "Player.h"
#include "MenuSelInput.h"
#include "AI.h"
using namespace std;

class Game {

	Screen scr; // obiekt obslugujacy ekran
	BoardSel boardsel; // zaznaczenie pola na planszy
	MenuSel menusel;  //zaznaczenie menu
	RotSel rotsel; // zaznaczenie przycisku rotacji
	QSel qsel; // zaznaczenie cwiartki do obrotu
	Selection* activesel; // aktywne zaznaczenie
	AI * ai[2]{nullptr}; // gracze - komputery

	bool ifexit; // czy uzytkownik chce wyjsc
	bool ifplay; // czy obecnie jest prowadzona gra

	Player playero; // gracz znaku O
	Player playerx; // gracz znaku X
	Player* gracz; // akywny gracz

	// metody obslugujace gre, plik Game.cpp

	// wyswietla na nowo plansze
	void resetgame();
	// zarzadza zaznaczeniami podczas zmiany ruchu na rotacje
	void setrotation();
	// zarzadza zaznaczeniami podczas zmiany ruchu na postawienie znaku
	void setboard();
	// wywoluje ruch komputera
	void ruchkomputera();
	// wpisuje na plansze ruch, sprawdza wygrana
	void zawierdz_ruch();
	// przekazuje zdarzenia klawiatury
	void handlekey( WORD key);
	// przekazuje dalej zdarzenia myszy
	void handlemouse( COORD mysz);

	// obsluga menu, plik Game_menu.cpp

	// glowa funkcja obslugujaca menu
	void menuaction();
	// konfiguruje nowa gre -- pobiera typ graczy i imiona
	bool nowa_gra();
	// menu z opcjami tak/nie
	bool czy_wykonac(const string s);
	// menu do pobierania nazwy pliku
	string pobierz_nazwe();
	// menu do pobrania imienia gracza
	bool pobierzstr(string & dest);

	// wczytywanie i zapisywanie do plikow, plik Game_io.cpp

	// zapisuje gre do pliku
	bool gra_do_pliku(const string s);
	// wczytuje linie nie szersza niz dostepne miejsce na ekranie, obcina biale znaki
	bool wczytaj_linie(ifstream & ifstr, string & dest);
	// to samo, ale nie linie tylko pojedynczy znak
	bool wczytaj_linie(ifstream & ifstr, char & dest);
	// sprawdza, czy string moze byc linia planszy
	bool sprawdzpopr(const string & linia, char * dest);
	// tworzy gracza-komputer na podstawie danych z pliku
	bool initAi(string typ, int id, char symbol1, char symbol2, Player * gracz);
	// wczytuje gre z pliku
	bool gra_z_pliku(const string s);

public:
	Game(string filestr = "board.txt");

	~Game();

	// glowna funkcja obslugujaca eventy z klawiatury i myszy, plik Game.cpp
	int mainloop();


};