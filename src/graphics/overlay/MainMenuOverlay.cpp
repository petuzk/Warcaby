#include "inc/graphics/overlay/MainMenuOverlay.hpp"
#include "inc/graphics/MainWindow.hpp"

#include <iostream>

std::vector<rl::Rectangle> MainMenuOverlay::clickables({
	{ 216, 304, 201, 32 },  // pierwszy gracz - człowiek
	{ 217, 351, 201, 32 },  // pierwszy gracz - komputer
	{ 540, 304, 201, 32 },  // drugi gracz - człowiek
	{ 541, 351, 201, 32 },  // drugi gracz - komputer
	{ 266, 490, 175, 32 },  // zakończ
	{ 524, 490, 170, 32 }   // zamknij
});

MainMenuOverlay::MainMenuOverlay(): MenuOverlay(clickables) {
	playersAreAI[0] = false;
	playersAreAI[1] = true;
}

void MainMenuOverlay::update() {
	if (isHidden()) {
		return;
	}

	Overlay::update();

	if (rl::IsMouseButtonPressed(rl::MOUSE_LEFT_BUTTON)) {
		switch (hovered) {
			case -1:  break;
			case  0:  playersAreAI[0] = false;  break;
			case  1:  playersAreAI[0] = true;   break;
			case  2:  playersAreAI[1] = false;  break;
			case  3:  playersAreAI[1] = true;   break;
			case  4:  MainWindow::getInstance()->close(); break;
			case  5:  MainWindow::getInstance()->startGame(playersAreAI[0], playersAreAI[1]); setHidden(true); break;
		}

		hovered = -1;
	}
}

void MainMenuOverlay::draw() {
	drawBg();

	rl::DrawTextEx(menuFont, "człowiek", getPos({ 213, 292 }), fontSize, fontSpacing, hovered == 0  ? fontSelectedColor : fontColor);
	rl::DrawTextEx(menuFont, "komputer", getPos({ 213, 339 }), fontSize, fontSpacing, hovered == 1 ? fontSelectedColor : fontColor);
	rl::DrawTextEx(menuFont, "vs",       getPos({ 455, 316 }), fontSize, fontSpacing, fontColor);
	rl::DrawTextEx(menuFont, "człowiek", getPos({ 537, 292 }), fontSize, fontSpacing, hovered == 2  ? fontSelectedColor : fontColor);
	rl::DrawTextEx(menuFont, "komputer", getPos({ 537, 339 }), fontSize, fontSpacing, hovered == 3 ? fontSelectedColor : fontColor);
	rl::DrawTextEx(menuFont, "zakończ",  getPos({ 262, 478 }), fontSize, fontSpacing, hovered == 4  ? fontSelectedColor : fontColor);
	rl::DrawTextEx(menuFont, "|",        getPos({ 468, 479 }), fontSize, fontSpacing, fontColor);
	rl::DrawTextEx(menuFont, "zacznij",  getPos({ 520, 478 }), fontSize, fontSpacing, hovered == 5 ? fontSelectedColor : fontColor);

	rl::DrawTextureV(menuCircle, getPos({ 172, static_cast<float>(playersAreAI[0] ? 361 : 315) }), rl::WHITE);
	rl::DrawTextureV(menuCircle, getPos({ 766, static_cast<float>(playersAreAI[1] ? 361 : 315) }), rl::WHITE);
}