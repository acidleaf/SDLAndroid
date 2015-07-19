#ifndef __ANIMSPRITE_H__
#define __ANIMSPRITE_H__

#include "Sprite.h"

class AnimSprite : public Sprite {
protected:
	SDL_Rect _texRect;
	int _framesX = 0, _framesY = 0, _numFrames = 0;
	int _curFrame = 0;
	float _timePassed = 0, _totalTime = 0;
	
	
	using Sprite::init;
	
public:
	bool init(SDL_Texture* tex, const SDL_Rect& rect, int frameW, int frameH, int frames, float fps = 1.0f);
	
	void update();
};

#endif