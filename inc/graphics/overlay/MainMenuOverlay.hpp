#pragma once

#include "inc/graphics/overlay/MenuOverlay.hpp"

class MainMenuOverlay: public MenuOverlay {
	static std::vector<rl::Rectangle> clicky;

	bool playersAreAI[2];

public:
	MainMenuOverlay();

	void drawOverlay();
	void update();
};