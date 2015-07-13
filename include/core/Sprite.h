#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "pch.hpp"

class Sprite {
protected:
	SDL_Texture* _tex = nullptr;
	SDL_Rect _srcRect;
	
	glm::ivec2 _size{0, 0};
	glm::vec2 _pos{0, 0}, _anchor{0, 0};
	
	float _alpha = 1.0f;
	
public:
	Sprite() {}
	virtual ~Sprite() {}
	
	virtual bool init(const char* file);
	virtual bool init(uint8_t* data, int w, int h, int bpp);
	virtual bool init(SDL_Texture* tex, SDL_Rect rect);
	virtual void release();
	
	virtual void render();
	
	
	virtual void anchor(float x, float y) { _anchor = {x, y}; }
	virtual glm::vec2& anchor() { return _anchor; }
	
	virtual void pos(float x, float y) { _pos = {x, y}; }
	virtual glm::vec2& pos() { return _pos; }
	
	virtual float& alpha() { return _alpha; }
	
	virtual const glm::ivec2& size() const { return _size; }
};

#endif