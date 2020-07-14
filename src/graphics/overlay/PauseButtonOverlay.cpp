#include "inc/graphics/overlay/PauseButtonOverlay.hpp"
#include "inc/graphics/MainWindow.hpp"

rl::Texture PauseButtonOverlay::pause = { 0 };
rl::Texture PauseButtonOverlay::pauseSelected = { 0 };
std::vector<rl::Rectangle> PauseButtonOverlay::clicky({
	{ 0, 0, 64, 54 }  // the button itself
});

// old overlay rectangle: { 884, 12, 64, 54 }

PauseButtonOverlay::PauseButtonOverlay():
	Overlay(
		{ 64, 54 },
		{
			(884.0f + 64.0f/2.0f) / MainWindow::winWidth,
			( 12.0f + 54.0f/2.0f) / MainWindow::winHeight
		},
		54.0f / MainWindow::winHeight,
		clicky,
		Animation::BACKGROUND
	)
{
	if (pause.width == 0) {
		pause = rl::LoadTexture("resources/images/menu/pause.png");
		pauseSelected = rl::LoadTexture("resources/images/menu/pause_selected.png");

		rl::SetTextureFilter(pause, rl::FILTER_BILINEAR);
		rl::SetTextureFilter(pauseSelected, rl::FILTER_BILINEAR);
	}
}

void PauseButtonOverlay::update() {
	Overlay::update();

	if (isVisible()) {
		if (rl::IsMouseButtonPressed(rl::MOUSE_LEFT_BUTTON) && selectedClicky == 0) {
			selectedClicky = -1;
			MainWindow::getInstance()->getPauseMenu()->setHidden(false);
		}
	}
}

void PauseButtonOverlay::drawOverlay() {
	rl::DrawTextureV(selectedClicky == 0 ? pauseSelected : pause, { clicky[0].x, clicky[0].y }, rl::WHITE);
}