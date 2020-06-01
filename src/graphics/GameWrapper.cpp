#include "inc/graphics/GameWrapper.hpp"
#include "inc/graphics/rendering/Camera.hpp"
#include "inc/graphics/MainWindow.hpp"

GameWrapper::GameWrapper(Board* board, Player* player1, Player* player2, Player::Side firstTurn, Player::Side cameraSide):
	Updatable(Priority::PGameWrapper), Game(board, player1, player2, firstTurn), cameraSide(cameraSide), gameStarted(false) { }

void GameWrapper::update() {
	if (!gameStarted) {
		Camera::getInstance()->moveToSide(cameraSide);
		gameStarted = true;
	}
	Game::update();
	if (isGameFinished()) {
		MainWindow::getInstance()->getGameEndMenu()->show(getWinner());
		MainWindow::getInstance()->stopGame();
	}
}