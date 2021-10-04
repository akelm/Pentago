#include "Selection.h"

Selection::Selection(COORD wym, COORD offs, Screen* s , WORD odzn , WORD zazn) :
	wymiary(wym),
	scr(s),
	offset(offs), zaznaczony(zazn), odznaczony(odzn), nast(nullptr), lewygorny(COORD{ 0,0 }) {
	buffer = new CHAR_INFO[wymiary.X*wymiary.Y];
	if (!scr) {
		return;
	}
}

Selection::~Selection() {
	delete[] buffer;
}


Selection& Selection::move(COORD wector) {
	odznacz();
	lewygorny += wector * wymiary;
	zaznacz();
	return *this;
}

Selection&  Selection::moveto(COORD wector) {
	odznacz();
	lewygorny = wector;
	zaznacz();
	return *this;
}

void Selection::select(WORD attrib) const{
	int rozm = wymiary.X*wymiary.Y;
	SMALL_RECT lpReadRegion = coord2rect(lewygorny+offset,lewygorny+offset+wymiary-1);
	memset(buffer, 0, rozm* sizeof(CHAR_INFO));
	
	ReadConsoleOutput(
		scr->hout,
		buffer,
		wymiary,
		{ 0,0 },
		&lpReadRegion
	);

	for (int i = 0; i < rozm; i++) {
		buffer[i].Attributes = attrib;
	}

	WriteConsoleOutput(
		scr->hout,
		buffer,
		wymiary,
		{ 0,0 },
		&lpReadRegion
	);
}


Selection& Selection::zaznacz() {
	select(zaznaczony);
	return *this;
}

Selection& Selection::odznacz() {
	select(odznaczony);
	return *this;
}