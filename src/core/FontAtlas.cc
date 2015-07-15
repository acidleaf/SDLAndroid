#include "Font.h"
#include "App.h"
#include "Utils.h"

#define GLYPH_START 0x20
#define GLYPH_END 0x7E

bool FontAtlas::init(const char* filename, int w, int h, float fontSize) {
	_w = w;
	_h = h;
	_fontSize = fontSize;
	
	// Read TTF font file into memory
	size_t ttfSize;
	uint8_t* ttfData = loadFileToBuffer(filename, &ttfSize);
	
	// Alloc single channel 8-bit image
	uint8_t* texData = new uint8_t[_w * _h];
	
	// Start baking?
	stbtt_pack_context spc;
	if (!stbtt_PackBegin(&spc, texData, _w, _h, 0, 1, nullptr)) {
		SDL_Log("stbtt_PackBegin failed\n");
		delete[] ttfData;
		delete[] texData;
		return false;
	}
	
	const int numGlyphs = GLYPH_END - GLYPH_START + 1;
	_charData = new stbtt_packedchar[numGlyphs];
	
	// No anti-aliasing
	//stbtt_PackSetOversampling(&spc, 2, 2);
	
	//int fontIndex = stbtt_GetFontOffsetForIndex(ttfData, 0);
	//STBTT_POINT_SIZE
	if (!stbtt_PackFontRange(&spc, ttfData, 0, _fontSize, GLYPH_START, numGlyphs, _charData)) {
		SDL_Log("stbtt_PackFontRange failed\n");
		delete[] ttfData;
		delete[] texData;
		return false;
	}
	
	stbtt_PackEnd(&spc);

	
	SDL_Surface* surf = SDL_CreateRGBSurface(0, _w, _h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	SDL_LockSurface(surf);
	uint8_t* pixels = (uint8_t*)surf->pixels;
	memset(pixels, 0, _w * _h * 4);
	for (int i = 0; i < _h; ++i) {
		for (int j = 0; j < _w; ++j) {
			int index = (j * 4) + (i * _w * 4);
			if (!texData[j + i * _w]) continue;
			pixels[index + 0] = 255;
			pixels[index + 1] = 255;
			pixels[index + 2] = 255;
			pixels[index + 3] = texData[j + i * _w];
		}
	}
	SDL_UnlockSurface(surf);
	_tex = SDL_CreateTextureFromSurface(App::getInstance()->renderer(), surf);
	SDL_FreeSurface(surf);
	
	if (ttfData) delete[] ttfData;
	if (texData) delete[] texData;
	
	return true;
}


void FontAtlas::release() {
	SDL_DestroyTexture(_tex);
	if (_charData) delete[] _charData;
}

void FontAtlas::renderAtlas() {
	auto app = App::getInstance();
	//glm::vec2 dstPos = _pos - ((glm::vec2)_size * _anchor);
	SDL_Rect dst{ 0, 0, _w, _h };
	
	SDL_RenderCopy(app->renderer(), _tex, nullptr, &dst);
}

void FontAtlas::writeLine(const char* text, float x, float y) {
	auto app = App::getInstance();
	float curX = x, curY = y;
	stbtt_aligned_quad q;
	
	for (int t = 0; text[t]; ++t) {
		int ci = text[t] - GLYPH_START;
		stbtt_GetPackedQuad(_charData, _w, _h, ci, &curX, &curY, &q, 1);
		
		const auto& c = _charData[ci];
		SDL_Rect src{ (int)c.x0, (int)c.y0, (int)(c.x1 - c.x0), (int)(c.y1 - c.y0) };
		SDL_Rect dst{ (int)q.x0, (int)q.y0, (int)(q.x1 - q.x0), (int)(q.y1 - q.y0) };
		
		
		SDL_RenderCopy(app->renderer(), _tex, &src, &dst);
		/*
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
		*/
	}
}