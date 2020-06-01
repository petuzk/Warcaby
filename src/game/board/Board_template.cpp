#include "inc/game/board/Board.hpp"

/* Oddzielny plik pozwala na #include kodu źródłowego
 */

template <typename CheckerManImpl>
void Board::reset() {
	erase();
	for (int row = 0; row < BoardConst::SIZE; row++) {
		if (row == BoardConst::START_ROWS)
			row = BoardConst::SIZE - BoardConst::START_ROWS;

		for (int col = row % 2; col < BoardConst::SIZE; col += 2) {
			Square sq(col, row);
			atRef(sq) = std::make_shared<CheckerManImpl>(sq, row < BoardConst::START_ROWS ? Player::PLAYER_1 : Player::PLAYER_2);
		}
	}
}