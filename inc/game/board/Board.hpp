#pragma once

#include <array>
#include <vector>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include "inc/game/board/BoardConst.hpp"
#include "inc/game/board/Square.hpp"
#include "inc/game/checker/Checker.hpp"
#include "inc/game/player/Player.hpp"

class VirtualBoard;

/* Reprezentuje planszę.
 * Przechowuje i zarządza obiektami typu Checker.
 */

class Board {
public:
	struct DiagonalInfo {
		Square::Offset direction;
		std::vector<std::shared_ptr<Checker>> checkers;
	};

private:
	// Tablica pól czarnych, przechowuje piony
	// Wykorzystanie: board[row/2][col]
	std::shared_ptr<Checker> board[BoardConst::SIZE / 2][BoardConst::SIZE];

	// Zbite piony nie mogą być usuwane z pamięci
	// gdyż mogą być wykorzystywane w implementacji graficznej
	std::vector<std::shared_ptr<Checker>> outside;

protected:
	// "Wersja" planszy, jest inkrementowana po każdym ruchu
	// Pozwala utworzyć pamięć cache, która by przechowywała np. możliwe ruchy
	int version;

	// Wyczyszcza board i outside
	void erase();

	// Przesuwa pion w pamięci, ale nie wywołuje metodę Checker::moveTo().
	// Zwraca przesunięty pion
	std::shared_ptr<Checker> moveInternal(Square from, Square to);

public:
	Board();
	virtual ~Board();

	/* Wyczyszcza planszę i tworzy obiekty danego typu. Typ ma być implementacją klasy CheckerMan.
	 * Utworzone obiekty istnieją dopóki nie zostaną usunięte w kolejnym wywołaniu reset().
	 */
	template <typename CheckerManImpl>
	void reset();

	// Zwraca referencję do komórki w tablicy board, ktora reprezentuje dane pole
	std::shared_ptr<Checker>& atRef(Square sq);

	// Zwraca zawartość pola
	std::shared_ptr<Checker> at(Square sq);

	std::vector<std::shared_ptr<Checker>> getPlayersCheckers(Player::Side side, bool includeOutside = false);
	bool isCheckersLastRow(Square sq);

	// Przesuwa pion
	void move(Square from, Square to);

	// Zwraca wirtualną kopię planszy (patrz VirtualBoard.hpp)
	std::shared_ptr<VirtualBoard> virtualCopy();

	// Zwraca "wersję" planszy
	int getVersion();

	/* Zwraca elementy elementy znajdujące się na "półprzekątnych" (promieniach) danego pola.
	 * Są tak ułożone, że:
	 *
	 *   f           c    * items[0].checkers[0] = a
	 *     e       b      * items[0].checkers[1] = b
	 *       d   a        * items[0].checkers[2] = c
	 *         P          * items[1].checkers[0] = d
	 *       g   j        * items[1].checkers[1] = e
	 *     h       k      * items[2].checkers[0] = g
	 *   i           l    * items[3].checkers[0] = j
	 *
	 * items[0] jest promieniem skierowanym do prawej górnej komórki w kierunku ruchu piona.
	 */
	std::array<DiagonalInfo, 4> getDiagonalItems(Square from);
};

std::ostream& operator<<(std::ostream& stream, Board* b);