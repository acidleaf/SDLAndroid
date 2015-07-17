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
	auto fonts = app->fonts();
	
	static int i = 0;
	for (int i = 0; i < 10; ++i) {
		fonts->writeLine(_font, 10, (i + 1) * _font->size, "Hello line %d", i);
	}
	
	const char* msg = "Lorem ipsum dolor sit amet\nconsectetur adipiscing elit. Nullam sem justo\nvenenatis ac convallis molestie, vulputate eu ligula.\nMorbi elementum, nisl eget tincidunt luctus,\nfelis libero volutpat nisi, id dignissim tortor augue at nunc.\n\n  Nam varius nec sem ac laoreet. Donec orci erat, lobortis id laoreet et, ullamcorper vitae mauris.\nNam id maximus lacus. Morbi convallis ullamcorper arcu vel ornare. Duis tempor tellus ut interdum convallis.";
	SDL_Rect r{0, 200, app->resX(), 100};
	//fonts->writeRect(_font, r, msg);
	fonts->writeLine(_font, 0, 200, msg);
	
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