#include "MainMenu.h"
#include "App.h"

bool MainMenu::init() {
	if (!_ss.init("spritesheets/mainMenu.xml")) return false;
	
	if (!_ss.getSprite("panel", _panel)) return false;
	if (!_startBtn.init(&_ss, "startBtn_normal", "startBtn_pressed")) return false;
	if (!_optionsBtn.init(&_ss, "optionsBtn_normal", "optionsBtn_pressed")) return false;
	
	auto app = App::getInstance();
	
	_panel.anchor(0.5f, 0.5f);
	_panel.pos(app->resX() / 2, app->resY() / 2);
	
	_startBtn.anchor(0.5f, 0.5f);
	_startBtn.pos(app->resX() / 2, _panel.pos().y - 40);
	_startBtn.onClick([](Button* target, void* data) {
		auto sm = App::getInstance()->scene();
		sm->push(sm->mainScene());
	}, this);
	
	_optionsBtn.anchor(0.5f, 0.5f);
	_optionsBtn.pos(app->resX() / 2, _panel.pos().y + 30);
	
	return true;
}

void MainMenu::release() {
	_ss.release();
	_panel.release();
	_startBtn.release();
	_optionsBtn.release();
}

void MainMenu::update() {
	
}

void MainMenu::render() {
	_panel.render();
	_startBtn.render();
	_optionsBtn.render();
}

void MainMenu::handleEvents(const SDL_Event& e) {
	if (_startBtn.handleEvents(e)) return;
	if (_optionsBtn.handleEvents(e)) return;
}
