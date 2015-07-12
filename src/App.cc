#include "App.h"
#include "Timer.h"

static App* _appInstance = nullptr;

App*& App::getInstance() {
	if (!_appInstance) _appInstance = new App();
	return _appInstance;
}


App::App() {}

App::~App() {}

bool App::init(const char* title) {
	
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}
	
	// Get display size since we're on mobile
	SDL_DisplayMode displayMode;
	if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0) {
		_resX = displayMode.w;
		_resY = displayMode.h;
	} else {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error retrieving display mode: %s", SDL_GetError());
	}
	
	
	
	// Init window
	_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _resX, _resY, 0);
	if (!_window) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating window: %s\n", SDL_GetError());
		return false;
	}
	
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!_renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating renderer: %s\n", SDL_GetError());
		return false;
	}
	
	SDL_version v;
	SDL_GetVersion(&v);
	SDL_Log("SDL version: %d.%d.%d\n", v.major, v.minor, v.patch);
	
	
	if (!_scene.init()) return false;
	
	
	return true;
}

void App::release() {
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}




void App::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) _done = true;
		
		// Quit when ESC key is pressed
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) _done = true;
	}
}



void App::update() {
	const uint32_t curTime = Timer::getTimeMs();
	int loops = 0;
	
	
	while(curTime > _nextTick && loops < MAX_FRAMESKIP) {
		// Update stuff here
		_scene.update();
		
		_nextTick += SKIP_TICKS;
		++loops;
		
		_fps.update();
	}
	
	//float t = float(curTime + SKIP_TICKS - _nextTick) / (float)SKIP_TICKS;
	render();
}

void App::render() {
	SDL_RenderClear(_renderer);
	
	// Render stuff here
	_scene.render();
	
	SDL_RenderPresent(_renderer);
}
