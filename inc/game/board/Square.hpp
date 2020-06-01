#pragma once

#include <string>
#include <locale>
#include <stdexcept>
#include "inc/game/board/BoardConst.hpp"

/* Identyfikuje pojedyńcze pole na planszy.
 * Jeżeli numer kolumny lub rzędu jest większy niż rozmiar planszy
 * lub mniejszy niż zero, odpowiednie pola _col i _row
 * przechowują wartość -1 i takie pole jest "poza planszą"
 * (Square::isOutside() = true)
 */

class Square {
	int _col, _row;

public:

	/* Reprezentuje pole względem dowolnego pola.
	 * Można te obiekty traktować jako wektory.
	 * Zakresy numerów rzędów i kolumn nie są sprawdzane.
	 * Obiekt może być przeskalowany oraz dodany do obiektu typu Square.
	 */

	class Offset {
		int _coloff, _rowoff;

	public:
		// Konstruuje wektor zerowy
		Offset();

		// Konstruuje wektor o podanych współrzędnych
		Offset(int coloff, int rowoff);

		int colOff() const;
		int rowOff() const;

		Offset operator*(int mul) const;
		int& operator[](int i);
	};

	// Konstruuje pole poza planszą
	Square();

	// Konstruuje pole o podanym rzędzie i kolumnie
	Square(int col, int row);

	// Konstruuje pole o podanej nazwie (np. A1, X17)
	Square(const char* name);
	Square(std::string name);

	bool isOutside() const;
	bool isBlack() const;
	int col() const;
	int row() const;
	std::string name();

	bool isBetween(Square x, Square y) const;
	Square operator+(Offset offset) const;
	bool operator==(Square other) const;
	bool operator!=(Square other) const;

	// Zwraca true, jeżeli pole jest na planszy (nie poza)
	operator bool() const;
};