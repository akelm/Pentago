#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "ai.h"

using namespace std;

// typ danych bedacy graczem
struct Player {
public:
	// typ gracz: czlowiek czy komputer
	char typ;
	// symbol stawiany na planszy
	const char symbol;
	// imie gracza
	string name;
	// opoznienie wykonania ruchu w ms, 0 dla czlowieka
	// bo on sie sam opoznia
	int delay;
	// wskaznik na drugiego gracza
	Player* nast;
	// wspolrzedne, gdzie pojawia sie imie gracza
	COORD* wsp;
	// jest komputer, to wskaznik do obiektu,
	// ktory obsluguje jego ruchy
	AI* ai;


	Player(char sym='\0', char t = 'K', string s="", int d=0): symbol(sym), typ(t), name(s), delay(d), 
		nast(nullptr), wsp(nullptr), ai(nullptr){
	}
	 ~Player() {};


};


#endif // !1
