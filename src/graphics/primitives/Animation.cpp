#include "inc/graphics/primitives/Animation.hpp"
#include "inc/graphics/primitives/Animator.hpp"

Animation::Animation(milliseconds duration, bool parallelToPrev, Animation::State state):
	started(false), duration(duration), state(state)
{
	Animator::getInstance()->addAnimation(this, parallelToPrev);
}

Animation::~Animation() {
	try {
		Animator::getInstance()->removeAnimation(this);
	}
	catch (std::invalid_argument e) { }
}

float Animation::getProgress(float linearProgress) {
	return linearProgress;
}

void Animation::animate(float progress) {
	throw std::runtime_error("not implemented");
}

Animation::State Animation::getState() {
	return state;
}

Animation::State Animation::animate() {
	if (state == FINISHED) {
		return FINISHED;
	}

	milliseconds now = time();

	if (!started) {
		started = true;
		startTime = now;
	}

	if (now > startTime + duration) {
		state = FINISHED;
		animate(getProgress(1.0f));
	}
	else {
		float linear = static_cast<float>((now - startTime).count()) / static_cast<float>(duration.count());
		animate(getProgress(linear));
	}

	return state;
}

void Animation::stop() {
	state = FINISHED;
}

milliseconds Animation::time() {
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}

Animation::State operator&&(Animation::State x, Animation::State y) {
	return (x > y) ? x : y;
}