#pragma once

#include "inc/graphics/overlay/Overlay.hpp"

class MenuOverlay: public Overlay {
protected:
	static constexpr float fontSize = 56.0f;
	static constexpr float fontSpacing = 0.0f;
	static const rl::Color fontColor;
	static const rl::Color fontSelectedColor;
	static rl::Texture menuBackground;
	static rl::Texture menuLogo;
	static rl::Texture menuCircle;
	static rl::Font menuFont;

	static void static_init();

	MenuOverlay(const std::vector<rl::Rectangle>& clicky);

	void drawBg();

public:
	void setHidden(bool h, bool menuReplacement = false);
};