#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_rect_pack.h"
#include "Font.h"

/*
#define GLYPH_START 0x20
#define NUM_GLYPHS 0x7E

// w, h: defines font atlas resolution
bool Font::init(const char* fontFile, int w, int h, float fontSize) {
	_resX = w;
	_resY = h;
	_fontSize = fontSize;
	
	// Read TTF font file into memory
	FILE* ttf = fopen(fontFile, "rb");
	if (!ttf) {
		printf("Error loading TTF file: %s\n", fontFile);
		return false;
	}
	
	fseek(ttf, 0, SEEK_END);
	size_t ttfSize = ftell(ttf);
	rewind(ttf);
	
	uint8_t* ttfData = new uint8_t[ttfSize];
	fread(ttfData, ttfSize, 1, ttf);
	fclose(ttf);
	
	
	int fontIndex = stbtt_GetFontOffsetForIndex(ttfData, 0);
	
	
	// Alloc single channel 8-bit image
	_texData = new uint8_t[_resX * _resY];
	
	// Start baking?
	stbtt_pack_context spc;
	if (!stbtt_PackBegin(&spc, _texData, _resX, _resY, 0, 1, nullptr)) {
		printf("stbtt_PackBegin failed\n");
		delete[] ttfData;
		delete[] _texData;
		return false;
	}
	
	const int numGlyphs = NUM_GLYPHS - GLYPH_START;
	_charData = new stbtt_packedchar[numGlyphs];
	
	if (!stbtt_PackFontRange(&spc, ttfData, fontIndex, _fontSize, GLYPH_START, numGlyphs, _charData)) {
		printf("stbtt_PackFontRange failed\n");
		delete[] ttfData;
		delete[] _texData;
		return false;
	}
	
	stbtt_PackEnd(&spc);
	
	delete[] ttfData;
	
	
	return true;
}

void Font::release() {
	if (_texData) delete[] _texData;
	if (_charData) delete[] _charData;
}

int Font::getTextW(const char* text) const {
	stbtt_aligned_quad q;
	int width = 0;
	float curX = 0, curY = 0;
	for (int t = 0; text[t]; ++t) {
		int c = text[t] - GLYPH_START;
		stbtt_GetPackedQuad(_charData, _resX, _resY, c, &curX, &curY, &q, 1);
	}
	return curX;
}

void Font::writeLine(const char* text, float x, float y, uint8_t* image, int imgW, int imgH, int imgBpp) {
	float curX = x, curY = y;
	const int imgPitch = imgW * imgBpp;
	stbtt_aligned_quad q;
	
	for (int t = 0; text[t]; ++t) {
		int c = text[t] - GLYPH_START;
		stbtt_GetPackedQuad(_charData, _resX, _resY, c, &curX, &curY, &q, 1);
		
		for (int i = q.y0, ii = _charData[c].y0; i < q.y1; ++i, ++ii) {
			if (i < 0 || i >= _resY) continue;
			for (int j = q.x0, jj = _charData[c].x0; j < q.x1; ++j, ++jj) {
				if (j < 0 || j >= _resX) continue;
				int pi = (j * imgBpp) + (i * imgPitch);
				int fi = jj + ii * _resX;
				
				image[pi + 0] = glm::clamp(image[pi + 0] + _texData[fi], 0, 255);
				image[pi + 1] = glm::clamp(image[pi + 1] + _texData[fi], 0, 255);
				image[pi + 2] = glm::clamp(image[pi + 2] + _texData[fi], 0, 255);
			}
		}
	}
}
*/