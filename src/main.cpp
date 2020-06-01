#include <iostream>
#include "inc/graphics/MainWindow.hpp"

int main(int argc, char const *argv[]) {
	MainWindow::getInstance()->mainloop();
	return 0;
}