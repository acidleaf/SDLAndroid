#include "Button.h"
#include "App.h"
#include "Utils.h"

bool Button::init(const SpriteSheet* spritesheet, const char* normal, const char* pressed) {
	// Assuming that they all exist in the same texture
	if (!spritesheet->getTexture(normal, _tex, _rects[BTN_UP])) return false;
	if (!spritesheet->getTexture(pressed, _tex, _rects[BTN_DOWN])) return false;
	
	_size = { _rects[BTN_UP].w, _rects[BTN_UP].h };
	
	return true;
}

void Button::render() {
	glm::vec2 dstPos = _pos - ((glm::vec2)_size * _anchor);
	SDL_Rect dst{ (int)dstPos.x, (int)dstPos.y, _size.x, _size.y };
	
	SDL_RenderCopy(App::getInstance()->renderer(), _tex, &_rects[_state], &dst);
}

bool Button::handleEvents(const SDL_Event& e) {
	auto app = App::getInstance();
	glm::vec2 topLeft = _pos - ((glm::vec2)_size * _anchor);
	SDL_Rect btnRect{ (int)topLeft.x, (int)topLeft.y, _size.x, _size.y };
	glm::vec2 p{ e.tfinger.x * app->resX(), e.tfinger.y * app->resY() };
	
	if (e.type == SDL_FINGERDOWN) {
		if (pointInRect(p, btnRect)) {
			_state = BTN_DOWN;
			return true;
		}
	} else if (e.type == SDL_FINGERUP) {
		_state = BTN_UP;
		if (pointInRect(p, btnRect)) {
			if (_onClick) _onClick(this, _callbackData);
		}
	} else if (e.type == SDL_FINGERMOTION) {
		if (_state == BTN_DOWN && !pointInRect(p, btnRect)) {
			_state = BTN_UP;
		}
	}
	
	return false;
}

void Button::onClick(BtnCallback callback, void* data) {
	_onClick = callback;
	_callbackData = data;
}