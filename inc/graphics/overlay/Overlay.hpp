#pragma once

#include <vector>
#include "inc/graphics/primitives/Animation.hpp"
#include "inc/graphics/primitives/Updatable.hpp"
#include "inc/graphics/primitives/Renderable.hpp"

#include <math.h>  // to avoid errors when including raymath inside namespace
namespace rl {
	extern "C" {
		#include "raylib.h"
		#include "raymath.h"
	}
}

/* Base class for any interface overlay.
 * In order to make window resizeable, this class creates 2D scene with its own
 * coordinate system and uses Camera2D to render the scene onto the screen.
 * As a result, resizing the window does not affect scene's coordinate system but affects
 * its rendered size.
 *
 * The size of the scene is defined by an aspect ratio and a scale relative to the screen.
 * The position of the scene is specified by two normalized cartesian coordinates (from 0 to 1).
 * It is also possible to move the scene on the screen by moving the camera. It is used
 * by the OverlayAnimation to animate appearing/disappearing of an overlay.
 *
 * Overriden Renderable::draw() method is final and is used to setup 2D scene.
 * It calls drawOverlay(), which you have to define in a derived class to draw the scene.
 */

class Overlay: public Renderable, public Updatable {
	class OverlayAnimation: public Animation {
		Overlay* overlay;
		bool hide;

		float getProgress(float linearProgress);
		void animate(float progress);

	public:
		OverlayAnimation(Overlay* overlay, bool hide, Animation::State animState);
	};

	rl::Vector2 size, pos;
	float scale;

	rl::Camera2D camera;
	rl::Vector2 cameraBaseOffset, cameraAddOffset;

	Animation::State animState;

	const std::vector<rl::Rectangle>& clicky;

	bool hidden, transitioning;

	void setupCamera();

protected:
	int selectedClicky;

	/* Constructs Overlay.
	 * Parameters:
	 *   size      -- overlay size in abstract units. Determines aspect ratio
	 *                and coordinate system used in rendering.
	 *   pos       -- overlay position in rendered screen.
	 *                Ranges from 0.0 (stick to left or top) to 1.0 (right or bottom).
	 *   scale     -- overlay width will not exceed `scale * screen_width`
	 *                and height will not exceed `scale * screen_height`.
	 *   clicky    -- vector of selectable areas. Index of the currently selected area
	 *                will be stored in `selectedClicky` (or -1 if nothing is selected).
	 *                If some rectangles overlap, the first one will be selected.
	 *   animState -- type of overlay appearing/disappearing animation
	 */

	Overlay(rl::Vector2 size, rl::Vector2 pos, float scale,
		const std::vector<rl::Rectangle>& clicky, Animation::State animState = Animation::FOREGROUND);

	virtual void drawOverlay() = 0;

public:
	rl::Vector2 getSize();
	rl::Vector2 getPos();
	float getScale();
	bool isVisible();

	void setHidden(bool h);
	void setCameraOffset(rl::Vector2 offset);

	void draw() final;
	void update();
};