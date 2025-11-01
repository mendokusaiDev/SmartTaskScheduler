#include "TUI.h"
#include <cstdlib>

scheduler::TUI app;

void whenExit() {
	app.atExit();
	return;
}


int main() {

	atexit(whenExit);
	app.mainMenu();

	return 0;
}