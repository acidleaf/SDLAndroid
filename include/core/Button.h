#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "pch.hpp"
#include "Sprite.h"
#include "SpriteSheet.h"

enum ButtonState {
	BTN_UP,
	BTN_DOWN,
	NUM_BTN_STATES
};

class Button : public Sprite {
private:
	using Sprite::init;
	
protected:
	typedef void (*BtnCallback)(Button* target, void* data);
	
	ButtonState _state = BTN_UP;
	SDL_Texture* _tex = nullptr;
	SDL_Rect _rects[NUM_BTN_STATES];
	
	BtnCallback _onClick = nullptr;
	void* _callbackData = nullptr;
	
public:
	bool init(const SpriteSheet* spritesheet, const char* normal, const char* pressed);
	void render();
	bool handleEvents(const SDL_Event& e);
	
	void onClick(BtnCallback callback, void* data);
};

#endif