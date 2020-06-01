#include "inc/graphics/primitives/Renderable.hpp"
#include "inc/graphics/primitives/Renderer.hpp"

Renderable::Renderable(Renderable::Type type): type(type) {
	Renderer::getInstance()->addRenderable(this);
}

Renderable::~Renderable() {
	Renderer::getInstance()->removeRenderable(this);
}
