#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_rect_pack.h"
#include "Font.h"
#include "App.h"
#include "Utils.h"


bool FontManager::init() {
	_numFonts = 4;
	const int fontSizes[] = { 8, 16, 24, 32 };
	const char* fontName = "04b03.ttf";
	
	// Allocate font
	_fonts = new Font[_numFonts];
	
	// Load ttf
	size_t ttfSize;
	uint8_t* ttfData = loadFileToBuffer(fontName, &ttfSize);
	if (!ttfData) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to load font: %s", fontName);
		return false;
	}
	
	uint8_t* tmpTex = new uint8_t[TEX_W * TEX_H];
	SDL_Surface* tmpSurf = SDL_CreateRGBSurface(0, TEX_W, TEX_H, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	
	// Load fonts
	bool rc = true;
	for (int i = 0; i < _numFonts; ++i) {
		strcpy(_fonts[i].name, fontName);
		if (!createFontSize(ttfData, fontSizes[i], &_fonts[i], tmpTex, tmpSurf)) {
			SDL_LogWarn(SDL_LOG_CATEGORY_ERROR, "Unable to create font %s with size %d", fontName, fontSizes[i]);
			rc = false;
		}
	}
	
	if (ttfData) delete[] ttfData;
	if (tmpTex) delete[] tmpTex;
	if (tmpSurf) SDL_FreeSurface(tmpSurf);
	
	
	return rc;
}

bool FontManager::createFontSize(uint8_t* ttf, int size, Font* font, uint8_t* tmpTex, SDL_Surface* tmpSurf) {
	font->size = size;
	font->data = new stbtt_packedchar[NUM_GLYPHS];
	
	stbtt_pack_context spc;
	if (!stbtt_PackBegin(&spc, tmpTex, TEX_W, TEX_H, 0, 1, nullptr)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "stbtt_PackBegin failed\n");
		return false;
	}
	
	if (!stbtt_PackFontRange(&spc, ttf, 0, font->size, GLYPH_START, NUM_GLYPHS, font->data)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "stbtt_PackFontRange failed\n");
		return false;
	}
	stbtt_PackEnd(&spc);
	
	SDL_LockSurface(tmpSurf);
	uint8_t* pixels = (uint8_t*)tmpSurf->pixels;
	memset(pixels, 255, TEX_W * TEX_H * 4);
	const int pitch = TEX_W * 4;
	for (int i = 0; i < TEX_H; ++i) {
		for (int j = 0; j < TEX_W; ++j) {
			int index = (j * 4) + (i * pitch);
			pixels[index + 3] = tmpTex[j + i * TEX_W];
		}
	}
	SDL_UnlockSurface(tmpSurf);
	font->tex = SDL_CreateTextureFromSurface(App::getInstance()->renderer(), tmpSurf);
	
	return true;
}


void FontManager::releaseFont(Font* font) {
	if (font->data) delete[] font->data;
	if (font->tex) SDL_DestroyTexture(font->tex);
}
void FontManager::release() {
	for (int i = 0; i < _numFonts; ++i) {
		releaseFont(&_fonts[i]);
	}
	if (_fonts) delete[] _fonts;
}

const Font* FontManager::getFont(const char* name, int size) const {
	for (int i = 0; i < _numFonts; ++i) {
		if (!strcmp(_fonts[i].name, name) && _fonts[i].size == size) return &_fonts[i];
	}
	return nullptr;
}


size_t FontManager::lineWidth(const Font* font, const char* fmt, ...) {
	if (!font) return 0;
	
	va_list args;
	va_start(args, fmt);
	vsprintf(_buffer, fmt, args);
	va_end(args);
	
	stbtt_aligned_quad q;
	float curX = 0, curY = 0;
	for (int t = 0; _buffer[t]; ++t) {
		int c = _buffer[t] - GLYPH_START;
		stbtt_GetPackedQuad(font->data, TEX_W, TEX_H, c, &curX, &curY, &q, 1);
	}
	return curX;
}

void FontManager::writeLine(const Font* font, float x, float y, const char* fmt, ...) {
	if (!font) return;
	
	va_list args;
	va_start(args, fmt);
	vsprintf(_buffer, fmt, args);
	va_end(args);
	
	auto app = App::getInstance();
	float curX = x, curY = y;
	stbtt_aligned_quad q;
	int lines = 0;
	for (int t = 0; _buffer[t] && t < BUFFER_SIZE; ++t) {
		if (_buffer[t] == '\n') {
			++lines;
			curX = x;
			curY = y + lines * font->size;
			continue;
		}
		int ci = _buffer[t] - GLYPH_START;
		stbtt_GetPackedQuad(font->data, TEX_W, TEX_H, ci, &curX, &curY, &q, 1);
		
		const auto& c = font->data[ci];
		SDL_Rect src{ (int)c.x0, (int)c.y0, (int)(c.x1 - c.x0), (int)(c.y1 - c.y0) };
		SDL_Rect dst{ (int)(q.x0), (int)(q.y0), (int)(q.x1 - q.x0), (int)(q.y1 - q.y0) };
		
		SDL_RenderCopy(app->renderer(), font->tex, &src, &dst);
	}
	_lastLineW = curX - x;
}

void FontManager::writeRect(const Font* font, SDL_Rect rect, const char* text) {
	if (!font) return;
	
	auto app = App::getInstance();
	int curLine = 0;
	float cx = rect.x, cy = rect.y;
	stbtt_aligned_quad q;
	
	while (*text) {
		float curW = 0.0f;
		const char* start = text;
		const char* lastSpace = text;
		int revCount = 0;
		// Advance through until the text overflows or text ends
		while (*text && curW < rect.w) {
			float dx = 0.0f, dy = 0.0f;
			bool stop = false;
			if (*text == ' ') {
				lastSpace = text;
				revCount = 0;
			} else if (*text == '\n') {
				lastSpace = text;
				stop = true;
			}
			
			int ci = *text - GLYPH_START;
			stbtt_GetPackedQuad(font->data, TEX_W, TEX_H, ci, &dx, &dy, &q, 1);
			curW += dx;
			++text;
			++revCount;
			if (stop) {
				revCount = 0;
				break;
			}
		}
		
		// Print from start until lastSpace
		for (const char* t = start; t != lastSpace; ++t) {
			int ci = *t - GLYPH_START;
			stbtt_GetPackedQuad(font->data, TEX_W, TEX_H, ci, &cx, &cy, &q, 1);
			
			const auto& c = font->data[ci];
			SDL_Rect src{ (int)c.x0, (int)c.y0, (int)(c.x1 - c.x0), (int)(c.y1 - c.y0) };
			SDL_Rect dst{ (int)q.x0, (int)q.y0, (int)(q.x1 - q.x0), (int)(q.y1 - q.y0) };
			
			SDL_RenderCopy(app->renderer(), font->tex, &src, &dst);
		}
		
		// Line printed! Update cursor
		++curLine;
		cx = rect.x;
		cy = rect.y + curLine * font->size;
		
		// Rewind until last space + 1
		text -= (revCount - 1);
	}
}