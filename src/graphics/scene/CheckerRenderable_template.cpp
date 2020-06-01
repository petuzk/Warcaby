#include "inc/graphics/scene/CheckerRenderable.hpp"
#include "inc/graphics/MainWindow.hpp"

template <typename Base>
CheckerRenderable<Base>::JumpAnimation::JumpAnimation(
	std::shared_ptr<CheckerRenderable<Base>> checker, Square origin, Square destination):
	Animation(milliseconds(200)), checker(checker), destSquare(destination)
{
	this->origin = checker->getPosForSquare(origin);
	this->destination = checker->getPosForSquare(destination);
}

template <typename Base>
float CheckerRenderable<Base>::JumpAnimation::jump(float linearProgress, float jumpHeight) {
	// https://www.desmos.com/calculator/pkao43tmb5
	return jumpHeight * -4.0f * linearProgress * (linearProgress - 1);
}

template <typename Base>
void CheckerRenderable<Base>::JumpAnimation::animate(float progress) {
	checker->setPos({
		(destination.x - origin.x) * progress + origin.x,
		(destination.y - origin.y) * progress + origin.y + jump(progress, 0.9f),
		(destination.z - origin.z) * progress + origin.z,
	});
}



template <typename Base>
CheckerRenderable<Base>::KingConversionAnimation::KingConversionAnimation(
	std::shared_ptr<CheckerRenderable<Base>> animated, std::shared_ptr<CheckerRenderable<CheckerKing>> king):
	Animation(milliseconds(500)), animated(animated), king(king)
{
	rl::Vector3 pos = animated->getPosForSquare(animated->getSquare());
	x = pos.x;
	z = pos.z;
	// ukryj do końca animacji
	king->setPos({ 0.0f, -1.0f, 0.0f });
}

template <typename Base>
void CheckerRenderable<Base>::KingConversionAnimation::animate(float progress) {
	if (progress < 1.0f) {
		animated->setPos({ x, JumpAnimation::jump(progress, 2.4f), z	});
		animated->setXRotation(progress * 5 * PI);
	} else {
		// ukryj, po zwolnienu z pamięci obiektu animacji pion również zostanie usunięty
		animated->setPos({ 0.0f, -1.0f, 0.0f });
		king->setSquare(animated->getSquare());
	}
}



template <typename Base>
rl::Model CheckerRenderable<Base>::rlModel = { 0 };

template <typename Base>
CheckerRenderable<Base>::CheckerRenderable(Square square, Player::Side side):
	Base(square, side), Renderable(Renderable::T3D), xAngle(0.0f)
{
	setSquare(square);

	if (std::is_base_of<CheckerMan, Base>::value) {
		if (rlModel.meshCount == 0) {
			rlModel = rl::LoadModel("resources/models/checker.gltf");
			rl::Shader shader = ShaderProvider::getInstance()->loadShader("resources/shaders/base.vs", "resources/shaders/checker.fs");
			rlModel.materials[0].shader = shader;
			rlModel.materials[1].shader = shader;
		}
	}
	else if (std::is_base_of<CheckerKing, Base>::value) {
		if (rlModel.meshCount == 0) {
			// kopiuj model
			rlModel = CheckerRenderable<CheckerMan>::rlModel;
			// przewróc
			rlModel.transform = rl::MatrixRotateX(PI);
		}
	}

	static_assert(std::is_base_of<CheckerMan, Base>::value || std::is_base_of<CheckerKing, Base>::value, "not implemented");
}

template <typename Base>
void CheckerRenderable<Base>::draw() {
	if (xAngle != 0.0f)
		// ustaw nachylenie
		rlModel.transform = rl::MatrixRotateX(xAngle);

	rl::DrawModel(rlModel, pos, 1.0f, Checker::mySide == Player::PLAYER_1 ? rl::RAYWHITE : rl::ORANGE);

	if (xAngle != 0.0f)
		// przywróc nachylenie domyślne (bo model jest wspólny)
		rlModel.transform = rl::MatrixIdentity();
}

template <typename Base>
void CheckerRenderable<Base>::moveTo(Square square) {
	new JumpAnimation(std::enable_shared_from_this<CheckerRenderable<Base>>::shared_from_this(), Checker::mySquare, square);
	Base::setSquare(square);
}

template <typename Base>
void CheckerRenderable<Base>::setSquare(Square square) {
	Base::setSquare(square);
	setPos(getPosForSquare(square));
}

template <typename Base>
void CheckerRenderable<Base>::setPos(rl::Vector3 position) {
	pos = position;
}

template <typename Base>
void CheckerRenderable<Base>::setXRotation(float angle) {
	xAngle = angle;
}

template <typename Base>
rl::Vector3 CheckerRenderable<Base>::getPos() {
	return pos;
}

template <typename Base>
rl::Vector3 CheckerRenderable<Base>::getPosForSquare(Square square) {
	if (square.isOutside()) {
		return MainWindow::getInstance()->getEnvironment()->getOutsidePosFor(Checker::mySide);
	} else {

		return (rl::Vector3){ -3.5f + square.col(), 0.125f, 3.5f - square.row() };
	}
}

template <typename Base>
std::shared_ptr<CheckerKing> CheckerRenderable<Base>::convertToKing() {
	auto king = std::make_shared<CheckerRenderable<CheckerKing>>(Checker::mySquare, Checker::mySide);
	new KingConversionAnimation(std::enable_shared_from_this<CheckerRenderable<Base>>::shared_from_this(), king);
	return king;
}