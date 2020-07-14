#pragma once

#include <chrono>
#include <functional>

using namespace std::chrono;

class Animation {
public:
	enum State {
		FINISHED,
		BACKGROUND,
		FOREGROUND
	};

	typedef std::function<void()> onEndCallbackType;

protected:
	State state;
	bool started;
	milliseconds duration;
	milliseconds startTime;
	onEndCallbackType onEndCallback;

	static milliseconds time();

	Animation(milliseconds duration, bool parallelToPrev = false, State state = FOREGROUND);

	virtual float getProgress(float linearProgress);
	virtual void animate(float progress);

public:
	virtual ~Animation();

	State getState();
	virtual State animate();

	virtual void onEnd(onEndCallbackType callback);

	void stop();
};

Animation::State operator&&(Animation::State x, Animation::State y);