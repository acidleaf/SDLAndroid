#ifndef __TIMER_H__
#define __TIMER_H__

#include "pch.hpp"

class Timer {
public:
	static uint32_t getTimeMs() { return SDL_GetTicks(); }
	static double getTime() { return SDL_GetTicks() * 0.001; }
};

#endif