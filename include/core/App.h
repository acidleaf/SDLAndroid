#ifndef __APP_H__
#define __APP_H__

#include "pch.hpp"
#include "FPS.h"
#include "MainScene.h"
#include "SceneManager.h"


class App {
private:
	const int FPS = 60;
	const int SKIP_TICKS = 1000 / FPS;
	const int MAX_FRAMESKIP = 5;
	
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	
	int _resX, _resY, _pixelScale = 1;
	bool _done = false;
	bool _paused = false;
	
	uint32_t _nextTick = 0;
	
	FPSCounter _fps;
	SceneManager _scene;
	
	static int eventFilter(void* ptr, SDL_Event* e);
	
public:
	bool init(int pixelScale = 1);
	void release();
	
	void handleEvents();
	void update();
	void render();
	
	
	bool done() const { return _done; }
	SDL_Window* window() { return _window; }
	SDL_Renderer* renderer() { return _renderer; }
	
	int resX() const { return _resX; }
	int resY() const { return _resY; }
	
	SceneManager* scene() { return &_scene; }
	
	const int pixelScale() const { return _pixelScale; }
	
	static App*& getInstance();
};


#endif
