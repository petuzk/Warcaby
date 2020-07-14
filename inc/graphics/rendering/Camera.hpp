#pragma once

#include "inc/game/player/Player.hpp"
#include "inc/graphics/primitives/PropertyAnimation.hpp"
#include "inc/graphics/primitives/Updatable.hpp"
#include "inc/graphics/rendering/ShaderProvider.hpp"

#include <math.h>  // to avoid errors when including raymath inside namespace
namespace rl {
	extern "C" {
		#include "raylib.h"
		#include "raymath.h"
	}
}

class Camera {
private:
	class ZRotationAnimation: public PropertyAnimation<float> {
		Camera* cam;

		float getProgress(float linearProgress);
		void updateProperty(float angle);

	public:
		ZRotationAnimation(float fromAngle, float toAngle, bool parallelToPrev);
	};

	class TargetDistanceAnimation: public PropertyAnimation<float> {
		Camera* cam;

		float getProgress(float linearProgress);
		void updateProperty(float dist);

	public:
		TargetDistanceAnimation(float fromDist, float toDist, Animation::State state = Animation::FOREGROUND);
	};

	class FlyAnimation: public PropertyAnimation<float> {
		Camera* cam;

		void updateProperty(float angle);
		Animation::State animate();

	public:
		FlyAnimation(float startAngle);
	};

	rl::Camera rlCam;

	bool playerView;
	Player::Side playerViewSide;

	float zAngle;
	float distance;
	FlyAnimation* flyAnim;
	ZRotationAnimation* rotAnim;
	TargetDistanceAnimation *distAnim;
	static constexpr float flyDistance = 25.0f;
	static constexpr float playDistance = 12.5f;
	static constexpr float planeAngle = 45.0f;

	static float normalizeAngle(float a);

	Camera();

public:
	Camera(Camera const &)         = delete;
  void operator=(Camera const &) = delete;

	static Camera* getInstance();

	void setZAngle(float angle);
	void setDistance(float dist);
	void recalculatePos();

	void begin3D();
	void end3D();

	void stopFlying();
	void fly();
	void moveToSide(Player::Side side);

	rl::Ray getMouseRay();
};