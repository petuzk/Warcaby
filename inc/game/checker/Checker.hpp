#pragma once

#include <vector>
#include <memory>
#include "inc/game/board/Square.hpp"
#include "inc/game/player/Player.hpp"

class Board;
class PlayerMove;
class PlayerMoveSequence;

/* Reprezentuje pion.
 * Dziedzicząc klasę, należy implementowac computePossibleMoves()
 * i ewentualnie moveTo() (np. dla animacji przesunięcia).
 */

class Checker {
protected:
	Square mySquare;
	Player::Side mySide;

	// Wersja planszy, dla której były wyliczone cachedMoves
	int cachedVersion;
	std::vector<PlayerMove> cachedMoves;

	virtual std::vector<PlayerMove> computePossibleMoves(Board* board, std::shared_ptr<PlayerMoveSequence> sequence) = 0;

public:
	Checker(Square square, Player::Side side);
	virtual ~Checker() = default;

	virtual void moveTo(Square square);
	virtual void setSquare(Square square);

	virtual Square getSquare();
	virtual Player::Side getSide();

	// Zwraca cachedMoves jeżeli zgadza się wersja planszy,
	// jeżeli nie -- wywołuje computePossibleMoves()
	virtual std::vector<PlayerMove> getPossibleMoves(Board* board, std::shared_ptr<PlayerMoveSequence> sequence);

	// Czy pion może się poruszyć / wykonać bicie na danej planszy
	virtual bool canMove(Board* board) = 0;
	virtual bool canCapture(Board* board) = 0;
};