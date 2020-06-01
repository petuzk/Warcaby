#include "inc/graphics/overlay/Overlay.hpp"

Overlay::OverlayAnimation::OverlayAnimation(Overlay* overlay, bool hide, Animation::State animationState):
	Animation(milliseconds(800), false, animationState), overlay(overlay), hide(hide) { }

float Overlay::OverlayAnimation::getProgress(float linearProgress) {
	// https://easings.net/en#easeInOutQuart
	float x = linearProgress;
	return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
}

void Overlay::OverlayAnimation::animate(float progress) {
	progress = hide ? -progress : progress - 1.0f;  // 0 -> -1 lub -1 -> 0
	overlay->rectOffset.y = progress * (overlay->overlayRectangle.y + overlay->overlayRectangle.height);
}



Overlay::Overlay(
	rl::Rectangle overlayRectangle, const std::vector<rl::Rectangle>& clickables, Animation::State animationState):
	Renderable(Renderable::T2D), Updatable(Priority::POverlay), overlayRectangle(overlayRectangle),
	clickables(clickables), animationState(animationState),
	rectOffset({ 0.0f, -(overlayRectangle.y + overlayRectangle.height) }), hidden(true) { }

rl::Vector2 Overlay::getPos(rl::Vector2 relative) {
	return rl::Vector2Add(relative, rectOffset);
}

rl::Rectangle Overlay::getRect(rl::Rectangle relative) {
	return { relative.x + rectOffset.x, relative.y + rectOffset.y, relative.width, relative.height };
}

void Overlay::update() {
	if (hidden) {
		return;
	}

	rl::Vector2 mouse = rl::GetMousePosition();
	hovered = -1;

	for (int i = 0; i < clickables.size(); i++) {
		if (rl::CheckCollisionPointRec(mouse, getRect(clickables[i]))) {
			hovered = i;
			break;
		}
	}
}

bool Overlay::isHidden() {
	return hidden;
}

void Overlay::setHidden(bool h) {
	if (hidden != h) {
		hidden = h;
		new OverlayAnimation(this, h, animationState);
	}
}