/*
The source code is from Gustavo Pezzi, co-founder of Pikuma.com
*/

#include "DiskApp.h"

int main(int argc, char *args[]) {
	Application app;

	app.setup();

	while (app.isRunning()) {
		app.input();
		app.update();
		app.render();
	}

	app.destroy();

	return 0;
}