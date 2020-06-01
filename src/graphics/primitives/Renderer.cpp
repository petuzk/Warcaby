#include "inc/graphics/primitives/Renderer.hpp"

Renderer* Renderer::instance = nullptr;

Renderer::Renderer() {
	instance = this;
}

Renderer* Renderer::getInstance() {
	return Renderer::instance;
}

void Renderer::addRenderable(Renderable* r) {
	renderables.push_back(r);
}

void Renderer::removeRenderable(Renderable* r) {
	auto pos = std::find(std::begin(renderables), std::end(renderables), r);
	if (pos != std::end(renderables)) {
		renderables.erase(pos);
	} else {
		throw std::invalid_argument("removing renderable which is not in the list");
	}
}

void Renderer::drawAll(Renderable::Type type) {
	for (Renderable* r: renderables) {
		if (r->type == type) {
			r->draw();
		}
	}
}