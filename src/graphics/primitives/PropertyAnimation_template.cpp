#include "inc/graphics/primitives/PropertyAnimation.hpp"

template <typename T>
PropertyAnimation<T>::PropertyAnimation(milliseconds duration, T from, T to, bool parallelToPrev, State state):
	Animation(duration, parallelToPrev, state), from(from), to(to) { }

template <typename T>
void PropertyAnimation<T>::animate(float progress) {
	updateProperty((to - from) * progress + from);
}
