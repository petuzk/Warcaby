#pragma once

#include <vector>
#include "inc/graphics/primitives/Animation.hpp"
#include "inc/graphics/primitives/Updatable.hpp"
#include "inc/graphics/primitives/Renderable.hpp"

#include <math.h>  // na potrzebę raymath
namespace rl {
	extern "C" {
		#include "raylib.h"
		#include "raymath.h"
	}
}

class Overlay: public Renderable, public Updatable {
	class OverlayAnimation: public Animation {
		Overlay* overlay;
		bool hide;

		float getProgress(float linearProgress);
		void animate(float progress);

	public:
		OverlayAnimation(Overlay* overlay, bool hide, Animation::State animationState);
	};

	friend class OverlayAnimation;

	Animation::State animationState;
	rl::Rectangle overlayRectangle;
	rl::Vector2 rectOffset;
	bool hidden;
	const std::vector<rl::Rectangle>& clickables;

protected:
	int hovered;

	Overlay(rl::Rectangle overlayRectangle,
		const std::vector<rl::Rectangle>& clickables, Animation::State animationState = Animation::FOREGROUND);

	rl::Vector2 getPos(rl::Vector2 relative);
	rl::Rectangle getRect(rl::Rectangle relative);

public:
	void update();

	bool isHidden();
	void setHidden(bool h);
};