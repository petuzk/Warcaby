#pragma once

#include "inc/game/board/Board.hpp"
#include "inc/game/board/BoardConst.hpp"
#include "inc/game/player/Player.hpp"
#include "inc/graphics/primitives/Renderable.hpp"
#include "inc/graphics/rendering/ShaderProvider.hpp"

#include <math.h>  // na potrzebę raymath
namespace rl{
	extern "C" {
		#include "raylib.h"
		#include "raymath.h"
	}
}

class Environment: public Renderable {
	int outsidePosIndices[2];

public:
	Environment();

	void draw();
	void reset();

	rl::Vector3 getOutsidePosFor(Player::Side side);
};