#ifndef HELPERS_H
#define HELPERS_H

#include <Windows.h>
#include <iostream>
using namespace std;

// element listy 2 kierunkowej
template<typename T=string>
class ElementListy {
public:
	T wartosc;
	ElementListy* nast;
	ElementListy* poprz;

	ElementListy(T s="") : wartosc(s), nast(nullptr), poprz(nullptr) {}
};

// lista 2-kierunkowa
template<typename T>
class Lista2kier {
public:
	ElementListy<T>* glowa;

	Lista2kier() : glowa(nullptr) {}
	~Lista2kier() { usun_liste(); }

	Lista2kier& dodaj(string wartosc = "") {
		if (!glowa) {
			glowa = new ElementListy<T>(wartosc);
			return *this;
		}
		ElementListy<T>* wsk = glowa;
		while (wsk->nast) {
			wsk = wsk->nast;
		}
		ElementListy<T>* nowy = new ElementListy<T>(wartosc);
		wsk->nast = nowy;
		nowy->poprz = glowa;
		return *this;
	}

	Lista2kier& usun_element(ElementListy<T>* el) {
		if (el) {
			if (el == glowa) {
				glowa = el->nast;
			}
			if (el->nast) {
				ElementListy<T>* wsk = el->nast;
				wsk->poprz = nullptr;
				el->nast = nullptr;
			}
			if (el->poprz) {
				ElementListy<T>* wsk = el->poprz;
				wsk->nast = nullptr;
				el->poprz = nullptr;
			}
			delete el;
		}
		return *this;
	}

	Lista2kier& usun_liste() {
		while (glowa) {
			usun_element(glowa);
		}
		return *this;
	}
};



// losowanie SHORTa
SHORT losuj(SHORT lmax, SHORT lmin);

// SMALL rect z dwoch wspolrzednych
SMALL_RECT coord2rect(const COORD a, const COORD b);

// przeciazone operatory dla COORD
const COORD operator+(const COORD& a, const COORD& b);

const COORD operator-(const COORD& a);

const COORD operator-(const COORD& a, const COORD& b);

ostream& operator<<(ostream& os, const COORD& a);

void operator+=(COORD& a, const COORD& b);

const COORD operator*(const COORD& a, const COORD& b);

void operator*=(COORD& a, const COORD& b);

const COORD operator-(const COORD& a, const SHORT& b);

const bool operator==(const COORD& a, const COORD& b);

const bool operator!=(const COORD& a, const COORD& b);

const COORD operator!(const COORD& a);

const bool operator>(const COORD& a, const COORD& b);

const bool operator>=(const COORD& a, const COORD& b);

const bool operator<(const COORD& a, const COORD& b);

const bool operator<=(const COORD& a, const COORD& b);

const COORD operator/(const COORD& a, const COORD& b);

void operator*=(COORD& a, const SHORT b);

void operator*(COORD& a, const SHORT b);
const COORD operator*(const SHORT b, const COORD& a);


#endif // !HELPERS_H


