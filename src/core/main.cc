#include "App.h"

int main(int argc,  char* argv[]) {
	
	App* app = App::getInstance();
	
	if (!app->init(2)) {
		printf("Failed initializing app\n");
		return -1;
	}
	
	while (!app->done()) {
		app->handleEvents();
		app->update();
	}
	
	
	app->release();
	
	delete app;
	return 0;
}

