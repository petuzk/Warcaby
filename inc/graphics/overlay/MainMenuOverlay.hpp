#pragma once

#include "inc/graphics/overlay/MenuOverlay.hpp"

class MainMenuOverlay: public MenuOverlay {
	static std::vector<rl::Rectangle> clickables;

	bool playersAreAI[2];

public:
	MainMenuOverlay();

	void draw();
	void update();
};