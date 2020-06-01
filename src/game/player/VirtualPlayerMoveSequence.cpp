#include "inc/game/player/VirtualPlayerMoveSequence.hpp"
#include "inc/game/board/Board.hpp"
#include "inc/game/board/VirtualBoard.hpp"

void VirtualPlayerMoveSequence::apply(std::shared_ptr<VirtualBoard> vboard) {
	PlayerMoveSequence::apply(vboard.get());
}

void VirtualPlayerMoveSequence::addInto(std::shared_ptr<PlayerMoveSequence> s) {
	if (!s->isEmpty()) {
		throw std::invalid_argument("destination sequence is not empty");
	}

	for (PlayerMove& move: sequence) {
		s->add(move);
	}
}