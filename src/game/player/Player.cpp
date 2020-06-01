#include "inc/game/player/Player.hpp"
#include "inc/game/Game.hpp"
#include "inc/game/board/Board.hpp"
#include "inc/game/player/PlayerMove.hpp"
#include "inc/game/player/PlayerMoveSequence.hpp"

Player::Side Player::opposite(Player::Side to) {
	return to == PLAYER_1 ? PLAYER_2 : PLAYER_1;
}

Player::Player(Player::Side side): mySide(side) { }

Player::Side Player::getSide() {
	return mySide;
}

void Player::requestMove(Game* game, Board* board, std::shared_ptr<PlayerMoveSequence> sequence) {
	this->game = game;
	this->board = board;
	this->sequence = sequence;

	requestMove();
}

void Player::respond(PlayerMove move) {
	sequence->add(move);
	respond();
}

void Player::respond() {
	game->moveResponse(sequence);
}