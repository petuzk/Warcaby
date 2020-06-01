#include "inc/graphics/primitives/Animator.hpp"

Animator* Animator::instance = nullptr;

Animator::ParallelAnimation::ParallelAnimation(): Animation(milliseconds::max()) { }

Animator::ParallelAnimation::~ParallelAnimation() {
	for (Animation* a: parallel) {
		delete a;
	}
}

bool Animator::ParallelAnimation::merge(Animation* newAnim, Animation* prevAnim) {
	bool isNew = false;
	ParallelAnimation* pa;

	try {
		pa = dynamic_cast<ParallelAnimation*>(prevAnim);
	}
	catch (std::bad_cast e) {
		// dynamic_cast może zwrócić nullptr zamiast rzucić wyjątek, dlatego sprawdzam niżej
		pa = nullptr;
	}

	if (pa == nullptr) {
		isNew = true;
		pa = new ParallelAnimation();
		pa->addAnimation(prevAnim);
	}

	pa->addAnimation(newAnim);
	return isNew;
}

void Animator::ParallelAnimation::addAnimation(Animation* a) {
	parallel.push_front(a);
}

Animation::State Animator::ParallelAnimation::animate() {
	state = FINISHED;

	for (Animation* a: parallel) {
		state = state && a->animate();
	}

	return state;
}



Animator::Animator() {
	instance = this;
}

Animator* Animator::getInstance() {
	return Animator::instance;
}

void Animator::addAnimation(Animation* a, bool parallelToPrev) {
	if (a->getState() == Animation::FINISHED) {
		throw std::invalid_argument("trying to add finished animation");
	}
	else if (parallelToPrev) {
		auto size = animations.size();

		if (size == 0) {
			throw std::logic_error("no previous animation");
		}

		Animation* last = animations[size - 1]; animations.pop_back();
		bool newAnimCreated = ParallelAnimation::merge(a, last);

		/* Jeżeli ostatnia animacja nie jest obiektem ParallelAnimation, zostanie utworzony nowy
		 * i automatycznie dodany. Jeżeli jest, nowych obiektów nie powstaje, więc dodajemy z powrotem.
		 * Chyba to by działalo szybciej, niż sprawdzać za każdym razem, czy obiekt już jest dodany.
		 */

		if (!newAnimCreated) {
			animations.push_back(last);
		}
	}
	else {
		animations.push_back(a);
	}
}

void Animator::removeAnimation(Animation* a) {
	auto pos = std::find(std::begin(animations), std::end(animations), a);
	if (pos != std::end(animations)) {
		Animation* a = *pos;
		animations.erase(pos);
		delete a;
	} else {
		throw std::invalid_argument("removing renderable which is not in the list");
	}
}

Animation::State Animator::animateAll() {
	Animation* a;
	Animation::State state, retState = Animation::FINISHED;

	for (int i = 0; i < animations.size(); ) {
		a = animations[i];
		if (a->getState() == Animation::FOREGROUND && retState == Animation::FOREGROUND) {
			i++;
		} else {
			state = a->animate();
			if (state == Animation::FINISHED) {
				removeAnimation(a);
			}
			else {
				retState = retState && state;
				i++;
			}
		}
	}

	return retState;
}