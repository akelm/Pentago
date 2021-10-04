#include "AI.h"


 RuchAI RandomAI::ruch() {
	// plansza
	COORD nowewsp = { losuj(5,0),losuj(5,0) };
	while (!board.czypusta(nowewsp * board.wymiary)) {
		nowewsp = { losuj(5,0),losuj(5,0) };
	}

	return { nowewsp, COORD{ losuj(1,0), losuj(1,0) }, COORD{ losuj(1,0),0 } };
}


 vector<linia> StrategyAI::znajdzlinie(const char stan[6][6], char znak) {
	vector<linia> linie;
	// przeszukujemy wszystkie pkty na planszy
	for (SHORT x = 0; x < 6; x++) {
		for (SHORT y = 0; y < 6; y++) {
			if (stan[x][y] == znak) {
				// dla kazdego znalezionego znaku sprawdzamy czy w ktoryms kierunku biegnie linia
				for (int ox = -1; ox <= 1; ox++) {
					for (int oy = -1; oy <= 1; oy++) {
						linia res = szukajliniizpktu(stan, znak, x, y, ox, oy);
						// pomijamy linie o dlugosci mniejszej niz 2
						if (res.dlugosc > 1 && res.poczatek != res.koniec) {
							linie.push_back(res);
						}
					}
				}
			}
		}
	}

	return linie;
}

 linia StrategyAI::szukajliniizpktu(const char stan[6][6], char znak, SHORT x, SHORT y, SHORT kierunekX, SHORT kierunekY) {
	// (nie sprawdzamy od srodka linii)
	if (okcoord(x - kierunekX, y - kierunekY) && stan[x - kierunekX][y - kierunekY] == znak
		&& okcoord(x + kierunekX, y + kierunekY) && stan[x + kierunekX][y + kierunekY] == znak) {
		return { {},{},0 };
	}

	linia linia;
	linia.poczatek = { x,y };
	linia.koniec = { x,y };
	linia.dlugosc = 1;
	linia.kierx = kierunekX;
	linia.kiery = kierunekY;
	linia.poleza = okcoord(x - kierunekX, y - kierunekY) ? stan[x - kierunekX][y - kierunekY] : '\0';
	linia.poleprzed = okcoord(x + kierunekX, y + kierunekY) ? stan[x + kierunekX][y + kierunekY] : '\0';

	SHORT cx = x, cy = y;

	while (okcoord(cx + kierunekX, cy + kierunekY) && stan[cx + kierunekX][cy + kierunekY] == znak) {
		cx += kierunekX;
		cy += kierunekY;
		linia.dlugosc++;
		linia.koniec = { cx,cy };
		linia.poleprzed = okcoord(cx + kierunekX, cy + kierunekY) ? stan[cx + kierunekX][cy + kierunekY] : '\0';
	}

	return linia;
}

 vector<COORD> StrategyAI::dostepneruchy(char stan[6][6]) {
	vector<COORD> ruchy;

	vector<linia> linie = znajdzlinie(stan, mojsymb);
	vector<linia> linie_przec = znajdzlinie(stan, symbprzec);

	bool czyobrona = false;
	// sprawdzamy czy przeciwnik jest bliski wygranej
	// blokowanie ruchow zalezy od poziomu trudnosci
	for (const linia & l : linie_przec) {
		if (trudnosc == latwy && l.dlugosc > 3 // latwo - blokujemy tylko linie 4 znakow
			|| trudnosc != latwy && l.dlugosc > 2 // troche trudniej - mozemy atakowac tez linie 3 znakow
			) {

			if ((trudnosc != latwy && l.dlugosc > 3) // troche trudniej - zawsze staramy sie blokowac linie z 4 znakow
				|| (trudnosc == trudny // trudno - zawsze atakujemy linie 3 znakow z pustymi polami na koncach
					&& (l.poleprzed == '.' && l.poleza == '.'))) { 
				
				czyobrona = true;
			}

			if (l.poleprzed == '.') {
				ruchy.push_back({ l.koniec.X + l.kierx, l.koniec.Y + l.kiery });
			}
			if (l.poleza == '.') {
				ruchy.push_back({ l.poczatek.X - l.kierx, l.poczatek.Y - l.kiery });
			}
		}
	}

	// jezeli nie musimy blokowac przeciwnika, generujemy liste ruchow przedluzajacych linie
	if (!linie.empty() && !czyobrona) {
		for (const linia & l : linie) {
			if (l.poleprzed == '.') {
				linia res = szukajliniizpktu(stan, '.', l.koniec.X + l.kierx, l.koniec.Y + l.kiery, l.kierx, l.kiery);
				if (l.dlugosc + res.dlugosc > 3) {
					ruchy.push_back({ l.koniec.X + l.kierx, l.koniec.Y + l.kiery });
				}
			}
			if (l.poleza == '.') {
				linia res = szukajliniizpktu(stan, '.', l.poczatek.X - l.kierx, l.poczatek.Y - l.kiery, -l.kierx, -l.kiery);
				if (l.dlugosc + res.dlugosc > 3) {
					ruchy.push_back({ l.poczatek.X - l.kierx, l.poczatek.Y - l.kiery });
				}
			}
		}
	}

	// jak nie ma linii do przedluzania i nie musimy pilnie blokowac przeciwnika 
	//  tworzymy inna linie w dobrze rokujacym kierunku
	if (linie.empty() && !czyobrona) {
		for (SHORT x = 0; x < 6; x++) {
			for (SHORT y = 0; y < 6; y++) {
				if (stan[x][y] == mojsymb) {
					for (SHORT ox = -1; ox <= 1; ox++) {
						for (SHORT oy = -1; oy <= 1; oy++) {
							if (okcoord(x + ox, y + oy) && stan[x + ox][y + oy] == '.') {
								linia res = szukajliniizpktu(stan, '.', x + ox, y + oy, ox, oy);
								if (res.dlugosc > 3) {
									ruchy.push_back({ x + ox, y + oy });
								}
							}
						}
					}
				}
			}
		}
	}

	// jak nie ma pomyslu na konkretny ruch, zwracamy wszystkie miejsca na planszy 
	// (pozniej wylosowany zostanie z nich ruch)
	if (ruchy.empty()) {
		for (SHORT x = 0; x < 6; x++) {
			for (SHORT y = 0; y < 6; y++) {
				if (stan[x][y] == '.') {
					ruchy.push_back({ x,y });
				}
			}
		}
	}

	return ruchy;
}

 RuchAI StrategyAI::ruch() {

	char stan[6][6];
	board.czytajplansze(stan);

	const vector<COORD> ruchy = dostepneruchy(stan);

	COORD ruch = ruchy[rand() % ruchy.size()];

	// zwracamy losowy ruch ze zbioru dostepnych	
	return {
		ruch,
		COORD{ rand() % 2,rand() % 2 },
		COORD{ (rand() % 2) - 1,0 }
	};
}
