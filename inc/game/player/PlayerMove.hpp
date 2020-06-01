#pragma once

#include <stdexcept>
#include "inc/game/board/Square.hpp"

class Board;
class VirtualBoard;

/* Reprezentuje pojedyńczy ruch.
 */

class PlayerMove {

	bool last;
	Square origin, destination, capture;
	int boardVersion;

	// Ograniczenie dostępu do konstruktora i unsetIsLast()
	friend class CheckerMan;
	friend class CheckerKing;
	PlayerMove(int boardVersion, Square origin, Square destination, Square capture = Square());
	void unsetIsLast();

	// Ograniczenie dostępu do apply()
	friend class PlayerMoveSequence;
	void apply(Board* board);

public:
	Square getOrigin();
	Square getDestination();
	Square getCapture();

	bool isLast();
};