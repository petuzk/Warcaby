#pragma once

#include "inc/game/player/PlayerMoveSequence.hpp"

class VirtualBoard;

/* Jak i VirtualBoard, reprezentuje virtualny ciąg ruchów.
 * Może być zastosowany wyłącznie do planszy virtualnej lub
 * przekopiowany do pustego PlayerMoveSequence.
 */

class VirtualPlayerMoveSequence: public PlayerMoveSequence {
public:
	using PlayerMoveSequence::PlayerMoveSequence;
	void apply(std::shared_ptr<VirtualBoard> vboard);
	void addInto(std::shared_ptr<PlayerMoveSequence> s);
};