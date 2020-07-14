#include "inc/graphics/overlay/GameEndOverlay.hpp"
#include "inc/graphics/MainWindow.hpp"

std::vector<rl::Rectangle> GameEndOverlay::clicky({
	{ 302-122, 490-59, 354, 32 }  // back to menu
});

GameEndOverlay::GameEndOverlay(): MenuOverlay(clicky) { }

void GameEndOverlay::show(Player* winner) {
	if (winner != nullptr) {
		message = "wygrał gracz nr x";
		message.back() = winner->getSide() == Player::PLAYER_1 ? '1' : '2';
	} else {
		// font is monospace
		message = "      remis      ";
	}

	setHidden(false);
}

void GameEndOverlay::update() {
	Overlay::update();

	if (isVisible()) {
		if (rl::IsMouseButtonPressed(rl::MOUSE_LEFT_BUTTON) && selectedClicky == 0) {
			selectedClicky = -1;
			setHidden(true, true);
			MainWindow::getInstance()->getMainMenu()->setHidden(false, true);
		}
	}
}

void GameEndOverlay::drawOverlay() {
	drawBg();

	rl::DrawTextEx(menuFont, message.c_str(),  { 260-122, 269-59 }, fontSize, fontSpacing, fontColor);
	rl::DrawTextEx(menuFont, "powrót do menu", { 299-122, 478-59 }, fontSize, fontSpacing, selectedClicky == 0 ? fontSelectedColor : fontColor);

	rl::DrawTextureV(menuCircle, { 467-122, 389-59 }, rl::WHITE);
}