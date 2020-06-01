#pragma once

#include "inc/game/checker/CheckerMan.hpp"
#include "inc/game/checker/CheckerKing.hpp"
#include "inc/game/player/Player.hpp"
#include "inc/graphics/primitives/Animation.hpp"
#include "inc/graphics/primitives/Renderable.hpp"
#include "inc/graphics/rendering/ShaderProvider.hpp"
#include "inc/graphics/scene/Environment.hpp"

namespace rl {
	extern "C" {
		#include "raylib.h"
	}
}



template <typename Base>
class CheckerRenderable: public Base, public Renderable, public std::enable_shared_from_this<CheckerRenderable<Base>> {
	static_assert(std::is_base_of<Checker, Base>::value, "base must be derived from Checker");

public:
	class JumpAnimation: public Animation {
		std::shared_ptr<CheckerRenderable<Base>> checker;
		rl::Vector3 origin, destination;
		Square destSquare;

	public:
		JumpAnimation(std::shared_ptr<CheckerRenderable<Base>> checker, Square origin, Square destination);
		void animate(float progress);

		static float jump(float linearProgress, float jumpHeight);
	};

	class KingConversionAnimation: public Animation {
		std::shared_ptr<CheckerRenderable<Base>> animated;
		std::shared_ptr<CheckerRenderable<CheckerKing>> king;
		float x, z;

	public:
		KingConversionAnimation(std::shared_ptr<CheckerRenderable<Base>> animated, std::shared_ptr<CheckerRenderable<CheckerKing>> king);
		void animate(float progress);
	};

	static rl::Model rlModel;

private:
	rl::Vector3 pos;
	float xAngle;

public:
	CheckerRenderable(Square square, Player::Side side);

	void draw();

	void moveTo(Square square);
	void setSquare(Square square);
	void setPos(rl::Vector3 position);
	void setXRotation(float angle);

	rl::Vector3 getPos();
	rl::Vector3 getPosForSquare(Square square);

	std::shared_ptr<CheckerKing> convertToKing();
};