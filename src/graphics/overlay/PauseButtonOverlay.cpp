#include "inc/graphics/overlay/PauseButtonOverlay.hpp"
#include "inc/graphics/MainWindow.hpp"

rl::Texture PauseButtonOverlay::pause = { 0 };
rl::Texture PauseButtonOverlay::pauseSelected = { 0 };
std::vector<rl::Rectangle> PauseButtonOverlay::clickables({
	{ 884, 12, 64, 54 }  // przycisk
});

PauseButtonOverlay::PauseButtonOverlay(): Overlay(clickables[0], clickables, Animation::BACKGROUND) {
	if (pause.width == 0) {
		pause = rl::LoadTexture("resources/images/menu/pause.png");
		pauseSelected = rl::LoadTexture("resources/images/menu/pause_selected.png");
	}
}

void PauseButtonOverlay::update() {
	if (isHidden()) {
		return;
	}

	Overlay::update();

	if (rl::IsMouseButtonPressed(rl::MOUSE_LEFT_BUTTON) && hovered == 0) {
		hovered = -1;
		MainWindow::getInstance()->getPauseMenu()->setHidden(false);
	}
}

void PauseButtonOverlay::draw() {
	rl::DrawTextureV(hovered == 0 ? pauseSelected : pause, getPos({ clickables[0].x, clickables[0].y }), rl::WHITE);
}