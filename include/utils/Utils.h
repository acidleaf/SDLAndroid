#ifndef __UTILS_H__
#define __UTILS_H__

#include "pch.hpp"

uint8_t* loadFileToBuffer(const char* filename, size_t* size);
uint8_t* loadPNG(const char* filename, int* w, int* h);

inline bool pointInRect(const glm::vec2& p, const SDL_Rect& r) {
	return (p.x >= r.x && p.x <= r.x + r.w && p.y >= r.y && p.y <= r.y + r.h);
}

#endif