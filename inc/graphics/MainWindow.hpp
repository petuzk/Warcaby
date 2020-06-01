#pragma once

#include "inc/ai/AIPlayer.hpp"
#include "inc/graphics/GameWrapper.hpp"
#include "inc/graphics/HumanPlayer.hpp"
#include "inc/graphics/overlay/GameEndOverlay.hpp"
#include "inc/graphics/overlay/MainMenuOverlay.hpp"
#include "inc/graphics/overlay/PauseButtonOverlay.hpp"
#include "inc/graphics/overlay/PauseMenuOverlay.hpp"
#include "inc/graphics/primitives/Animator.hpp"
#include "inc/graphics/primitives/Renderer.hpp"
#include "inc/graphics/primitives/Updater.hpp"
#include "inc/graphics/rendering/Camera.hpp"
#include "inc/graphics/rendering/PostProcessor.hpp"
#include "inc/graphics/rendering/ShaderProvider.hpp"
#include "inc/graphics/scene/BoardRenderable.hpp"
#include "inc/graphics/scene/Environment.hpp"

namespace rl {
	extern "C" {
		#include "raylib.h"
	}
}

class MainWindow: public Updater, public Renderer, public Animator {
	MainWindow();

	bool closeRequested;
	bool gameEndRequested;

	GameWrapper* game;

	Camera* camera;
	PostProcessor* pp;

	Environment* env;
	BoardRenderable* board;

	MainMenuOverlay* mainMenu;
	GameEndOverlay* gameEndMenu;
	PauseMenuOverlay* pauseMenu;
	PauseButtonOverlay* pauseButton;

public:
	static constexpr int winWidth = 960;
	static constexpr int winHeight = 720;

	MainWindow(MainWindow const &)     = delete;
  void operator=(MainWindow const &) = delete;

	static MainWindow* getInstance();

	Environment* getEnvironment();
	BoardRenderable* getBoard();

	MainMenuOverlay* getMainMenu();
	GameEndOverlay* getGameEndMenu();
	PauseMenuOverlay* getPauseMenu();
	PauseButtonOverlay* getPauseButton();

	void close();
	void startGame(bool player1IsAi, bool player2IsAi);
	void stopGame();

	void mainloop();
};