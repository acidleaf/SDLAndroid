#ifndef __UTILS_H__
#define __UTILS_H__

#include "pch.hpp"

uint8_t* loadFileToBuffer(const char* filename, size_t* size);
uint8_t* loadPNG(const char* filename, int* w, int* h);

#endif