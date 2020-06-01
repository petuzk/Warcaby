#pragma once

/* Przechowuje podstawowe stałe dotyczące plansy.
 * Ma to na celu ułatwienie zmiany zasad gry, ackolwiek
 * w implementacji często są pewne założenia co do tych wartości.
 */

class BoardConst {
public:
	static constexpr int SIZE = 8;
	static constexpr int START_ROWS = 3;
	static_assert(START_ROWS * 2 <= SIZE, "logic error");
	static constexpr int CHECKERS_PER_PLAYER = SIZE * START_ROWS / 2;
};