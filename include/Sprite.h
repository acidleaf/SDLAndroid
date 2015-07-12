#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "pch.hpp"

class Sprite {
protected:
	SDL_Texture* _tex = nullptr;
	SDL_Rect _srcRect;
	
	glm::ivec2 _size;
	glm::vec2 _pos, _anchor;
	
	uint8_t* loadPNG(const char* filename);
	
public:
	Sprite() {}
	virtual ~Sprite() {}
	
	virtual bool init(const char* file);
	virtual bool init(uint8_t* data, int w, int h, int bpp);
	virtual void release();
	
	virtual void render();
	
	
	virtual void anchor(float x, float y) { _anchor = {x, y}; }
	virtual glm::vec2& anchor() { return _anchor; }
	
	virtual void pos(float x, float y) { _pos = {x, y}; }
	virtual glm::vec2& pos() { return _pos; }
	
	virtual const glm::ivec2& size() const { return _size; }
};

#endif