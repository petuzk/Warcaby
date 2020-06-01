#include "inc/graphics/primitives/Updatable.hpp"
#include "inc/graphics/primitives/Updater.hpp"

Updatable::Updatable(int priority): priority(priority) {
	Updater::getInstance()->addUpdatable(this);
}

Updatable::~Updatable() {
	Updater::getInstance()->removeUpdatable(this);
}