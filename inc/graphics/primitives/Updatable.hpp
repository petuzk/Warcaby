#pragma once

#include "inc/graphics/primitives/PrioritySettings.hpp"

class Updatable {
protected:
	Updatable(int priority);

public:
	virtual ~Updatable();

	const int priority;

	virtual void update() = 0;
};