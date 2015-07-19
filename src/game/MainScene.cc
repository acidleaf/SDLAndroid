#include "MainScene.h"
#include "App.h"

bool MainScene::init() {
	auto app = App::getInstance();
	
	if (!_ss.init("spritesheets/mainScene.xml")) return false;
	
	_ss.getSprite("logo", _s1);
	_s1.anchor(0.5f, 0.5f);
	_s1.pos(app->resX() / 2, app->resY() / 2);
	
	_ss.getSprite("circle", _s2);
	_s2.anchor(0.5f, 0.5f);
	_s2.pos(_s2.size().x / 2, _s2.size().y / 2);
	
	SDL_Texture* animTex;
	SDL_Rect animRect;
	_ss.getTexture("frames", animTex, animRect);
	_as.init(animTex, animRect, 64, 64, 16, 4);
	_as.pos(100, 100);
	
	_v.x = 6.0f;
	
	_font = app->fonts()->getFont("04b03.ttf", 8);
	
	return true;
}

void MainScene::release() {
	_s1.release();
	_s2.release();
	_ss.release();
	_as.release();
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
	
	_as.update();
}

void MainScene::render() {
	_s1.render();
	_s2.render();
	_as.render();
	
	auto app = App::getInstance();
	auto fonts = app->fonts();
	
	const char* msg = "Lorem ipsum dolor sit amet";
	//SDL_Rect r{0, 200, app->resX(), 100};
	//fonts->writeRect(_font, r, msg);
	fonts->writeLine(_font, 0, 200, msg);
	
}

void MainScene::handleEvents(const SDL_Event& e) {
	auto app = App::getInstance();
	
	if (e.type == SDL_FINGERDOWN) {
		_s2.pos(app->resX() * e.tfinger.x, app->resY() * e.tfinger.y);
	}
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_AC_BACK) {
		app->scene()->pop();
	}
}