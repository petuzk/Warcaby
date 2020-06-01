#include "inc/graphics/overlay/PauseMenuOverlay.hpp"
#include "inc/graphics/MainWindow.hpp"

std::vector<rl::Rectangle> PauseMenuOverlay::clickables({
	{ 313, 281, 331, 32 }, // powrót do gry
	{ 302, 490, 354, 32 }  // powrót do menu
});

PauseMenuOverlay::PauseMenuOverlay(): MenuOverlay(clickables) { }

void PauseMenuOverlay::update() {
	if (isHidden()) {
		return;
	}

	Overlay::update();

	if (rl::IsMouseButtonPressed(rl::MOUSE_LEFT_BUTTON)) {
		if (hovered >= 0) {
			bool menuReplacement = hovered == 1;
			hovered = -1;

			setHidden(true, menuReplacement);

			if (menuReplacement) {
				MainWindow::getInstance()->stopGame();
				MainWindow::getInstance()->getMainMenu()->setHidden(false, menuReplacement);
			}
		}
	}
}

void PauseMenuOverlay::draw() {
	drawBg();

	rl::DrawTextEx(menuFont, "powrót do gry",  getPos({ 310, 269 }), fontSize, fontSpacing, hovered == 0 ? fontSelectedColor : fontColor);
	rl::DrawTextEx(menuFont, "powrót do menu", getPos({ 299, 478 }), fontSize, fontSpacing, hovered == 1 ? fontSelectedColor : fontColor);

	rl::DrawTextureV(menuCircle, getPos({ 467, 389 }), rl::WHITE);
}