#include "MainScene.h"
#include "App.h"

bool MainScene::init() {
	auto app = App::getInstance();
	
	if (!_ss.init("spritesheets/ss-default.xml")) return false;
	
	_s1 = _ss.getSprite("logo.png");
	_s1.anchor(0.5f, 0.5f);
	_s1.pos(app->resX() / 2, app->resY() / 2);
	
	/*
	int w = 100, h = 100, bpp = 3;
	uint8_t* data = new uint8_t[w * h * bpp];
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			int index = (j * bpp) + (i * w * bpp);
			data[index + 0] = 128;
			data[index + 1] = 255;
		}
	}
	if (!_s2.init(data, w, h, bpp)) return false;
	delete[] data;
	*/
	
	
	_s2 = _ss.getSprite("circle.png");
	_s2.anchor(0.5f, 0.5f);
	_s2.pos(_s2.size().x / 2, _s2.size().y / 2);
	
	_v.x = 8.0f;
	
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
}

void MainScene::handleEvents(const SDL_Event& e) {
	auto app = App::getInstance();
	if (e.type == SDL_FINGERDOWN) {
		_s2.pos(app->resX() * e.tfinger.x, app->resY() * e.tfinger.y);
	}
}
