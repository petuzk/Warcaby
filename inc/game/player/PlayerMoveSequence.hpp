#pragma once

#include <vector>
#include <memory>
#include "inc/game/board/Square.hpp"
#include "inc/game/player/Player.hpp"
#include "inc/game/player/PlayerMove.hpp"

class Board;
class VirtualBoard;
class VirtualPlayerMoveSequence;

/* Reprezentuje ciąg ruchów (np. bicie kilku pionów).
 * Sprawdza, czy jest możliwe wykonanie kolejnego ruchu
 * według zasad gry (nie można przeskakiwać więcej niż
 * jeden raz przez ten sam pion). Również konwertuje pion
 * w damkę gdy po ostatnim ruchu pion znajduje się na ostatnim rzędzie.
 */

class PlayerMoveSequence {
protected:
	int applied;
	std::vector<Square> origins;
	std::vector<PlayerMove> sequence;

	// Ograniczenie dostępu do apply()
	friend class Game;
	virtual void apply(Board* board);

public:
	PlayerMoveSequence(Board* board, Player::Side turn);
	PlayerMoveSequence(int applied, std::vector<Square> origins, std::vector<PlayerMove> sequence);
	std::shared_ptr<VirtualPlayerMoveSequence> virtualCopy();

	void add(PlayerMove move);
	bool isMoveAllowed(PlayerMove move);

	std::vector<Square> getOriginsForNextMove();
	bool isOriginForNextMove(Square sq);

	int capturedCount();
	bool isFinished();
	bool isEmpty();
};