#include "inc/graphics/MainWindow.hpp"

MainWindow::MainWindow() {
	rl::SetTraceLogLevel(rl::LOG_WARNING);
	rl::SetConfigFlags(rl::FLAG_WINDOW_RESIZABLE);
	rl::InitWindow(winWidth, winHeight, "Warcaby");
	rl::SetExitKey(0);

	rl::SetTargetFPS(60);

	closeRequested = false;
	gameEndRequested = false;
	game = nullptr;

	camera = Camera::getInstance();
	pp = PostProcessor::getInstance();

	env = new Environment();
	board = new BoardRenderable();

	mainMenu = new MainMenuOverlay();
	gameEndMenu = new GameEndOverlay();
	pauseMenu = new PauseMenuOverlay();
	pauseButton = new PauseButtonOverlay();
}

MainWindow* MainWindow::getInstance() {
	static MainWindow* instance = new MainWindow();
	return instance;
}

Environment*        MainWindow::getEnvironment() { return env; }
BoardRenderable*    MainWindow::getBoard()       { return board; }
MainMenuOverlay*    MainWindow::getMainMenu()    { return mainMenu; }
GameEndOverlay*     MainWindow::getGameEndMenu() { return gameEndMenu; }
PauseMenuOverlay*   MainWindow::getPauseMenu()   { return pauseMenu; }
PauseButtonOverlay* MainWindow::getPauseButton() { return pauseButton; }

void MainWindow::close() {
	closeRequested = true;
}

void MainWindow::startGame(bool player1IsAi, bool player2IsAi) {
	if (game == nullptr) {
		env->reset();
		board->reset();

		Player* player1 = player1IsAi ?
			static_cast<Player*>(new AIPlayer(Player::PLAYER_1)) :
			static_cast<Player*>(new HumanPlayer(Player::PLAYER_1));

		Player* player2 = player2IsAi ?
			static_cast<Player*>(new AIPlayer(Player::PLAYER_2)) :
			static_cast<Player*>(new HumanPlayer(Player::PLAYER_2));

		Player::Side camSide = Player::PLAYER_1;

		if (player1IsAi && !player2IsAi) {
			camSide = Player::PLAYER_2;
		}

		game = new GameWrapper(board, player1, player2, Player::PLAYER_1, camSide);
	}
}

void MainWindow::stopGame() {
	gameEndRequested = true;
}

void MainWindow::mainloop() {
	mainMenu->setHidden(false);
	camera->fly();

	while (!rl::WindowShouldClose() && !closeRequested) {

		updateAll(animateAll() == Animation::FOREGROUND);

		if (gameEndRequested) {
			// nie można tego robić w wywołaniu updateAll()
			gameEndRequested = false;
			delete game;
			game = nullptr;
			camera->fly();
		}

		pp->beginDrawing();

			rl::ClearBackground(rl::BROWN);

			camera->begin3D();

				drawAll(Renderable::T3D);

			camera->end3D();

			drawAll(Renderable::T2D);

		pp->endDrawing();
	}

	rl::CloseWindow();
}