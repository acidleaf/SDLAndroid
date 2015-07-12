#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__

#include "IScene.h"
#include "Sprite.h"

class MainScene : public IScene {
protected:
	Sprite _s1, _s2;
	
public:
	bool init();
	void release();
	void update();
	void render();
	void handleEvents(const SDL_Event& e);
};

#endif