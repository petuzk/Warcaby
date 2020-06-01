#include "inc/graphics/primitives/Updater.hpp"

Updater* Updater::instance = nullptr;

Updater::Updater() {
	instance = this;
}

Updater* Updater::getInstance() {
	return Updater::instance;
}

void Updater::addUpdatable(Updatable* u) {
	// sortowanie rosnąco na bieżąco
	updatables.insert(std::find_if(std::begin(updatables), std::end(updatables), [=](Updatable* t){ return t->priority > u->priority; }), u);
}

void Updater::removeUpdatable(Updatable* u) {
	auto pos = std::find(std::begin(updatables), std::end(updatables), u);
	if (pos != std::end(updatables)) {
		updatables.erase(pos);
	} else {
		throw std::invalid_argument("removing updatable which is not in the list");
	}
}

void Updater::updateAll(bool skipNegativePriority) {
	// for (Updatable* u: updatables) {
	// 	if (u->priority >= 0 || !skipNegativePriority) {
	// 		u->update();
	// 	}
	// }

	for (int i = 0; i < updatables.size(); i++) {
		Updatable* u = updatables[i];
		if (u->priority >= 0 || !skipNegativePriority) {
			u->update();
		}
	}
}