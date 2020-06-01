#include "inc/ai/AIPlayer.hpp"

#include <iostream>

AIPlayer::BestMoveSequence::BestMoveSequence(Player::Side side, int depth):
	side(side), depth(depth),
	value(side == Player::PLAYER_1 ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min()) { }

void AIPlayer::BestMoveSequence::update(std::shared_ptr<VirtualPlayerMoveSequence> seq, std::shared_ptr<VirtualBoard> result) {
	Player::Side opSide = Player::opposite(side);
	auto emptySeq = std::make_shared<VirtualPlayerMoveSequence>(result.get(), opSide);
	int test = minMax(opSide, depth - 1, emptySeq, result).value;
	if (
		(side == Player::PLAYER_1 && test < value) ||
		(side == Player::PLAYER_2 && test > value) ||
		(test == value && (sequence == nullptr || (std::rand() % 4) == 0)))
	{
		value = test;
		sequence = seq;
	}
}

/* Ta funkcja zwraca liczbę ujemną jeżeli pierwszy gracz (grający białymi pionami)
 * ma większe szanse na zwycięstwo lub dodatnią, jeżeli przewagę ma drugi gracz.
 */

int AIPlayer::getHeuristicEvaluation(std::shared_ptr<VirtualBoard> board) {
	int score = 0;

	score += board->getPlayersCheckers(Player::PLAYER_2).size();
	score -= board->getPlayersCheckers(Player::PLAYER_1).size();
	score *= 3; // przewaga w liczbie pionów ma większe znaczenie

	// dostrajanie
	for (int row = 0; row < BoardConst::SIZE; row++) {
		for (int col = row % 2; col < BoardConst::SIZE; col += 2) {
			auto checker = board->at(Square(col, row));
			if (checker != nullptr) {
				if (checker->canCapture(board.get())) {
					// odejmij 1 jezeli przeciwnik może wykonać bicie
					score += checker->getSide() == Player::PLAYER_1 ? -1 : 1;
				}
				else if (!checker->canMove(board.get())) {
					// dodaj 1 jeżeli przeciwnik jest zablokowany
					score += checker->getSide() == Player::PLAYER_1 ? 1 : -1;
				}
			}
		}
	}

	return score;
}

AIPlayer::BestMoveSequence AIPlayer::minMax(
	Player::Side forSide, int recursionDepth,
	std::shared_ptr<VirtualPlayerMoveSequence> seq,
	std::shared_ptr<VirtualBoard> board)
{
	BestMoveSequence best(forSide, recursionDepth);

	if (recursionDepth == 0) {
		best.sequence = seq;
		best.value = getHeuristicEvaluation(board);
		return best;
	}

	testPossibleSequences(seq, board, best);
	return best;
}

void AIPlayer::testPossibleSequences(
	std::shared_ptr<VirtualPlayerMoveSequence> seq,
	std::shared_ptr<VirtualBoard> board,
	BestMoveSequence& best)
{
	for (Square& sq: seq->getOriginsForNextMove()) {
		auto checker = board->at(sq);
		for (PlayerMove& move: checker->getPossibleMoves(board.get(), seq)) {
			auto nextSequence = seq->virtualCopy();
			nextSequence->add(move);
			auto nextBoard = board->virtualCopy();
			nextSequence->apply(nextBoard);
			if (nextSequence->isFinished()) {
				best.update(nextSequence, nextBoard);
			} else {
				testPossibleSequences(nextSequence, nextBoard, best);
			}
		}
	}
}

void AIPlayer::threadMain() {
	while (true) {
		while (threadState == WAIT || threadState == DONE) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		if (threadState == STOP) {
			return;
		}

		if (threadState == EXEC) {
			BestMoveSequence best = minMax(mySide, 4, startSequence, startBoard);
			choosenSequence = best.sequence;
			if (threadState == STOP) {
				return;
			} else {
				threadState = DONE;
			}
		}
	}
}

AIPlayer::AIPlayer(Player::Side side): Player(side), Updatable(Priority::PAIPlayer),
	threadState(WAIT), startSequence(nullptr), thread(&AIPlayer::threadMain, this) { }

AIPlayer::~AIPlayer() {
	threadState = STOP;
	thread.join();
}

void AIPlayer::update() {
	if (threadState == DONE) {
		threadState = WAIT;
		choosenSequence->addInto(sequence);
		respond();
	}
}

void AIPlayer::requestMove() {
	startSequence = sequence->virtualCopy();
	startBoard = board->virtualCopy();
	threadState = EXEC;
}