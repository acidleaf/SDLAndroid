#include "SpriteSheet.h"
#include "pugixml.hpp"
#include "Utils.h"
#include "App.h"

bool SpriteSheet::init(const char* filename) {
	// Load spritesheet xml
	size_t xmlSize;
	uint8_t* xmlData = loadFileToBuffer(filename, &xmlSize);
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_buffer_inplace(xmlData, xmlSize);
	
	if (!result) {
		SDL_Log("Error parsing document %s: %s\n", filename, result.description());
		SDL_Log("  at offset: %td\n", result.offset);
		return false;
	}
	
	// XML document root
	pugi::xml_node root = doc.child("TextureAtlas");
	
	// Prepend folder
	char imagePath[MAX_NAME_SIZE * 2];
	sprintf(imagePath, "spritesheets/%s", root.attribute("imagePath").value());
	
	// Load image
	uint8_t* data = loadPNG(imagePath, &_size.x, &_size.y);
	if (!data) {
		SDL_Log("Error loading spritesheet image: %s\n", imagePath);
		return false;
	}
	
	// Create SDL_Texture
	SDL_Surface* surf = SDL_CreateRGBSurface(0, _size.x, _size.y, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	SDL_LockSurface(surf);
	uint8_t* pixels = (uint8_t*)surf->pixels;
	memcpy(pixels, data, _size.x * _size.y * 4);
	SDL_UnlockSurface(surf);
	_tex = SDL_CreateTextureFromSurface(App::getInstance()->renderer(), surf);
	SDL_FreeSurface(surf);
	delete[] data;
	
	// Count number of subtextures
	for (pugi::xml_node subTex = root.child("SubTexture"); subTex; subTex = subTex.next_sibling("SubTexture")) {
		++_numChildren;
	}
	
	// Allocate memory for subtextures
	_subTex = new SubTexture[_numChildren];
	
	// Parse the subtexture attributes and store them
	int i = 0;
	for (pugi::xml_node st = root.child("SubTexture"); st; st = st.next_sibling("SubTexture"), ++i) {
		memset(_subTex[i].name, 0, MAX_NAME_SIZE);
		strncpy(_subTex[i].name, st.attribute("name").value(), MAX_NAME_SIZE - 1);
		_subTex[i].pos = { st.attribute("x").as_float(), st.attribute("y").as_float() };
		_subTex[i].size = { st.attribute("width").as_float(), st.attribute("height").as_float() };
		_subTex[i].framePos = { st.attribute("frameX").as_float(), st.attribute("frameY").as_float() };
		_subTex[i].frameSize = { st.attribute("frameWidth").as_float(), st.attribute("frameHeight").as_float() };
	}
	
	// Release xml data buffer
	if (xmlData) delete[] xmlData;
	
	return true;
}

void SpriteSheet::release() {
	if (_subTex) delete[] _subTex;
	SDL_DestroyTexture(_tex);
}

Sprite SpriteSheet::getSprite(const char* name) const {
	for (int i = 0; i < _numChildren; ++i) {
		if (!strcmp(_subTex[i].name, name)) {
			Sprite s;
			s.init(_tex, {
				(int)_subTex[i].pos.x,
				(int)_subTex[i].pos.y,
				(int)_subTex[i].size.x,
				(int)_subTex[i].size.y
			});
			return s;
		}
	}
	SDL_Log("Error: %s not found in sprite sheet", name);
}

bool SpriteSheet::getTexture(const char* name, SDL_Texture*& texture, SDL_Rect& rect) const {
	for (int i = 0; i < _numChildren; ++i) {
		if (!strcmp(_subTex[i].name, name)) {
			texture = _tex;
			rect.x = (int)_subTex[i].pos.x;
			rect.y = (int)_subTex[i].pos.y;
			rect.w = (int)_subTex[i].size.x;
			rect.h = (int)_subTex[i].size.y;
			return true;
		}
	}
	SDL_Log("Error: %s not found in sprite sheet", name);
	return false;
}