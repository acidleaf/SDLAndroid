#include "Button.h"
#include "App.h"

bool Button::init(const SpriteSheet& spritesheet, const char* normal, const char* pressed) {
	// Assuming that they all exist in the same texture
	if (!spritesheet.getTexture(normal, _tex, _rects[BTN_NORMAL])) return false;
	if (!spritesheet.getTexture(pressed, _tex, _rects[BTN_PRESSED])) return false;
	
	_size = { _rects[BTN_NORMAL].w, _rects[BTN_NORMAL].h };
	
	return true;
}

void Button::render() {
	glm::vec2 dstPos = _pos - ((glm::vec2)_size * _anchor);
	SDL_Rect dst{ (int)dstPos.x, (int)dstPos.y, _size.x, _size.y };
	
	SDL_RenderCopy(App::getInstance()->renderer(), _tex, &_rects[_state], &dst);
}

void Button::handleEvents(const SDL_Event& e) {
	auto app = App::getInstance();
	glm::vec2 topLeft = _pos - ((glm::vec2)_size * _anchor);
	SDL_Rect btnRect{ (int)topLeft.x, (int)topLeft.y, _size.x, _size.y };
	
	if (e.type == SDL_FINGERDOWN) {
		glm::vec2 p{ e.tfinger.x * app->resX(), e.tfinger.y * app->resY() };
		
		if (p.x >= btnRect.x && p.x <= btnRect.x + btnRect.w &&
			p.y >= btnRect.y && p.y <= btnRect.y + btnRect.h) {
			_state = BTN_PRESSED;
		}
	} else if (e.type == SDL_FINGERUP) {
		_state = BTN_NORMAL;
	}
}
