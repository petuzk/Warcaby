#pragma once

#include <memory>

class Game;
class Board;
class PlayerMove;
class PlayerMoveSequence;

/* Reprezentuje gracza.
 */

class Player {
public:
	enum Side {
		PLAYER_1 = 0,
		PLAYER_2
	};

	static Side opposite(Side to);

protected:
	Side mySide;
	Game* game;
	Board* board;
	std::shared_ptr<PlayerMoveSequence> sequence;

	// Metody dla komunikacji z Game
	virtual void requestMove() = 0;
	void respond(PlayerMove move);
	void respond();

public:
	Player(Side side);
	virtual ~Player() = default;

	Side getSide();

	/* Metoda, wywoływana przez Game.
	 * Zapisuje w odpowiednich polach przekazane argumenty
	 * i wywołuje wersję bez parametrów.
	 */
	void requestMove(Game* game, Board* board, std::shared_ptr<PlayerMoveSequence> sequence);
};