#include "inc/game/board/Board.hpp"
#include "inc/game/board/VirtualBoard.hpp"

Board::Board(): version(0) {
	erase();
}

Board::~Board() {
	erase();
}

void Board::erase() {
	outside.clear();
	for (int row = 0; row < BoardConst::SIZE; row++) {
		for (int col = row % 2; col < BoardConst::SIZE; col += 2) {
			atRef(Square(col, row)) = nullptr;
		}
	}
}

/*
 * template <typename CheckerManImpl>
 * void Board::reset();
 *
 * Patrz Board__reset.cpp
 */

std::shared_ptr<Checker>& Board::atRef(Square sq) {
	if (sq.isOutside())
		throw std::invalid_argument("square is outside of the board");
	if (!sq.isBlack())
		throw std::invalid_argument("square is not black");

	return board[sq.row()/2][sq.col()];
}

std::shared_ptr<Checker> Board::at(Square sq) {
	try {
		return atRef(sq);
	}
	catch (std::invalid_argument e) {
		return nullptr;
	}
}

std::vector<std::shared_ptr<Checker>> Board::getPlayersCheckers(Player::Side side, bool includeOutside) {
	std::vector<std::shared_ptr<Checker>> out;

	for (int row = 0; row < BoardConst::SIZE; row++) {
		for (int col = row % 2; col < BoardConst::SIZE; col += 2) {
			auto c = at(Square(col, row));
			if (c && c->getSide() == side) {
				out.push_back(c);
			}
		}
	}

	if (includeOutside) {
		for (auto c: outside) {
			if (c->getSide() == side) {
				out.push_back(c);
			}
		}
	}

	return out;
}

bool Board::isCheckersLastRow(Square sq) {
	auto checker = at(sq);
	if (checker == nullptr)
		throw std::invalid_argument("square is empty");

	int lastRow = checker->getSide() == Player::PLAYER_1 ? BoardConst::SIZE-1 : 0;
	return sq.row() == lastRow;
}

std::shared_ptr<Checker> Board::moveInternal(Square from, Square to) {
	std::shared_ptr<Checker> c = at(from);

	if (c == nullptr)
		throw std::invalid_argument("origin square is empty");
	if (at(to) != nullptr)
		throw std::invalid_argument("destination square is not empty");

	if (to.isOutside()) {
		outside.push_back(c);
	} else {
		atRef(to) = c;
	}

	atRef(from) = nullptr;
	version++;
	return c;
}

void Board::move(Square from, Square to) {
	moveInternal(from, to)->moveTo(to);
}

std::shared_ptr<VirtualBoard> Board::virtualCopy() {
	return std::make_shared<VirtualBoard>(this);
}

int Board::getVersion() {
	return version;
}

std::array<Board::DiagonalInfo, 4> Board::getDiagonalItems(Square from) {
	std::array<Board::DiagonalInfo, 4> out;
	Square::Offset direction(1, 1);

	if (at(from) != nullptr) {
		if (at(from)->getSide() == Player::PLAYER_2) {
			direction = Square::Offset(-1, -1);
		}
	}

	for (int i = 0; i < 4; i++) {
		out[i].direction = direction;

		for (Square sq = from + direction; !sq.isOutside(); sq = sq + direction)
			out[i].checkers.push_back(at(sq));

		/* Kierunki mają być iterowane następująco (w przypadku pionów białych):
		 *   1. Square::Offset( 1,  1)
		 *   2. Square::Offset(-1,  1)  (w porównaniu do poprzedniego zmieniony znak kolumny)
		 *   3. Square::Offset(-1, -1)  (w porównaniu do poprzedniego zmieniony znak rzędu)
		 *   4. Square::Offset( 1, -1)  (w porównaniu do poprzedniego zmieniony znak kolumny)
		 * Widać ewidentnie, że zmiany znaku następują na przemian, dlatego stosowany jest iterator
		 */
		direction[i%2] = -direction[i%2];
	}

	return out;
}

std::ostream& operator<<(std::ostream& stream, Board* b) {
	for (int row = BoardConst::SIZE-1; row >= 0; row--) {
		for (int col = 0; col < BoardConst::SIZE; col++) {
			std::shared_ptr<Checker> c = b->at(Square(col, row));
			if (c == nullptr)
				stream << "-- ";
			else if (c->getSide() == Player::PLAYER_1)
				stream << "WT ";
			else
				stream << "BK ";
		}

		stream << '\n';
	}
	return stream;
}