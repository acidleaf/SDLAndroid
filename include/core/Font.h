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
	static const int GLYPH_START = 0x20;
	static const int GLYPH_END = 0x7E;
	static const int NUM_GLYPHS = (GLYPH_END - GLYPH_START);
	static const int TEX_W = 128;
	static const int TEX_H = 256;
	static const int BUFFER_SIZE = 512;

	Font* _fonts = nullptr;
	int _numFonts = 0;
	size_t _lastLineW = 0;
	
	// Scratch buffers used for printing, etc
	// Increase size accordingly
	char _buffer[BUFFER_SIZE];
	
	bool createFontSize(uint8_t* ttf, int size, Font* font, uint8_t* texData, SDL_Surface* surface);
	void releaseFont(Font* font);
	
	
public:
	bool init();
	void release();
	
	const Font* getFont(const char* name, int size) const;
	size_t lastLineWidth() const { return _lastLineW; }
	size_t lineWidth(const Font* font, const char* fmt, ...);
	void writeLine(const Font* font, float x, float y, const char* fmt, ...);
	void writeRect(const Font* font, SDL_Rect rect, const char* text);
};

#endif