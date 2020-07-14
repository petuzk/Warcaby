#include "inc/graphics/overlay/Overlay.hpp"

Overlay::OverlayAnimation::OverlayAnimation(Overlay* overlay, bool hide, Animation::State animState):
	Animation(milliseconds(800), false, animState), overlay(overlay), hide(hide) { }

float Overlay::OverlayAnimation::getProgress(float linearProgress) {
	// https://easings.net/en#easeInOutQuart
	float x = linearProgress;
	return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
}

void Overlay::OverlayAnimation::animate(float progress) {
	progress = hide ? -progress : progress - 1.0f;  // 0 -> -1 or -1 -> 0
	overlay->setCameraOffset({ 0, progress });
}



Overlay::Overlay(
	rl::Vector2 size, rl::Vector2 pos, float scale,
	const std::vector<rl::Rectangle>& clicky, Animation::State animState):
	Renderable(Renderable::T2D), Updatable(Priority::POverlay),
	size(size), pos(pos), cameraAddOffset({ 0, -1 }), scale(scale),
	clicky(clicky), animState(animState), hidden(true), transitioning(false)
{
	setupCamera();
}

void Overlay::setupCamera() {
	// some helpful variables
	int screenWidth = rl::GetScreenWidth(), screenHeight = rl::GetScreenHeight();
	float centerX = size.x / 2.0f, centerY = size.y / 2.0f;

	// set basic values
	camera.rotation = 0.0f;
	camera.target = { centerX, centerY };

	// calculate camera zoom depending on aspect ratios of menu and screen
	// it is actually a scale factor between menu coordinates and screen coordinates
	if (size.x / size.y > (float) screenWidth / screenHeight) {
		camera.zoom = scale * screenWidth / size.x;
	} else {
		camera.zoom = scale * screenHeight / size.y;
	}

	// calculate offsets
	cameraBaseOffset = {
		centerX * camera.zoom + (screenWidth  - size.x * camera.zoom) * pos.x,
		centerY * camera.zoom + (screenHeight - size.y * camera.zoom) * pos.y
	};

	setCameraOffset(cameraAddOffset);
}

rl::Vector2 Overlay::getSize() {
	return size;
}

rl::Vector2 Overlay::getPos() {
	return pos;
}

float Overlay::getScale() {
	return scale;
}

bool Overlay::isVisible() {
	return !hidden || transitioning;
}

void Overlay::setHidden(bool h) {
	if (hidden != h) {
		hidden = h;
		transitioning = true;
		OverlayAnimation* anim = new OverlayAnimation(this, h, animState);
		anim->onEnd([&] { transitioning = false; });
	}
}

void Overlay::setCameraOffset(rl::Vector2 offset) {
	float linScaleX = offset.x > 0 ? pos.x : 1 - pos.x;
	float linScaleY = offset.y > 0 ? pos.y : 1 - pos.y;
	int screenWidth = rl::GetScreenWidth(), screenHeight = rl::GetScreenHeight();

	cameraAddOffset = offset;
	offset = {
		cameraAddOffset.x * (screenWidth  - linScaleX * (screenWidth  - size.x * camera.zoom)),
		cameraAddOffset.y * (screenHeight - linScaleY * (screenHeight - size.y * camera.zoom))
	};
	camera.offset = rl::Vector2Add(cameraBaseOffset, offset);
}

void Overlay::draw() {
	if (isVisible()) {
		rl::BeginMode2D(camera);
		drawOverlay();
		rl::EndMode2D();
	}
}

void Overlay::update() {
	if (rl::IsWindowResized()) {
		setupCamera();
	}

	if (isVisible()) {
		rl::Vector2 mouse_pos = rl::GetScreenToWorld2D(rl::GetMousePosition(), camera);
		selectedClicky = -1;

		for (int i = 0; i < clicky.size(); i++) {
			if (rl::CheckCollisionPointRec(mouse_pos, clicky[i])) {
				selectedClicky = i;
				break;
			}
		}
	}
}