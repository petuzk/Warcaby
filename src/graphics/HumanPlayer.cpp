#include "inc/graphics/HumanPlayer.hpp"
#include "inc/graphics/MainWindow.hpp"

void HumanPlayer::requestMove() {
	MainWindow::getInstance()->getBoard()->requestMoveFor(this, sequence);
}