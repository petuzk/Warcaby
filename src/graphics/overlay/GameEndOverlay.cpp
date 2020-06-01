#include "inc/graphics/overlay/GameEndOverlay.hpp"
#include "inc/graphics/MainWindow.hpp"

std::vector<rl::Rectangle> GameEndOverlay::clickables({
	{ 302, 490, 354, 32 }  // powrót do menu
});

GameEndOverlay::GameEndOverlay(): MenuOverlay(clickables) { }

void GameEndOverlay::show(Player* winner) {
	if (winner != nullptr) {
		message = "wygrał gracz nr x";
		message.back() = winner->getSide() == Player::PLAYER_1 ? '1' : '2';
	} else {
		// aby nie wprowadzać dodatkowej zmiennej,
		// wykorzystuję fakt, że czcionka jest o stałej szerokości znaków
		message = "      remis      ";
	}

	setHidden(false);
}

void GameEndOverlay::update() {
	if (isHidden()) {
		return;
	}

	Overlay::update();

	if (rl::IsMouseButtonPressed(rl::MOUSE_LEFT_BUTTON) && hovered == 0) {
		hovered = -1;
		setHidden(true, true);
		MainWindow::getInstance()->getMainMenu()->setHidden(false, true);
	}
}

void GameEndOverlay::draw() {
	drawBg();

	rl::DrawTextEx(menuFont, message.c_str(),  getPos({ 260, 269 }), fontSize, fontSpacing, fontColor);
	rl::DrawTextEx(menuFont, "powrót do menu", getPos({ 299, 478 }), fontSize, fontSpacing, hovered == 0 ? fontSelectedColor : fontColor);

	rl::DrawTextureV(menuCircle, getPos({ 467, 389 }), rl::WHITE);
}