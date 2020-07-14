#pragma once

#include <string>
#include "inc/graphics/overlay/MenuOverlay.hpp"
#include "inc/game/player/Player.hpp"

class GameEndOverlay: public MenuOverlay {
	static std::vector<rl::Rectangle> clicky;

	std::string message;

public:
	GameEndOverlay();

	void show(Player* winner);

	void drawOverlay();
	void update();
};