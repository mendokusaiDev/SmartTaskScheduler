#include "TUI.h"
#include <cstdlib>

scheduler::TUI app;

void whenExit() {
	app.atExit();
	return;
}


int main() {

	system("chcp 65001");
	system("cls");

	atexit(whenExit);
	app.mainMenu();

	return 0;
}