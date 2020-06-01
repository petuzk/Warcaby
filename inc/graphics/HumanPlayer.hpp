#pragma once

#include "inc/game/player/Player.hpp"

class HumanPlayer: public Player {
	void requestMove();

public:
	using Player::Player;
	using Player::respond;
};