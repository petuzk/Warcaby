#pragma once

#include <chrono>

using namespace std::chrono;

class Animation {
public:
	enum State {
		FINISHED,
		BACKGROUND,
		FOREGROUND
	};

protected:
	State state;
	bool started;
	milliseconds duration;
	milliseconds startTime;

	static milliseconds time();

	Animation(milliseconds duration, bool parallelToPrev = false, State state = FOREGROUND);

	virtual float getProgress(float linearProgress);
	virtual void animate(float progress);

public:
	virtual ~Animation();

	State getState();
	virtual State animate();

	void stop();
};

Animation::State operator&&(Animation::State x, Animation::State y);