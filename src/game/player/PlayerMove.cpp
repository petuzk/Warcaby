#include "inc/game/player/PlayerMove.hpp"
#include "inc/game/board/Board.hpp"
#include "inc/game/board/VirtualBoard.hpp"

PlayerMove::PlayerMove(int boardVersion, Square origin, Square destination, Square capture):
	origin(origin), destination(destination), boardVersion(boardVersion), capture(capture), last(true) { }

Square PlayerMove::getOrigin() { return origin; }
Square PlayerMove::getDestination() { return destination; }
Square PlayerMove::getCapture() { return capture; }

void PlayerMove::apply(Board* board) {
	if (boardVersion != board->getVersion()) {
		throw std::logic_error("board version mismatch");
	}

	board->move(origin, destination);
}

bool PlayerMove::isLast() {
	return last;
}

void PlayerMove::unsetIsLast() {
	last = false;
}