#pragma once

#include <string>
#include "inc/graphics/overlay/MenuOverlay.hpp"
#include "inc/game/player/Player.hpp"

class GameEndOverlay: public MenuOverlay {
	static std::vector<rl::Rectangle> clickables;

	std::string message;

public:
	GameEndOverlay();

	void show(Player* winner);

	void draw();
	void update();
};