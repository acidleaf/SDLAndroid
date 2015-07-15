#ifndef __FONT_H__
#define __FONT_H__

#include "pch.hpp"
#include "stb_truetype.h"


class FontAtlas {
protected:
	stbtt_fontinfo _font;
	stbtt_packedchar* _charData;
	SDL_Texture* _tex = nullptr;
	int _w = 0, _h = 0;
	float _fontSize = 10.0f;
	
public:
	bool init(const char* filename, int w, int h, float fontSize);
	void release();
	
	void writeLine(const char* text, float x, float y);
	int getTextW(const char* text) const;
	
	void renderAtlas();
};

/*
class Font {
protected:
	stbtt_fontinfo _font;
	stbtt_packedchar* _charData;
	
	uint8_t* _texData;
	int _resX = 0, _resY = 0;
	float _fontSize = 16.0f;
	
public:
	bool init(const char* fontFile, int w, int h, float fontSize = 16.0f);
	void release();
	
	int getTextW(const char* text) const;
	void writeLine(const char* text, float x, float y, uint8_t* image, int imgW, int imgH, int imgBpp);
	
	void writeLine(const char* text, float x, float y);
	
	const uint8_t* texData() const { return _texData; }
};
*/

#endif