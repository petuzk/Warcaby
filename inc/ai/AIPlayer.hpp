#pragma once

#include <limits>
#include <thread>
#include <chrono>
#include "inc/game/board/Board.hpp"
#include "inc/game/board/BoardConst.hpp"
#include "inc/game/board/Square.hpp"
#include "inc/game/board/VirtualBoard.hpp"
#include "inc/game/player/Player.hpp"
#include "inc/game/player/VirtualPlayerMoveSequence.hpp"
#include "inc/graphics/primitives/Updatable.hpp"

/* Implementuje algorytm MinMax w interfejsie gracza.
 * Obliczenia odbywają się w oddzielnym wątku żeby nie
 * wstrzymywać rysowania.
 */

class AIPlayer: public Player, public Updatable {

	// Struktura pomocnicza do wyszukiwania najlepszego ruchu
	struct BestMoveSequence {
		// Ocena heurystyczna najlepszego znalezionego ruchu, głębokość rekurencji
		int value, depth;

		// Dla którego z graczy szukamy najlepszy ruch
		Player::Side side;

		// Znaleziony ruch lub nullptr
		std::shared_ptr<VirtualPlayerMoveSequence> sequence;

		BestMoveSequence(Player::Side side, int depth);

		// Sprawdza, czy ruch jest lepszy, jeżeli jest -- zapisuje
		void update(std::shared_ptr<VirtualPlayerMoveSequence> seq, std::shared_ptr<VirtualBoard> result);
	};

	// Dostęp do metody minMax()
	friend BestMoveSequence;

	// Zwraca ocene heurystyczną (l. ujemna jeżeli pierwszy gracz ma większe szanse na zwicięstwo)
	static int getHeuristicEvaluation(std::shared_ptr<VirtualBoard> board);

	// "Main" algorytmu: metoda rekurencyjna, gdy depth = 0 zwraca ocenę heurystyczną
	static BestMoveSequence minMax(
		Player::Side forSide, int recursionDepth,
		std::shared_ptr<VirtualPlayerMoveSequence> seq,
		std::shared_ptr<VirtualBoard> board);

	// Buduje możliwe ruchy i wywołuje BestMoveSequence::update
	static void testPossibleSequences(
		std::shared_ptr<VirtualPlayerMoveSequence> seq,
		std::shared_ptr<VirtualBoard> board,
		BestMoveSequence& best);

	// Wątek w którym wykonuje się threadMain()
	std::thread thread;

	// Kontrola wykonania wątku
	enum ThreadState {
		WAIT,
		STOP,
		EXEC,
		DONE
	};

	// Zmienne używane do komunikacji między wątkami
	ThreadState threadState;
	std::shared_ptr<VirtualBoard> startBoard;
	std::shared_ptr<VirtualPlayerMoveSequence> startSequence, choosenSequence;

	// "Main" wątku: w pętli oczekuje na nowe zadanie (EXEC) lub koniec działania (STOP)
	void threadMain();

	// Wywoływana przez Game
	void requestMove();

	// Sprawdza, czy wątek skończył poszukiwanie ruchu
	void update();

public:
	AIPlayer(Player::Side side);
	~AIPlayer();
};