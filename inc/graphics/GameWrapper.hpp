#pragma once

#include "inc/game/Game.hpp"
#include "inc/graphics/primitives/Updatable.hpp"

class GameWrapper: public Updatable, public Game {
	bool gameStarted;
	Player::Side cameraSide;

public:
	GameWrapper(Board* board, Player* player1, Player* player2, Player::Side firstTurn, Player::Side cameraSide);
	void update();
};