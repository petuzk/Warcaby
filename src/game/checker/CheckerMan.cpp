#include "inc/game/checker/CheckerMan.hpp"
#include "inc/game/board/Board.hpp"
#include "inc/game/board/VirtualBoard.hpp"
#include "inc/game/player/VirtualPlayerMoveSequence.hpp"

void CheckerMan::computePossibleCaptures(int v, const std::array<Board::DiagonalInfo, 4>& diagonals,
	Square origin, std::shared_ptr<PlayerMoveSequence> sequence, std::vector<PlayerMove>& dest)
{
	for (const auto& diagonal: diagonals) {
		const auto& c = diagonal.checkers;
		if (c.size() >= 2 && c[0] != nullptr && c[0]->getSide() != mySide && c[1] == nullptr) {
			PlayerMove m(v, origin, origin + diagonal.direction * 2, origin + diagonal.direction);
			if (sequence == nullptr || sequence->isMoveAllowed(m)) {
				dest.push_back(m);
			}
		}
	}
}

std::vector<PlayerMove> CheckerMan::computePossibleMoves(Board* board, std::shared_ptr<PlayerMoveSequence> sequence) {
	int v = board->getVersion();
	auto moves = std::vector<PlayerMove>();
	auto diagonals = board->getDiagonalItems(mySquare);

	// czy można wykonać bicie
	computePossibleCaptures(v, diagonals, mySquare, sequence, moves);

	if (moves.size() != 0) {
		// jeżeli tak, sprawdź, czy są możliwe kolejne
		for (auto& move: moves) {
			auto nextSequence = sequence->virtualCopy(); nextSequence->add(move);
			auto nextOrigin = nextSequence->getOriginsForNextMove().at(0);
			auto nextBoard = board->virtualCopy(); nextSequence->apply(nextBoard);
			std::vector<PlayerMove> tmp;
			computePossibleCaptures(nextBoard->getVersion(), nextBoard->getDiagonalItems(nextOrigin), nextOrigin, nextSequence, tmp);
			if (tmp.size() != 0) {
				// jeżeli tak, wyzeruj flagę isLast
				move.unsetIsLast();
			}
		}
	}
	else {
		// jeżeli nie, zapisz możliwe ruchy
		for (int i = 0; i < 2; i++) {
			const auto& c = diagonals[i].checkers;
			if (c.size() >= 1 && c[0] == nullptr) {
				moves.push_back(PlayerMove(v, mySquare, mySquare + diagonals[i].direction));
			}
		}
	}

	return moves;
}

bool CheckerMan::canMove(Board* board) {
	auto diagonals = board->getDiagonalItems(mySquare);
	return
		(diagonals[0].checkers.size() != 0 && diagonals[0].checkers[0] == nullptr) ||
		(diagonals[1].checkers.size() != 0 && diagonals[1].checkers[0] == nullptr);
}

bool CheckerMan::canCapture(Board* board) {
	int v = board->getVersion();
	auto moves = std::vector<PlayerMove>();
	auto diagonals = board->getDiagonalItems(mySquare);

	// czy można wykonać bicie
	computePossibleCaptures(v, diagonals, mySquare, nullptr, moves);

	return moves.size() != 0;
}

std::shared_ptr<CheckerKing> CheckerMan::convertToKing() {
	return std::make_shared<CheckerKing>(mySquare, mySide);
}