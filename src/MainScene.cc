#include "MainScene.h"
#include "App.h"

bool MainScene::init() {
	auto app = App::getInstance();
	
	if (!_s1.init("logo.png")) return false;
	_s1.anchor(0.5f, 0.5f);
	_s1.pos(app->resX() / 2, app->resY() / 2);
	
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
	
	return true;
}

void MainScene::release() {
	_s1.release();
	_s2.release();
}

void MainScene::update() {
}

void MainScene::render() {
	_s1.render();
	_s2.render();
}

void MainScene::handleEvents(const SDL_Event& e) {
}
