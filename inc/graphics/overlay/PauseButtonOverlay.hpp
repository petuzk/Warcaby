#pragma once

#include "inc/graphics/overlay/MenuOverlay.hpp"

class PauseButtonOverlay: public Overlay {
	static rl::Texture pause;
	static rl::Texture pauseSelected;
	static std::vector<rl::Rectangle> clicky;

public:
	PauseButtonOverlay();

	void drawOverlay();
	void update();
};