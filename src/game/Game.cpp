#include "inc/game/Game.hpp"

Game::Game(Board* board, Player* player1, Player* player2, Player::Side firstTurn):
	board(board), turn(firstTurn), winner(nullptr), drawCounter(0), gameFinished(false)
{
	players[0] = player1;
	players[1] = player2;

	newSequence();
	moveRequestDelayed = true;
}

Game::~Game() {
	delete players[0];
	delete players[1];
}

void Game::newSequence() {
	currentSequence = std::make_shared<PlayerMoveSequence>(board, turn);

	if (currentSequence->getOriginsForNextMove().size() == 0) {
		gameFinished = true;
		winner = players[Player::opposite(turn)];
	}
}

bool Game::updateDrawCounter() {
	if (currentSequence->capturedCount()) {
		drawCounter = 0;
		return false;
	} else {
		drawCounter++;
		return drawCounter == drawIn;
	}
}

void Game::moveResponse(std::shared_ptr<PlayerMoveSequence> sequence) {
	if (sequence != currentSequence) {
		throw std::invalid_argument("invalid response sequence");
	}

	sequence->apply(board);

	if (sequence->isFinished()) {
		if (updateDrawCounter()) {
			gameFinished = true;
		} else {
			turn = Player::opposite(turn);
			newSequence();
		}
	}

	if (!gameFinished) {
		moveRequestDelayed = true;
	}
}

void Game::update() {
	if (moveRequestDelayed) {
		moveRequestDelayed = false;
		players[turn]->requestMove(this, board, currentSequence);
	}
}

bool Game::isGameFinished() {
	return gameFinished;
}

Player* Game::getWinner() {
	return winner;
}