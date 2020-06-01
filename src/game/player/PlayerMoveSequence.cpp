#include "inc/game/player/PlayerMoveSequence.hpp"
#include "inc/game/board/Board.hpp"
#include "inc/game/board/VirtualBoard.hpp"
#include "inc/game/checker/CheckerMan.hpp"
#include "inc/game/player/VirtualPlayerMoveSequence.hpp"

PlayerMoveSequence::PlayerMoveSequence(Board* board, Player::Side turn): applied(0) {
	auto checkers = board->getPlayersCheckers(turn);

	for (auto c: checkers) {
		if (c->canCapture(board)) {
			origins.push_back(c->getSquare());
		}
	}

	if (origins.size() == 0) {
		for (auto c: checkers) {
			if (c->canMove(board)) {
				origins.push_back(c->getSquare());
			}
		}
	}
}

PlayerMoveSequence::PlayerMoveSequence(int applied, std::vector<Square> origins, std::vector<PlayerMove> sequence):
	applied(applied), origins(origins), sequence(sequence) { }

void PlayerMoveSequence::add(PlayerMove move) {
	if (isFinished())
		throw std::logic_error("cant add moves to finished sequence");

	if (!isMoveAllowed(move))
		throw std::logic_error("cant add unallowed move to a sequence");

	sequence.push_back(move);
	origins.clear();
	origins.push_back(sequence[sequence.size() - 1].getDestination());
}

bool PlayerMoveSequence::isMoveAllowed(PlayerMove move) {
	for (PlayerMove& i_move: sequence) {
		Square capture = i_move.getCapture();
		if (capture && capture.isBetween(move.getOrigin(), move.getDestination())) {
			return false;
		}
	}

	return true;
}

void PlayerMoveSequence::apply(Board* board) {
	if (isEmpty())
		throw std::logic_error("cant apply empty sequence");

	for ( ; applied < sequence.size(); applied++) {
		sequence[applied].apply(board);
	}

	if (isFinished()) {
		for (PlayerMove& move: sequence) {
			Square capture = move.getCapture();
			if (capture) {
				board->move(capture, Square());
			}
		}

		Square moveEnd = sequence[sequence.size() - 1].getDestination();
		if (board->isCheckersLastRow(moveEnd)) {
			auto checker = board->at(moveEnd);
			bool isCheckerMan;

			try {
				isCheckerMan = std::dynamic_pointer_cast<CheckerMan>(checker) != nullptr;
			}
			catch (std::bad_cast e) {
				isCheckerMan = false;
			}

			if (isCheckerMan) {
				board->atRef(moveEnd) = std::dynamic_pointer_cast<CheckerMan>(checker)->convertToKing();
			}
		}
	}
}

std::shared_ptr<VirtualPlayerMoveSequence> PlayerMoveSequence::virtualCopy() {
	return std::make_shared<VirtualPlayerMoveSequence>(applied, origins, sequence);
}

std::vector<Square> PlayerMoveSequence::getOriginsForNextMove() {
	return origins;
}

bool PlayerMoveSequence::isOriginForNextMove(Square sq) {
	auto pos = std::find(std::begin(origins), std::end(origins), sq);
	return pos != std::end(origins);
}

int PlayerMoveSequence::capturedCount() {
	int count = 0;

	for (PlayerMove& move: sequence) {
		if (move.getCapture()) {
			count++;
		}
	}

	return count;
}

bool PlayerMoveSequence::isFinished() {
	if (isEmpty())
		return false;
	else
		return sequence[sequence.size() - 1].isLast();
}

bool PlayerMoveSequence::isEmpty() {
	return sequence.size() == 0;
}
