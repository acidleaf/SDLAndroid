#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__

#include "IScene.h"
#include "Sprite.h"
#include "TextureAtlas.h"
#include "Font.h"
#include "AnimSprite.h"


class MainScene : public IScene {
protected:
	Sprite _s1, _s2;
	AnimSprite _as;
	TextureAtlas _ss;
	
	glm::vec2 _v;
	
	const Font* _font;
	
public:
	bool init();
	void release();
	void update();
	void render();
	void handleEvents(const SDL_Event& e);
};

#endif