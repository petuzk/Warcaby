#include "inc/graphics/overlay/MainMenuOverlay.hpp"
#include "inc/graphics/MainWindow.hpp"

#include <iostream>

std::vector<rl::Rectangle> MainMenuOverlay::clicky({
	{ 216-122, 304-59, 201, 32 },  // first player - human
	{ 217-122, 351-59, 201, 32 },  // first player - computer
	{ 540-122, 304-59, 201, 32 },  // second player - human
	{ 541-122, 351-59, 201, 32 },  // second player - computer
	{ 266-122, 490-59, 175, 32 },  // exit
	{ 524-122, 490-59, 170, 32 }   // start
});

MainMenuOverlay::MainMenuOverlay(): MenuOverlay(clicky) {
	playersAreAI[0] = false;
	playersAreAI[1] = true;
}

void MainMenuOverlay::update() {
	Overlay::update();

	if (isVisible()) {
		if (rl::IsMouseButtonPressed(rl::MOUSE_LEFT_BUTTON)) {
			switch (selectedClicky) {
				case -1:  break;
				case  0:  playersAreAI[0] = false;  break;
				case  1:  playersAreAI[0] = true;   break;
				case  2:  playersAreAI[1] = false;  break;
				case  3:  playersAreAI[1] = true;   break;
				case  4:  MainWindow::getInstance()->close(); break;
				case  5:  MainWindow::getInstance()->startGame(playersAreAI[0], playersAreAI[1]); setHidden(true); break;
			}

			selectedClicky = -1;
		}
	}
}

void MainMenuOverlay::drawOverlay() {
	drawBg();

	rl::DrawTextEx(menuFont, "człowiek", { 213-122, 292-59 }, fontSize, fontSpacing, selectedClicky == 0  ? fontSelectedColor : fontColor);
	rl::DrawTextEx(menuFont, "komputer", { 213-122, 339-59 }, fontSize, fontSpacing, selectedClicky == 1 ? fontSelectedColor : fontColor);
	rl::DrawTextEx(menuFont, "vs",       { 455-122, 316-59 }, fontSize, fontSpacing, fontColor);
	rl::DrawTextEx(menuFont, "człowiek", { 537-122, 292-59 }, fontSize, fontSpacing, selectedClicky == 2  ? fontSelectedColor : fontColor);
	rl::DrawTextEx(menuFont, "komputer", { 537-122, 339-59 }, fontSize, fontSpacing, selectedClicky == 3 ? fontSelectedColor : fontColor);
	rl::DrawTextEx(menuFont, "zakończ",  { 262-122, 478-59 }, fontSize, fontSpacing, selectedClicky == 4  ? fontSelectedColor : fontColor);
	rl::DrawTextEx(menuFont, "|",        { 468-122, 479-59 }, fontSize, fontSpacing, fontColor);
	rl::DrawTextEx(menuFont, "zacznij",  { 520-122, 478-59 }, fontSize, fontSpacing, selectedClicky == 5 ? fontSelectedColor : fontColor);

	rl::DrawTextureV(menuCircle, { 172-122, static_cast<float>(playersAreAI[0] ? 361 : 315)-59 }, rl::WHITE);
	rl::DrawTextureV(menuCircle, { 766-122, static_cast<float>(playersAreAI[1] ? 361 : 315)-59 }, rl::WHITE);
}