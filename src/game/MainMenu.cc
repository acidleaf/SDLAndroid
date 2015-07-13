#include "MainMenu.h"
#include "App.h"

bool MainMenu::init() {
	if (!_ss.init("spritesheets/mainMenu.xml")) return false;
	
	if (!_startBtn.init(&_ss, "start_btn_normal", "start_btn_pressed")) return false;
	
	auto app = App::getInstance();
	_startBtn.anchor(0.5f, 0.5f);
	_startBtn.pos(app->resX() / 2, app->resY() / 2);
	_startBtn.onClick([](Button* target, void* data) {
		auto sm = App::getInstance()->scene();
		sm->push(sm->mainScene());
	}, this);
	
	return true;
}

void MainMenu::release() {
	_ss.release();
	_startBtn.release();
}

void MainMenu::update() {
	
}

void MainMenu::render() {
	_startBtn.render();
}

void MainMenu::handleEvents(const SDL_Event& e) {
	if (_startBtn.handleEvents(e)) return;
}
