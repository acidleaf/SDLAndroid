#ifndef __FONT_H__
#define __FONT_H__

#include "pch.hpp"
#include "stb_truetype.h"

struct Font {
	char name[MAX_NAME_SIZE];
	int size = 0;
	stbtt_packedchar* data = nullptr;
	SDL_Texture* tex = nullptr;
};

class FontManager {
protected:
	Font* _fonts = nullptr;
	int _numFonts = 0;
	char _buffer[256];
	
	bool createFontSize(uint8_t* ttf, int size, Font* font, uint8_t* texData, SDL_Surface* surface);
	void releaseFont(Font* font);
public:
	bool init();
	void release();
	
	const Font* getFont(const char* name, int size) const;
	
	size_t lineWidth(const Font* font, const char* fmt, ...);
	void writeLine(const Font* font, float x, float y, const char* fmt, ...);
	void writeRect(const Font* font, SDL_Rect rect, const char* text);
};

#endif