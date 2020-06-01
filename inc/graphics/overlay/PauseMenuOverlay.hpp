#pragma once

#include "inc/graphics/overlay/MenuOverlay.hpp"

class PauseMenuOverlay: public MenuOverlay {
	static std::vector<rl::Rectangle> clickables;

public:
	PauseMenuOverlay();

	void draw();
	void update();
};