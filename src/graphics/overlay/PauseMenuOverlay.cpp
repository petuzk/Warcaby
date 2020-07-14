#include "inc/graphics/overlay/PauseMenuOverlay.hpp"
#include "inc/graphics/MainWindow.hpp"

std::vector<rl::Rectangle> PauseMenuOverlay::clicky({
	{ 313-122, 281-59, 331, 32 }, // back to game
	{ 302-122, 490-59, 354, 32 }  // back to menu
});

PauseMenuOverlay::PauseMenuOverlay(): MenuOverlay(clicky) { }

void PauseMenuOverlay::update() {
	Overlay::update();

	if (isVisible()) {
		if (rl::IsMouseButtonPressed(rl::MOUSE_LEFT_BUTTON)) {
			if (selectedClicky >= 0) {
				bool backToMenu = selectedClicky == 1;
				selectedClicky = -1;

				setHidden(true, backToMenu);

				if (backToMenu) {
					MainWindow::getInstance()->stopGame();
					MainWindow::getInstance()->getMainMenu()->setHidden(false, true);
				}
			}
		}
	}
}

void PauseMenuOverlay::drawOverlay() {
	drawBg();

	rl::DrawTextEx(menuFont, "powrót do gry",  { 310-122, 269-59 }, fontSize, fontSpacing, selectedClicky == 0 ? fontSelectedColor : fontColor);
	rl::DrawTextEx(menuFont, "powrót do menu", { 299-122, 478-59 }, fontSize, fontSpacing, selectedClicky == 1 ? fontSelectedColor : fontColor);

	rl::DrawTextureV(menuCircle, { 467-122, 389-59 }, rl::WHITE);
}