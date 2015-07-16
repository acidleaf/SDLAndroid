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
	
	_font = app->fonts()->getFont("04b03.ttf", 8);
	
	return true;
}

void MainScene::release() {
	_s1.release();
	_s2.release();
	_ss.release();
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
	
	auto app = App::getInstance();
	auto fonts = App::getInstance()->fonts();
	
	static int i = 0;
	for (int i = 0; i < 10; ++i) {
		fonts->writeLine(_font, 10, (i + 1) * _font->size, "Hello line %d", i);
	}
	
	const char* something = "something something...";
	int w = fonts->lineWidth(_font, something);
	fonts->writeLine(_font, app->resX() - w, 10, something);
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