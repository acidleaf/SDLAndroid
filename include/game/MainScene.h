#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__

#include "IScene.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Button.h"


class MainScene : public IScene {
protected:
	Sprite _s1, _s2;
	SpriteSheet _ss;
	
	glm::vec2 _v;
	
	Button _btn;
	
	void btnClicked(Button* target);
	
public:
	bool init();
	void release();
	void update();
	void render();
	void handleEvents(const SDL_Event& e);
};

#endif