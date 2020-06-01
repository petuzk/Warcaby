#include "inc/game/checker/Checker.hpp"
#include "inc/game/board/Board.hpp"
#include "inc/game/player/PlayerMove.hpp"
#include "inc/game/player/PlayerMoveSequence.hpp"

Checker::Checker(Square square, Player::Side side): mySquare(square), mySide(side), cachedVersion(-1) { }

void Checker::moveTo(Square square) {
	setSquare(square);
}

void Checker::setSquare(Square square) {
	mySquare = square;
}

Square Checker::getSquare() {
	return mySquare;
}

Player::Side Checker::getSide() {
	return mySide;
}

std::vector<PlayerMove> Checker::getPossibleMoves(Board* board, std::shared_ptr<PlayerMoveSequence> sequence) {
	if (board->getVersion() == cachedVersion)
		return cachedMoves;
	else {
		if (!sequence->isOriginForNextMove(mySquare))
			throw std::invalid_argument("wrong checker for the sequence");

		cachedMoves = computePossibleMoves(board, sequence);
		cachedVersion = board->getVersion();
		return cachedMoves;
	}
}