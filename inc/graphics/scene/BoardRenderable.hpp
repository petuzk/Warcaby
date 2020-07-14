#pragma once

#include <stdexcept>
#include "inc/game/board/Board.hpp"
#include "inc/game/board/Square.hpp"
#include "inc/game/checker/CheckerMan.hpp"
#include "inc/game/player/PlayerMoveSequence.hpp"
#include "inc/graphics/HumanPlayer.hpp"
#include "inc/graphics/primitives/Renderable.hpp"
#include "inc/graphics/primitives/Updatable.hpp"
#include "inc/graphics/rendering/Camera.hpp"
#include "inc/graphics/rendering/ShaderProvider.hpp"
#include "inc/graphics/scene/CheckerRenderable.hpp"

#include <math.h>  // to avoid errors when including raymath inside namespace
namespace rl {
	extern "C" {
		#include "raylib.h"
		#include "rlgl.h"
		#include "raymath.h"
	}
}

class BoardRenderable: public Board, public Renderable, public Updatable {
	enum InterfaceState {
		NONE,
		SELECT_CHECKER,
		SELECT_MOVE
	};

	static constexpr int SHADER_MAX_HIGHLIGHTS = 16;

	static rl::Model rlModel;
	static int shaderHlFromLoc[SHADER_MAX_HIGHLIGHTS];
	static int shaderHlToLoc[SHADER_MAX_HIGHLIGHTS];
	static int shaderHlColorLoc[SHADER_MAX_HIGHLIGHTS];

	bool inputEnabled;
	bool delayedShaderUpdate;
	InterfaceState ifState;
	HumanPlayer* moveForPlayer;
	std::vector<PlayerMove> possibleMoves;
	std::shared_ptr<PlayerMoveSequence> moveSequence;
	Square prevPointed, selectedMoveSquare, selectedCheckerSquare, reSelectedCheckerSquare;

	Square getPointedSquare();
	void updateShaderPos(int i, Square sq);
	void updateShaderColor(int i, rl::Color c = { 0, 0, 0, 0 });
	void updateShaderData();

public:
	static const rl::Vector3 CAMERA_TARGET;

	BoardRenderable();
	void reset();

	void setInputEnabled(bool en);

	void draw();
	void update();

	void requestMoveFor(HumanPlayer* player, std::shared_ptr<PlayerMoveSequence> sequence);
};