#pragma once

#include "inc/graphics/primitives/Animation.hpp"

template <typename T>
class PropertyAnimation: public Animation {
protected:
	T from, to;

	PropertyAnimation(milliseconds duration, T from, T to, bool parallelToPrev = false, State state = FOREGROUND);
	void animate(float progress);
	virtual void updateProperty(T value) = 0;
};