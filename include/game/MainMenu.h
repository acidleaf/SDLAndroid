#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "pch.hpp"
#include "IScene.h"
#include "SpriteSheet.h"
#include "Button.h"

class MainMenu : public IScene {
protected:
	SpriteSheet _ss;
	Sprite _panel;
	Button _startBtn, _optionsBtn;
	
public:
	bool init();
	void release();
	void update();
	void render();
	void handleEvents(const SDL_Event& e);
};

#endif