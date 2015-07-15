#include "MainScene.h"
#include "App.h"

bool MainScene::init() {
	auto app = App::getInstance();
	
	if (!_ss.init("spritesheets/ss-default.xml")) return false;
	
	_ss.getSprite("logo.png", _s1);
	_s1.anchor(0.5f, 0.5f);
	_s1.pos(app->resX() / 2, app->resY() / 2);
	
	_ss.getSprite("circle.png", _s2);
	_s2.anchor(0.5f, 0.5f);
	_s2.pos(_s2.size().x / 2, _s2.size().y / 2);
	
	if (!_btn.init(&_ss, "btn_normal.png", "btn_pressed.png")) return false;
	_btn.anchor(0.5f, 0.5f);
	_btn.pos(app->resX() / 2, 300);
	_btn.onClick([](Button* target, void* obj) {
		auto caller = (MainScene*)obj;
		caller->btnClicked(target);
	}, this);
	
	_v.x = 6.0f;
	
	_font.init("Cookie.ttf", 256, 256, 32);
	
	return true;
}

void MainScene::release() {
	_s1.release();
	_s2.release();
	_ss.release();
	_font.release();
}

void MainScene::update() {
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
	
	//_font.renderAtlas();
	
	_font.writeLine("Hello world!", 10, 20);
}

void MainScene::handleEvents(const SDL_Event& e) {
	auto app = App::getInstance();
	if (_btn.handleEvents(e)) return;
	
	if (e.type == SDL_FINGERDOWN) {
		_s2.pos(app->resX() * e.tfinger.x, app->resY() * e.tfinger.y);
	}
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_AC_BACK) {
		app->scene()->pop();
	}
}


void MainScene::btnClicked(Button* target) {
	auto app = App::getInstance();
	_s2.pos(app->resX() / 2, app->resY() / 2);
}