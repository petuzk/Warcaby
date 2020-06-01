#pragma once

#include <memory>
#include "inc/game/board/Board.hpp"
#include "inc/game/checker/Checker.hpp"
#include "inc/game/player/PlayerMove.hpp"

class Board;

class CheckerKing: public Checker {
	void computePossibleCaptures(int v, const std::array<Board::DiagonalInfo, 4>& diagonals,
		Square origin, std::shared_ptr<PlayerMoveSequence> sequence, std::vector<PlayerMove>& dest);

protected:
	std::vector<PlayerMove> computePossibleMoves(Board* board, std::shared_ptr<PlayerMoveSequence> sequence);

public:
	using Checker::Checker;

	bool canMove(Board* board);
	bool canCapture(Board* board);
};