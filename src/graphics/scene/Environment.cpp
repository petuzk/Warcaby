#include "inc/graphics/scene/Environment.hpp"

Environment::Environment(): Renderable(Renderable::T3D) {
	ShaderProvider* sProv = ShaderProvider::getInstance();
	sProv->addLight({ true, ShaderProvider::AMBIENT, rl::Vector3Zero(),                 (rl::Color){ 253, 236, 167, 255 } });
	sProv->addLight({ true, ShaderProvider::POINT, (rl::Vector3){ -9.0f, 7.0f, -9.0f }, (rl::Color){ 253, 236, 167, 255 } });
	sProv->addLight({ true, ShaderProvider::POINT, (rl::Vector3){  9.0f, 7.0f, -9.0f }, (rl::Color){ 253, 236, 167, 255 } });
	sProv->addLight({ true, ShaderProvider::POINT, (rl::Vector3){ -9.0f, 7.0f,  9.0f }, (rl::Color){ 253, 236, 167, 255 } });
	sProv->addLight({ true, ShaderProvider::POINT, (rl::Vector3){  9.0f, 7.0f,  9.0f }, (rl::Color){ 253, 236, 167, 255 } });

	reset();
}

void Environment::draw() {
	// tu może być dodany np. stól
}

void Environment::reset() {
	outsidePosIndices[0] = outsidePosIndices[1] = 0;
}

rl::Vector3 Environment::getOutsidePosFor(Player::Side side) {
	int index = outsidePosIndices[side];
	outsidePosIndices[side]++;

	static constexpr int checkersPerCol = (BoardConst::CHECKERS_PER_PLAYER / 2);
	int col = index / checkersPerCol;
	int row = index % checkersPerCol;
	float x = 5.5f + col;
	float z = 4.0f - row;
	if (side == Player::PLAYER_2) {
		x = -x; z = -z;
	}

	return (rl::Vector3){ x, -1.0f, z };
}