#ifndef __APP_H__
#define __APP_H__

#include "pch.hpp"
#include "FPS.h"
#include "MainScene.h"

class App {
private:
	const int FPS = 60;
	const int SKIP_TICKS = 1000 / FPS;
	const int MAX_FRAMESKIP = 5;
	
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	
	int _resX, _resY;
	bool _done = false;
	
	uint32_t _nextTick = 0;
	
	FPSCounter _fps;
	MainScene _scene;
	
public:
	App();
	~App();
	
	bool init(const char* title);
	void release();
	
	void handleEvents();
	void update();
	void render();
	
	
	bool done() const { return _done; }
	SDL_Window* window() { return _window; }
	SDL_Renderer* renderer() { return _renderer; }
	
	int resX() const { return _resX; }
	int resY() const { return _resY; }
	
	static App*& getInstance();
};


#endif
