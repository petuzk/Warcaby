#pragma once

#include "inc/game/board/Board.hpp"

/* Reprezentuje virtualną planszę, czyli planszę, poruszenie się
 * na której nie zmienia rzeczywistego położenia pionów.
 */

class VirtualBoard: public Board {
public:
	VirtualBoard(Board* board);
};
