#include "inc/graphics/rendering/Camera.hpp"
#include "src/graphics/primitives/PropertyAnimation_template.cpp"
#include "inc/graphics/scene/BoardRenderable.hpp"

Camera::ZRotationAnimation::ZRotationAnimation(float fromAngle, float toAngle, bool parallelToPrev):
	PropertyAnimation(milliseconds(500), fromAngle, toAngle, parallelToPrev), cam(Camera::getInstance()) { }

float Camera::ZRotationAnimation::getProgress(float linearProgress) {
	// https://easings.net/en#easeInOutCubic
	float x = linearProgress;
	return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
}

void Camera::ZRotationAnimation::updateProperty(float angle) {
	cam->setZAngle(angle);
	cam->recalculatePos();
}



Camera::TargetDistanceAnimation::TargetDistanceAnimation(float fromDist, float toDist, Animation::State state):
	PropertyAnimation(milliseconds(500), fromDist, toDist, false, state), cam(Camera::getInstance()) { }

float Camera::TargetDistanceAnimation::getProgress(float linearProgress) {
	// https://easings.net/en#easeInOutCubic
	float x = linearProgress;
	return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
}

void Camera::TargetDistanceAnimation::updateProperty(float dist) {
	cam->setDistance(dist);
}



Camera::FlyAnimation::FlyAnimation(float startAngle):
	PropertyAnimation(milliseconds(5000), startAngle, startAngle + 2*PI, false, Animation::BACKGROUND),
	cam(Camera::getInstance()) { }

void Camera::FlyAnimation::updateProperty(float angle) {
	cam->setZAngle(angle);
	cam->recalculatePos();
}

Animation::State Camera::FlyAnimation::animate() {
	// animacja zakończona z powodu innego, niż upłynięcie czasu
	if (state == Animation::FINISHED) {
		return Animation::FINISHED;
	}

	Animation::animate();

	if (state == Animation::FINISHED) {
		state = Animation::BACKGROUND;
		startTime = Animation::time();
	}

	return state;
}



Camera::Camera():
	rlCam({ 0 }), zAngle(0.0f), distance(flyDistance), playerView(false),
	flyAnim(nullptr), rotAnim(nullptr), distAnim(nullptr)
{
	rlCam.position = rl::Vector3Add(BoardRenderable::CAMERA_TARGET, (rl::Vector3){ 0.0f, 7.5f, 10.0f });
	rlCam.target = BoardRenderable::CAMERA_TARGET;
	rlCam.up = (rl::Vector3){ 0.0f, 1.0f, 0.0f };
	rlCam.fovy = 60.0f;
	rlCam.type = rl::CAMERA_PERSPECTIVE;
	rl::SetCameraMode(rlCam, rl::CAMERA_CUSTOM);
}

Camera* Camera::getInstance() {
	static Camera* instance = new Camera();
	return instance;
}

float Camera::normalizeAngle(float a) {
	while (a < 0.0f) a += 2*PI;
	while (a > 2*PI) a -= 2*PI;
	return a;
}

void Camera::setZAngle(float angle) {
	zAngle = normalizeAngle(angle);
}

void Camera::setDistance(float dist) {
	distance = dist;
}

void Camera::recalculatePos() {
	static const float sinPA = sinf(planeAngle);
	static const float cosPA = cosf(planeAngle);
	rlCam.position.x = rlCam.target.x + distance * sinf(zAngle) * cosPA;
	rlCam.position.y = rlCam.target.y + distance * sinPA * sinPA;
	rlCam.position.z = rlCam.target.z + distance * cosf(zAngle) * cosPA;
	ShaderProvider::getInstance()->updateCameraPos(rlCam.position);
}

void Camera::begin3D() {
	rl::BeginMode3D(rlCam);
}

void Camera::end3D() {
	rl::EndMode3D();
}

void Camera::stopFlying() {
	// zakończone animacje są automatycznie usuwane przez Animator
	if (flyAnim) {
		flyAnim->stop();
		flyAnim = nullptr;
	}
}

void Camera::fly() {
	if (flyAnim != nullptr) {
		return;
	}

	playerView = false;

	bool runDistAnimation = distance != flyDistance;
	if (runDistAnimation) {
		distAnim = new TargetDistanceAnimation(distance, flyDistance, Animation::BACKGROUND);
	}
	flyAnim = new FlyAnimation(zAngle);
}

void Camera::moveToSide(Player::Side side) {
	// ponieważ nie można wprost sprawdzać wartości zmiennoprzecinkowe,
	// wprowadzono zmienne pomocznicze
	if (!playerView || playerViewSide != side) {
		playerView = true;
		playerViewSide = side;

		stopFlying();

		bool runDistAnimation = distance != playDistance;
		if (runDistAnimation) {
			distAnim = new TargetDistanceAnimation(distance, playDistance);
		}
		rotAnim = new ZRotationAnimation(zAngle, PI * (2 - side), runDistAnimation);
	}
}

rl::Ray Camera::getMouseRay() {
	return rl::GetMouseRay(rl::GetMousePosition(), rlCam);
}