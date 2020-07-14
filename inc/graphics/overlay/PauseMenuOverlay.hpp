#pragma once

#include "inc/graphics/overlay/MenuOverlay.hpp"

class PauseMenuOverlay: public MenuOverlay {
	static std::vector<rl::Rectangle> clicky;

public:
	PauseMenuOverlay();

	void drawOverlay();
	void update();
};