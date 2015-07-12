#include "MainScene.h"
#include "App.h"

bool MainScene::init() {
	auto app = App::getInstance();
	
	if (!_ss.init("spritesheets/ss-default.xml")) return false;
	
	_s1 = _ss.getSprite("logo.png");
	_s1.anchor(0.5f, 0.5f);
	_s1.pos(app->resX() / 2, app->resY() / 2);
	
	_s2 = _ss.getSprite("circle.png");
	_s2.anchor(0.5f, 0.5f);
	_s2.pos(_s2.size().x / 2, _s2.size().y / 2);
	
	if (!_btn.init(_ss, "btn_normal.png", "btn_pressed.png")) return false;
	_btn.anchor(0.5f, 0.5f);
	_btn.pos(app->resX() / 2, 300);
	
	_v.x = 8.0f;
	
	return true;
}

void MainScene::release() {
	_s1.release();
	_s2.release();
	_ss.release();
}

void MainScene::update() {
	if (_btn.pressed()) return;
	
	auto app = App::getInstance();
	
	_s2.pos() += _v;
	if (_s2.pos().x < _s2.size().x / 2) {
		_s2.pos().x = _s2.size().x / 2;
		_v.x *= -1;
	} else if (_s2.pos().x > app->resX() - _s2.size().x / 2) {
		_s2.pos().x = app->resX() - _s2.size().x / 2;
		_v.x *= -1;
	}
}

void MainScene::render() {
	_s1.render();
	_s2.render();
	
	_btn.render();
}

void MainScene::handleEvents(const SDL_Event& e) {
	auto app = App::getInstance();
	if (e.type == SDL_FINGERDOWN) {
		_s2.pos(app->resX() * e.tfinger.x, app->resY() * e.tfinger.y);
	}
	
	_btn.handleEvents(e);
}
