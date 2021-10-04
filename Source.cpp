#include "Game.h"

int main(int argc, char *argv[]) {
	if (argc > 2) {
		cout << "Program przyjmuje 1 argument:" << endl;
		cout << "nazwe pliku z plansza" << endl;
		return 0;
	} else {
		if (argc == 2) {
			string nazwa = "";
			nazwa += argv[1];
			Game gs(nazwa);
			return gs.mainloop();
		} else {
			Game gs;
			return gs.mainloop();
		}
	}
}