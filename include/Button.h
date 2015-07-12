#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "pch.hpp"
#include "Sprite.h"
#include "SpriteSheet.h"

enum ButtonState {
	BTN_NORMAL,
	BTN_PRESSED,
	NUM_BTN_STATES
};

class Button : public Sprite {
private:
	using Sprite::init;
	
protected:
	ButtonState _state = BTN_NORMAL;
	SDL_Texture* _tex = nullptr;
	SDL_Rect _rects[NUM_BTN_STATES];
	
public:
	bool init(const SpriteSheet& spritesheet, const char* normal, const char* pressed);
	void render();
	void handleEvents(const SDL_Event& e);
	
	bool pressed() const { return _state == BTN_PRESSED; }
};

#endif