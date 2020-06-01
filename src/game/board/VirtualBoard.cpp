#include "inc/game/board/VirtualBoard.hpp"
#include "inc/game/checker/CheckerMan.hpp"
#include "inc/game/checker/CheckerKing.hpp"

VirtualBoard::VirtualBoard(Board* board) {
	version = board->getVersion();
	for (int row = 0; row < BoardConst::SIZE; row++) {
		for (int col = row % 2; col < BoardConst::SIZE; col += 2) {
			Square sq(col, row);
			std::shared_ptr<Checker> c = board->at(sq);

			if (c == nullptr) {
				atRef(sq) = nullptr;
			} else {
				bool isMan;

				try {
					isMan = std::dynamic_pointer_cast<CheckerMan>(c) != nullptr;
				}
				catch (std::bad_cast e) {
					isMan = false;
				}

				if (isMan)
					atRef(sq) = std::make_shared<CheckerMan>(sq, c->getSide());
				else
					atRef(sq) = std::make_shared<CheckerKing>(sq, c->getSide());
			}
		}
	}
}