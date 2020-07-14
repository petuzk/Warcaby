#include "inc/graphics/overlay/MenuOverlay.hpp"
#include "inc/graphics/MainWindow.hpp"

rl::Texture MenuOverlay::menuBackground = { 0 };
rl::Texture MenuOverlay::menuLogo = { 0 };
rl::Texture MenuOverlay::menuCircle = { 0 };
rl::Font MenuOverlay::menuFont = { 0 };
const rl::Color MenuOverlay::fontColor = { 255, 255, 229, 255 };
const rl::Color MenuOverlay::fontSelectedColor = { 186, 136, 75, 255 };

// old overlay rectangle: { 122, 59, 720, 525 }

MenuOverlay::MenuOverlay(const std::vector<rl::Rectangle>& clicky):
	Overlay(
		{ 720, 525 },
		{
			(122.0f + 720.0f/2.0f) / MainWindow::winWidth,
			( 59.0f + 525.0f/2.0f) / MainWindow::winHeight
		},
		525.0f / MainWindow::winHeight,
		clicky
	)
{
	if (menuBackground.width == 0) {
		menuBackground = rl::LoadTexture("resources/images/menu/bg.png");
		menuLogo = rl::LoadTexture("resources/images/menu/logo.png");
		menuCircle = rl::LoadTexture("resources/images/menu/circle.png");
		menuFont = rl::LoadFont("resources/fonts/DMMono-Regular-56.fnt");

		rl::SetTextureFilter(menuBackground, rl::FILTER_BILINEAR);
		rl::SetTextureFilter(menuLogo, rl::FILTER_BILINEAR);
		rl::SetTextureFilter(menuCircle, rl::FILTER_BILINEAR);
		rl::SetTextureFilter(menuFont.texture, rl::FILTER_BILINEAR);
	}
}

void MenuOverlay::drawBg() {
	rl::DrawTextureV(menuBackground, { 122-122, 215-59 }, rl::WHITE);
	rl::DrawTextureV(menuLogo, { 227-122, 59-59 }, rl::WHITE);
}

void MenuOverlay::setHidden(bool h, bool menuReplacement) {
	Overlay::setHidden(h);
	MainWindow::getInstance()->getBoard()->setInputEnabled(h);
	if (!menuReplacement) {
		// animate pause button if no menu will appear in a moment (no replacement)
		MainWindow::getInstance()->getPauseButton()->setHidden(!h);
	}
}