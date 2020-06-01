#include "inc/game/checker/CheckerKing.hpp"
#include "inc/game/board/Board.hpp"
#include "inc/game/board/VirtualBoard.hpp"
#include "inc/game/player/VirtualPlayerMoveSequence.hpp"

void CheckerKing::computePossibleCaptures(int v, const std::array<Board::DiagonalInfo, 4>& diagonals,
	Square origin, std::shared_ptr<PlayerMoveSequence> sequence, std::vector<PlayerMove>& dest)
{
	for (const auto& diagonal: diagonals) {
		const auto& c = diagonal.checkers;
		int i = 0;
		while (i < c.size() && c[i] == nullptr) i++; // pomiń puste pola
		// i = numer pierwszego napotkanego pionu
		if (i < c.size() && c[i]->getSide() != mySide) {
			Square enemy = origin + diagonal.direction * (i + 1);
			for (i++; i < c.size() && c[i] == nullptr; i++) {
				// dodaj puste pola za pionem przeciwnika
				PlayerMove m(v, origin, origin + diagonal.direction * (i + 1), enemy);
				if (sequence == nullptr || sequence->isMoveAllowed(m)) {
					dest.push_back(m);
				}
			}
		}
	}
}

std::vector<PlayerMove> CheckerKing::computePossibleMoves(Board* board, std::shared_ptr<PlayerMoveSequence> sequence) {
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
			if (tmp.size() != 0)
				// jeżeli tak, wyzeruj flagę isLast
				move.unsetIsLast();
		}
	}
	else {
		// jeżeli nie, zapisz możliwe ruchy
		for (int i = 0; i < 4; i++) {
			const auto& c = diagonals[i].checkers;
			for (int j = 0; j < c.size() && c[j] == nullptr; j++) {
				moves.push_back(PlayerMove(v, mySquare, mySquare + diagonals[i].direction * (j + 1)));
			}
		}
	}

	return moves;
}

bool CheckerKing::canMove(Board* board) {
	auto diagonals = board->getDiagonalItems(mySquare);
	for (int i = 0; i < 4; i++) {
		if (diagonals[i].checkers.size() != 0 && diagonals[i].checkers[0] == nullptr) {
			return true;
		}
	}

	return false;
}

bool CheckerKing::canCapture(Board* board) {
	int v = board->getVersion();
	auto moves = std::vector<PlayerMove>();
	auto diagonals = board->getDiagonalItems(mySquare);

	// czy można wykonać bicie
	computePossibleCaptures(v, diagonals, mySquare, nullptr, moves);

	return moves.size() != 0;
}