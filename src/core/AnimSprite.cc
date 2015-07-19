#include "AnimSprite.h"
#include "App.h"

bool AnimSprite::init(SDL_Texture* tex, const SDL_Rect& rect, int frameW, int frameH, int frames, float fps) {
	if (!tex) return false;
	
	_tex = tex;
	_texRect = rect;
	
	_framesX = _texRect.w / frameW;
	_framesY = _texRect.h / frameH;
	_numFrames = frames;
	
	_size.x = frameW;
	_size.y = frameH;
	
	_srcRect = {0, 0, _size.x, _size.y};
	
	_totalTime = 1.0f / fps * _numFrames;
	
	
	return true;
}

void AnimSprite::update() {
	_timePassed += App::getInstance()->dt();
	if (_timePassed >= _totalTime) _timePassed -= _totalTime;
	const float t = _timePassed / _totalTime;
	_curFrame = t * _numFrames;
	
	const int fx = _curFrame % _framesX;
	const int fy = _curFrame / _framesX;
	
	_srcRect = {_texRect.x + fx * _size.x, _texRect.y + fy * _size.y, _size.x, _size.y };
}
