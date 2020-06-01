#pragma once

#include <vector>
#include <stdexcept>
#include "inc/graphics/primitives/Renderable.hpp"

class Renderer {
	static Renderer* instance;

protected:
	std::vector<Renderable*> renderables;

	Renderer();
	void drawAll(Renderable::Type type);

public:
	static Renderer* getInstance();

	virtual void addRenderable(Renderable* r);
	virtual void removeRenderable(Renderable* r);
};