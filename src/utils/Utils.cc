#include "Utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "pugixml.hpp"


uint8_t* loadFileToBuffer(const char* filename, size_t* size) {
	SDL_RWops* file = SDL_RWFromFile(filename, "rb");
	if (!file) {
		SDL_Log("Error loading file: %s", filename);
		return nullptr;
	}
	
	Sint64 fileSize = SDL_RWseek(file, 0, RW_SEEK_END);
	SDL_RWseek(file, 0, RW_SEEK_SET);
	
	uint8_t* data = new uint8_t[fileSize];
	SDL_RWread(file, data, fileSize, 1);
	SDL_RWclose(file);
	
	*size = fileSize;
	return data;
}

uint8_t* loadPNG(const char* filename, int* w, int* h) {
	// Load the PNG data into memory
	size_t pngSize;
	uint8_t* pngData = loadFileToBuffer(filename, &pngSize);
	uint8_t* imgData = stbi_load_from_memory(pngData, pngSize, w, h, nullptr, 4);
	if (!imgData) {
		SDL_Log("Invalid image: %s", filename);
		if (pngData) delete[] pngData;
		return nullptr;
	}
	
	if (pngData) delete[] pngData;
	return imgData;
}
