#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>
#include "inc/graphics/primitives/Updatable.hpp"

class Updater {
	static Updater* instance;

	std::vector<Updatable*> updatables;

protected:
	Updater();
	void updateAll(bool skipNegativePriority);

public:
	static Updater* getInstance();

	virtual void addUpdatable(Updatable* u);
	virtual void removeUpdatable(Updatable* u);
};