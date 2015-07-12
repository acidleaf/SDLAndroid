#ifndef __SPRITESHEET_H___
#define __SPRITESHEET_H___

#include "pch.hpp"
#include "Sprite.h"

/*
	Currently we shall use the Sparrow TextureAtlas format
	Using Zwoptex to generate the spritesheets.
*/

class SpriteSheet {
protected:
	struct SubTexture {
		char name[MAX_NAME_SIZE];
		glm::vec2 pos;
		glm::vec2 size;
		glm::vec2 framePos;
		glm::vec2 frameSize;
	};
	
	SubTexture* _subTex = nullptr;
	size_t _numChildren = 0;
	
	SDL_Texture* _tex = nullptr;
	glm::ivec2 _size{0, 0};
	
public:
	bool init(const char* file);
	void release();
	
	size_t numChildren() const { return _numChildren; }
	
	Sprite getSprite(const char* name);
};

#endif