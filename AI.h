#ifndef AI_H
#define AI_H

#include "BoardSel.h"
#include <vector>
#include <sstream>

using namespace std;

// informacje o ruchu wybranym przez AI
struct RuchAI {
	COORD ruch;	// pozycja ruchu na planszy 
	COORD quart;// cwiartka ktora zostanie obrocona
	COORD rot;	// kierunek obrotu
};

// poziom trudnosci AI (istotne tylko StrategyAI)
enum TrudnoscAI {
	latwy,	// AI moze probowac przeszkodzic graczowi w ustawieniu linii 5 znakow
	sredni,	// AI zawsze powinno probowac powstrzymac gracza gdy wykryje linie 4 znakow, moze celowo blokowac linie 3 znakow
	trudny	// AI zawsze blokuje linie 4 znakow oraz 3 znakow z miejscem na obu koncach. Moze celowo blokowac linie 3 znakow.
};

// struktura pomocnicza dla AI, opisuje linie na planszy skladajaca sie ze znakow tego samego gracza
// uzywane do: 
//		wykrywania linii przeciwnika w celu ich blokowania
//		wykrywania linii AI w celu ich predluzania
//		sprawdzania, czy w danym kierunku jest dosc wolnych pol, by zbudowac wygrywajaca linie
struct linia {
	COORD poczatek;
	COORD koniec;
	SHORT dlugosc;
	SHORT kierx;
	SHORT kiery;
	char poleprzed;	// zawartosc kolejnego pola w kierunku w ktorym biegnie linia
	char poleza;	// zawartosc pola przed poczatkiem linii, zgodnie z jej kierunkiem
};

// klasa bazowa reprezentujaca komputerowego przeciwnika
class AI {
protected:
	const BoardSel & board;
	const char mojsymb;		// symbol tego gracza
	const char symbprzec;	// symbol przeciwnika
	TrudnoscAI trudnosc;

public:
	AI(BoardSel & board, char mojSymbol, char wrogiSymbol, TrudnoscAI trudnosc)
		: board(board), mojsymb(mojSymbol), symbprzec(wrogiSymbol), trudnosc(trudnosc) {}

	virtual ~AI() {}

	// zwraca informacje o ruchu wybranym przez AI, nie zmienia stanu planszy
	virtual RuchAI ruch() = 0;
	// zwraca znak pozwalajacy zidentyfikowac typ i poziom trudnosci podklasy
	// uzywane do zapisu/wczytywanis stanu gry
	virtual char id() = 0;
};

// zarowno obroty i miejsca wstawiania znakow sa losowane
class RandomAI : public AI {
	SHORT losuj(SHORT lmax, SHORT lmin) {
		SHORT spread = lmax - lmin + 1;
		return ((SHORT)rand() % (spread)+lmin);
	}

public:
	RandomAI(BoardSel & board, char mojSymbol, char wrogiSymbol, TrudnoscAI trudnosc) :
		AI(board, mojSymbol, wrogiSymbol, trudnosc) {}

	~RandomAI() {}

	RuchAI ruch();
	char id() {
		return 'R';
	}
};

// AI starajaca sie wniosowac na podstawie linii skladajacych sie z pionkow na planszy
//   *jezeli wykryje linie przeciwnika o dlugoci 3 lub 4, 
//		stara sie ja zablokowac (zalezy od poziomu trudnosci)
//   *jezeli na planszy nie ma dobrze rokujacych pionkow AI, wstawia kolejny w losowym miejscu
//   *jezeli na planszy nie ma dobrze rokujacych linii AI, 
//	    ale sa pionki od ktorych mozna prowadzic wygrywajaca linie, losuje ruch w ich okolicy
//   *jezeli na planszy sa dobrze rokujace linie, losuje ruch przedluzajacy jedna z nich w ktoryms z dostepnych kierunkow
//   *obroty i ich kierunek sa wybierane losowo.
class StrategyAI : public AI
{
	// czy punkt znajduje sie na planszy
	inline bool okcoord(short x, short y) {
		return x >= 0 && x <= 5 && y >= 0 && y <= 5;
	}

	// znajduje wszystkie linie na planszy skladajace sie z podanego znaku
	 vector<linia> znajdzlinie(const char stan[6][6], char znak);

	// szuka linii skladajacej sie z podanego znaku, zaczynajacej sie w pkcoe x/y, biegnacej w kierunku kierx/kiery
	 linia szukajliniizpktu(const char stan[6][6], char znak, SHORT x, SHORT y, SHORT kierx, SHORT kiery);

	// zwraca liste ruchow adekwatnych dla danej sytuacji (nie jest to zbior wszystkich mozliwych ruchow)
	 vector<COORD> dostepneruchy(char stan[6][6]);

public:
	StrategyAI(BoardSel & board, char mojSymbol, char wrogiSymbol, TrudnoscAI trudnosc) :
		AI(board, mojSymbol, wrogiSymbol, trudnosc) {}

	~StrategyAI() {}

	 RuchAI ruch();
	 char id() {
		switch (trudnosc) {
			case latwy:
				return 'A';
			case sredni:
				return 'B';
			case trudny:
				return 'C';
			default:
				return '?';
		}
	}
};

#endif