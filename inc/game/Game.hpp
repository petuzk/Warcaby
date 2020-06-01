#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include "inc/game/board/Board.hpp"
#include "inc/game/player/Player.hpp"
#include "inc/game/player/PlayerMove.hpp"
#include "inc/game/player/PlayerMoveSequence.hpp"

/* Zarządza graczami.
 */

class Game {
	Board* board;
	Player* players[2];
	Player* winner;
	Player::Side turn;
	std::shared_ptr<PlayerMoveSequence> currentSequence;

	int drawCounter;
	static constexpr int drawIn = 30;

	bool gameFinished;
	bool moveRequestDelayed;

	void newSequence();
	bool updateDrawCounter();

public:
	Game(Board* board, Player* player1, Player* player2, Player::Side firstTurn = Player::PLAYER_1);
	virtual ~Game();

	void moveResponse(std::shared_ptr<PlayerMoveSequence> sequence);
	void update();

	bool isGameFinished();
	Player* getWinner();
};