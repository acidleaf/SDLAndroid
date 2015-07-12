#include "Sprite.h"
#include "App.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Sprite::init(const char* file) {
	// We force 4 channel textures for now
	uint8_t* data = loadPNG(file);
	if (!data) {
		printf("Error loading texture: %s\n", file);
		return false;
	}
	_srcRect = {0, 0, _size.x, _size.y};
	SDL_Surface* surf = SDL_CreateRGBSurface(0, _srcRect.w, _srcRect.h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	
	SDL_LockSurface(surf);
	uint8_t* pixels = (uint8_t*)surf->pixels;
	memcpy(pixels, data, _srcRect.w * _srcRect.h * 4);
	SDL_UnlockSurface(surf);
	
	_tex = SDL_CreateTextureFromSurface(App::getInstance()->renderer(), surf);
	
	SDL_FreeSurface(surf);
	if (data) delete[] data;
	
	return true;
}

bool Sprite::init(uint8_t* data, int w, int h, int bpp) {
	if (!data || !w || !h) return false;
	if (bpp != 1 && bpp != 3 && bpp != 4) return false;
	
	_size = {w, h};
	_srcRect = {0, 0, w, h};
	
	SDL_Surface* surf = SDL_CreateRGBSurface(0, _size.x, _size.y, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	
	SDL_LockSurface(surf);
	uint8_t* pixels = (uint8_t*)surf->pixels;
	if (bpp == 4) memcpy(pixels, data, w * h * bpp);
	else {
		for (int i = 0; i < _size.y; ++i) {
			for (int j = 0; j < _size.x; ++j) {
				const uint32_t srcI = j * bpp + i * w * bpp;
				const uint32_t dstI = j * 4 + i * surf->pitch;
				
				if (bpp == 1) {
					pixels[dstI + 0] = data[srcI];
					pixels[dstI + 1] = data[srcI];
					pixels[dstI + 2] = data[srcI];
					pixels[dstI + 3] = 255;
				} else {
					pixels[dstI + 0] = data[srcI + 0];
					pixels[dstI + 1] = data[srcI + 1];
					pixels[dstI + 2] = data[srcI + 2];
					pixels[dstI + 3] = 255;
				}
			}
		}
	}
	SDL_UnlockSurface(surf);
	
	_tex = SDL_CreateTextureFromSurface(App::getInstance()->renderer(), surf);
	
	SDL_FreeSurface(surf);
	
	return true;
}

void Sprite::release() {
	SDL_DestroyTexture(_tex);
}

void Sprite::render() {
	glm::vec2 dstPos = _pos - ((glm::vec2)_size * _anchor);
	SDL_Rect dst{ (int)dstPos.x, (int)dstPos.y, _size.x, _size.y };
	SDL_RenderCopy(App::getInstance()->renderer(), _tex, &_srcRect, &dst);
}


uint8_t* Sprite::loadPNG(const char* filename) {
	// Load the PNG data into memory
	SDL_RWops* pngFile = SDL_RWFromFile(filename, "rb");
	if (!pngFile) {
		SDL_Log("Error loading image: %s", filename);
		return nullptr;
	}
	
	Sint64 pngSize = SDL_RWseek(pngFile, 0, RW_SEEK_END);
	SDL_RWseek(pngFile, 0, RW_SEEK_SET);
	
	uint8_t* pngData = new uint8_t[pngSize];
	SDL_RWread(pngFile, pngData, pngSize, 1);
	SDL_RWclose(pngFile);
	
	uint8_t* imgData = stbi_load_from_memory(pngData, pngSize, &_size.x, &_size.y, nullptr, 4);
	if (!imgData) {
		SDL_Log("Invalid image: %s", filename);
		if (pngData) delete[] pngData;
		return nullptr;
	}
	
	if (pngData) delete[] pngData;
	return imgData;
}