#include "inc/graphics/overlay/MenuOverlay.hpp"
#include "inc/graphics/MainWindow.hpp"

rl::Texture MenuOverlay::menuBackground = { 0 };
rl::Texture MenuOverlay::menuLogo = { 0 };
rl::Texture MenuOverlay::menuCircle = { 0 };
rl::Font MenuOverlay::menuFont = { 0 };
const rl::Color MenuOverlay::fontColor = { 255, 255, 229, 255 };
const rl::Color MenuOverlay::fontSelectedColor = { 186, 136, 75, 255 };

MenuOverlay::MenuOverlay(const std::vector<rl::Rectangle>& clickables): Overlay({ 122, 59, 720, 525 }, clickables) {
	if (menuBackground.width == 0) {
		menuBackground = rl::LoadTexture("resources/images/menu/bg.png");
		menuLogo = rl::LoadTexture("resources/images/menu/logo.png");
		menuCircle = rl::LoadTexture("resources/images/menu/circle.png");
		menuFont = rl::LoadFont("resources/fonts/DMMono-Regular-56.fnt");
	}
}

void MenuOverlay::drawBg() {
	rl::DrawTextureV(menuBackground, getPos({ 122, 215 }), rl::WHITE);
	rl::DrawTextureV(menuLogo, getPos({ 227, 59 }), rl::WHITE);
}

void MenuOverlay::setHidden(bool h, bool menuReplacement) {
	Overlay::setHidden(h);
	MainWindow::getInstance()->getBoard()->setInputEnabled(h);
	if (!menuReplacement) {
		// nie zaczynaj animacji przesunięcia przycisku jeżeli za chwilę pojawi się kolejne menu
		MainWindow::getInstance()->getPauseButton()->setHidden(!h);
	}
}