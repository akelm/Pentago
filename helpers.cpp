#include "helpers.h"

const COORD operator+(const COORD& a, const COORD& b) {
	return { a.X + b.X, a.Y + b.Y };
}

const COORD operator*(const COORD& a, const COORD& b) {
	return { a.X * b.X, a.Y * b.Y };
}


const COORD operator*(const COORD& a, const SHORT b) {
	return { a.X * b, a.Y * b };
}

const COORD operator*(const SHORT b, const COORD& a) {
	return { a.X * b, a.Y * b };
}

const COORD operator/(const COORD& a, const COORD& b) {
	return { a.X / b.X, a.Y / b.Y };
}

const COORD operator-(const COORD& a) {
	return { -a.X, -a.Y };
}

const COORD operator-(const COORD& a, const COORD& b) {
	return a + (-b);
}

ostream& operator<<(ostream& os, const COORD& a) {
	os << "(" << a.X << ", " << a.Y << ")";
	return os;
}

void operator+=(COORD& a, const COORD& b) {
	a = a + b;
}

void operator*=(COORD& a, const COORD& b) {
	a = a * b;
}

void operator*=(COORD& a, const SHORT b) {
	a.X *= b;
	a.Y *= b;
}


const bool operator==(const COORD& a, const COORD& b) {
	if (a.X == b.X && a.Y == b.Y) {
		return true;
	}
	return false;
}

const bool operator!=(const COORD& a, const COORD& b) {
	if (a.X == b.X && a.Y == b.Y) {
		return false;
	}
	return true;
}

const COORD operator!(const COORD& a) {
	return COORD{ a.X == 0, a.Y == 0 };
}


const bool operator>(const COORD& a, const COORD& b) {
	if (a.X > b.X && a.Y > b.Y) {
		return true;
	}
	return false;
}

const bool operator>=(const COORD& a, const COORD& b) {
	if (a.X >= b.X && a.Y >= b.Y) {
		return true;
	}
	return false;
}

const bool operator<(const COORD& a, const COORD& b) {
	if (a.X < b.X && a.Y < b.Y) {
		return true;
	}
	return false;
}

const bool operator<=(const COORD& a, const COORD& b) {
	if (a.X <= b.X && a.Y <= b.Y) {
		return true;
	}
	return false;
}

const COORD operator-(const COORD& a, const SHORT& b) {
	COORD c = { b,b };
	return a - c;
}

SMALL_RECT coord2rect(const  COORD a, const  COORD b) {
	SMALL_RECT c;
	c.Left = a.X;
	c.Top = a.Y;
	c.Right = b.X;
	c.Bottom = b.Y;
	return c;
}

SHORT losuj(SHORT lmax, SHORT lmin) {
	SHORT spread = lmax - lmin + 1;
	return ((SHORT)rand() % (spread)+lmin);
}