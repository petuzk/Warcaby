#include "inc/game/board/Square.hpp"

Square::Offset::Offset(): _coloff(0), _rowoff(0) { }
Square::Offset::Offset(int coloff, int rowoff): _coloff(coloff), _rowoff(rowoff) { }

Square::Square(): _row(-1), _col(-1) { }

Square::Square(int col, int row) {
	if (col < 0 || col >= BoardConst::SIZE || row < 0 || row >= BoardConst::SIZE) {
		col = row = -1;
	}

	_col = col;
	_row = row;
}

Square::Square(const char* name): Square(std::string(name)) { }

Square::Square(std::string name) {
	// Mogą skończyć się nam litery
	static_assert(BoardConst::SIZE <= 26, "unsupported board size");

	if (name.length() < 2) {
		throw std::invalid_argument("square name must be at least 2 characters long");
	}

	char col_name = std::toupper(name[0], std::locale());
	char row_name = name[1];

	if (col_name == '-' && row_name == '-') {
		_col = _row = -1;
		return;
	}

	if (col_name < 'A' || col_name >= 'A' + BoardConst::SIZE) {
		throw std::invalid_argument("bad column name");
	}

	_col = col_name - 'A';
	_row = std::stoi(name.substr(1));

	if (_row < 0 || _row >= BoardConst::SIZE) {
		throw std::invalid_argument("bad row name");
	}
}

bool Square::isOutside() const {
	return _col < 0;
}

bool Square::isBlack() const {
	return !((_col + _row) % 2);
}

int Square::col() const { return _col; }
int Square::row() const { return _row; }
int Square::Offset::colOff() const { return _coloff; }
int Square::Offset::rowOff() const { return _rowoff; }

std::string Square::name() {
	if (isOutside())
		return "--";

	char name[2] = { static_cast<char>('A' + _col), static_cast<char>('1' + _row) };
	return std::string(name, 2);
}

bool Square::isBetween(Square x, Square y) const {
	int min_col, max_col;
	min_col = max_col = x.col();
	if (y.col() < min_col)
		min_col = y.col();
	else
		max_col = y.col();

	if (!(min_col < _col && _col < max_col))
		return false;

	int min_row, max_row;
	min_row = max_row = x.row();
	if (y.row() < min_row)
		min_row = y.row();
	else
		max_row = y.row();

	return min_row < _row && _row < max_row;
}

Square Square::operator+(Square::Offset offset) const {
	return Square(_col + offset.colOff(), _row + offset.rowOff());
}

bool Square::operator==(Square other) const {
	return _col == other.col() && _row == other.row();
}

bool Square::operator!=(Square other) const {
	return !(*this == other);
}

Square::operator bool() const {
	return !isOutside();
}

Square::Offset Square::Offset::operator*(int mul) const {
	return Offset(_coloff * mul, _rowoff * mul);
}

int& Square::Offset::operator[](int i) {
	if (i == 0)
		return _coloff;
	else if (i == 1)
		return _rowoff;
	else
		throw std::invalid_argument("index must be either 0 or 1");
}