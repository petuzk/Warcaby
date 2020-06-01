#pragma once

#include <vector>
#include <forward_list>
#include <stdexcept>
#include "inc/graphics/primitives/Animation.hpp"

class Animator {
	static Animator* instance;

	class ParallelAnimation: public Animation {
		std::forward_list<Animation*> parallel;

		ParallelAnimation();

	public:
		static bool merge(Animation* newAnim, Animation* prevAnim);
		~ParallelAnimation();

		void addAnimation(Animation* a);
		Animation::State animate();
	};

	std::vector<Animation*> animations;

protected:

	Animator();
	Animation::State animateAll();

public:
	static Animator* getInstance();

	void addAnimation(Animation* a, bool parallelToPrev);
	void removeAnimation(Animation* a);
};