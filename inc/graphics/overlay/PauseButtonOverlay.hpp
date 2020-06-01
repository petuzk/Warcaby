#pragma once

#include "inc/graphics/overlay/MenuOverlay.hpp"

class PauseButtonOverlay: public Overlay {
	static rl::Texture pause;
	static rl::Texture pauseSelected;
	static std::vector<rl::Rectangle> clickables;

public:
	PauseButtonOverlay();

	void draw();
	void update();
};