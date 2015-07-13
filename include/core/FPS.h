#ifndef __FPS_H__
#define __FPS_H__

#include "pch.hpp"

class FPSCounter {
private:
	bool _first = true;
	uint32_t _frames = 0;
	uint32_t _start = 0;
	float _fps = 0.0f;
	
public:
	void update() {
		if (_first) {
			_first = false;
			_start = SDL_GetTicks();
			_frames = 0;
			return;
		}
		
		++_frames;
		uint32_t frameTime = SDL_GetTicks() - _start;
		if (frameTime >= 1000) {
			_fps = _frames * 1000.0f / frameTime;
			_start = SDL_GetTicks();
			_frames = 0;
		}
	}
	
	float operator()() const { return _fps; }
};

#endif